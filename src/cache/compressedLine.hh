#ifndef CACHE_COMPRESSEDLINE_HH
#define CACHE_COMPRESSEDLINE_HH
#include "cache/line.hh"
#include "common/bit/bitvec.hh"
#include <stdio.h>
#include <vector>
#include <cstring>
#include <cmath>
#include <assert.h>

using namespace std;

class BaseCompressedLine : public Line
{
    public:
        int m_compressed_size;
        // BaseCompressedLine should be able to be constructed from a Line
        BaseCompressedLine(Line* line, int compressed_size) : Line(line->m_size, line->m_set, line->m_bank, line->m_addr, line->m_segs)
        {
            m_compressed_size = compressed_size;
        }

        int get_compressed_size() const
        {
            return m_compressed_size;
        }

        bool is_compressed() const
        {
            return m_compressed_size < m_size;
        }

        void print()
        {
            printf("[ base compressed line addr: %16lx, physical size: %d, set: %d, bank: %d, data: ", 
                m_addr, m_compressed_size, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

class BPCLine : public BaseCompressedLine
{
    public:
        int m_base_size_in_bit;
        int m_planes_size_in_bit;

        BPCLine(Line* line) : BaseCompressedLine(line, -1)
        {
            // split the line data into 4 byte segments
            int num_segs = line->m_size / 4;
            u_int32_t * segs = new u_int32_t[num_segs];
            memcpy(segs, line->m_segs, line->m_size);

            // the first segment is the base for subtraction
            int num_deltas = num_segs - 1;
            u_int64_t * deltas = new u_int64_t[num_deltas];// each delta is actually 33 bit
            memset(deltas, 0, num_deltas * sizeof(u_int64_t));
            u_int32_t base = segs[0];
            // make the rest of the segments the deltas
            for (int i = 1; i < num_deltas; i++) {
                u_int64_t d = ((u_int64_t)segs[i] - (u_int64_t)base);
                deltas[i] = (u_int64_t) d;
            }
            delete [] segs;

            // transpose the segments to be 33 u_int16_t (15 bit) DBP planes
            int num_DBP = (line->m_size / 2)+1; // 33
            u_int16_t * DBP = new u_int16_t[num_DBP];
            memset(DBP, 0, num_DBP * sizeof(u_int16_t));
            // for plane i, take ith bit from all deltas
            for (int i=0; i < num_DBP; i++) { //0...32
                for (int j=0; j < num_deltas; j++) {
                    DBP[i] = DBP[i] | (((deltas[j] >> i) & 1) << j);
                }
            }
            delete [] deltas;

            int num_DBX = num_DBP - 1; //32
            u_int16_t * DBX = new u_int16_t[num_DBX];
            // xor with neighboring DBP planes, the last plane is untouched
            for (int i = 0; i < num_DBX; i++) {// 0..31
                DBX[i] = DBP[i] ^ DBP[i+1];
            }
            // DBX 0..31, need to also consider DBP[32]

            int num_bits_base = base_orig_encoder(base);
            m_base_size_in_bit = num_bits_base;
            int num_bits_planes = bp_encoder(DBX, DBP, num_DBP);//33
            m_planes_size_in_bit = num_bits_planes;

            int total_bytes = int(ceil(((double)num_bits_base + (double)num_bits_planes) / 8.0));
            // round up to a multiple of 8
            total_bytes = (total_bytes + 7) & ~7;

            delete [] DBP;
            delete [] DBX;

            if (total_bytes > line->m_size) {
                total_bytes = line->m_size;
            }

            m_compressed_size = total_bytes;
        }
        ~BPCLine()
        {
        }

        // compress the 15 bit planes alltogether (original paper uses 31-bit planes)
        int bp_encoder(u_int16_t * DBX, u_int16_t * DBP, int num_DBP) {
            int num_bits = 0;
            int length_single_zero_plane = 3;
            int length_single_multiple_plane = 7;
            // printf("num_DBP: %d\n", num_DBP);

            // first pass, compress all the zero planes
            vector<int> idx_zero_planes = {};
            for (int i = 0; i < num_DBP; i++) { //0..32
                u_int16_t plane_under_test;
                if (i == num_DBP-1) { // the last one is the DBP[32]
                    plane_under_test = DBP[i];
                } else {
                    plane_under_test = DBX[i];
                }

                if (plane_under_test == 0) {
                    // add this index
                    idx_zero_planes.push_back(i);
                }
            }
            // printf("idx_zero_planes: ");
            // for (unsigned i = 0; i < idx_zero_planes.size(); i++) {
            //     printf("%d ", idx_zero_planes[i]);
            // }
            // printf("\n");
            // figure out how many runs (consecutive index) are there
            int prev_idx = -1;
            vector<int> length_of_runs = {};
            int cur_run_idx = -1;
            for (unsigned i = 0; i < idx_zero_planes.size(); i++) {
                int cur_idx = idx_zero_planes[i];
                if (prev_idx == -1) {
                    // this is the first zero
                    cur_run_idx = 0; // first run
                    prev_idx = cur_idx;
                    length_of_runs.push_back(1);
                } else if (prev_idx + 1 == cur_idx) {
                    // this is a consecutive zero
                    length_of_runs[cur_run_idx] += 1;
                    prev_idx = cur_idx;
                } else {
                    // this is a new run
                    cur_run_idx += 1;
                    prev_idx = cur_idx;
                    length_of_runs.push_back(1);
                }
            }
            // printf("length of runs: ");
            // for (unsigned i = 0; i < length_of_runs.size(); i++) {
            //     printf("%d ", length_of_runs[i]);
            // }
            // printf("\n");
            int total_ct_zero = 0;
            for (unsigned i = 0; i < length_of_runs.size(); i++) {
                total_ct_zero += length_of_runs[i];
                if (length_of_runs[i] == 1) {
                    num_bits += length_single_zero_plane;
                } else {
                    assert (length_of_runs[i] <= num_DBP);
                    num_bits += length_single_multiple_plane;
                }
            }
            assert(total_ct_zero == (int)idx_zero_planes.size());

            // second path, compress the non-zero planes
            for (int i = 0; i < num_DBP; i++) {
                u_int16_t plane_under_test;
                if (i == num_DBP-1) { // the last one is the DBP[32]
                    plane_under_test = DBP[i];
                } else {
                    plane_under_test = DBX[i];
                }

                u_int8_t * plane_bytes = reinterpret_cast<u_int8_t*>(&plane_under_test);
                int data_size = 2; // 2byte


                if (plane_under_test == 0) continue; // already counted
                else if (plane_under_test == 0x7f) num_bits+=5; // all 1
                else if (DBP[i] == 0) num_bits+=5;
                else if (plane_under_test == 0x3 || plane_under_test == 0x6 || plane_under_test == 0xc || plane_under_test == 0x18 || 
                    plane_under_test == 0x30 || plane_under_test == 0x60 || plane_under_test == 0xc0 || plane_under_test == 0x180 || 
                    plane_under_test == 0x300 || plane_under_test == 0x600 || plane_under_test == 0xc00 || plane_under_test == 0x1800 || 
                    plane_under_test == 0x3000 || plane_under_test == 0x6000) num_bits+=10;
                else if (countSetBits(plane_bytes, data_size) == 1) num_bits += 10;
                else num_bits += 16;
            }
            return num_bits;
        }

        // compress the 32 bit base
        int base_orig_encoder(u_int32_t base) {
            int32_t base_signed = (int32_t) base;
            if (base == 0) return 3;
            if (base_signed <= 7 && base_signed >= -8) return 7;
            if (base_signed <= 127 && base_signed >= -128) return 11;
            if (base_signed <= 32767 && base_signed >= -32768) return 19;
            return 33;
        }

        void print()
        {
            printf("[ BPC compressed line addr: %16lx, physical size: %d (%d+%d bits), set: %d, bank: %d", 
                m_addr, m_compressed_size, m_base_size_in_bit, m_planes_size_in_bit, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

// class for a line that is compressed using BDI algorithm, which contains a base and an array of deltas/immediates
class BDILine : public BaseCompressedLine
{
    public:
        int m_base_size;
        int m_delta_size;
        int m_num_deltas;
        vector<bool> m_is_immediate;
        int8_t * m_base;
        int8_t * m_d_i;
        enum { BDI, IMM, B, Z, RAW } m_type;
        BDILine(
            Line* line, 
            int base_size, 
            int delta_size, 
            vector<bool> is_immediate, 
            int8_t * base, 
            int8_t * d_i) : 
            BaseCompressedLine(line, base_size + (delta_size+1) * (line->m_size / base_size))
        {
            m_type = BDI; // constructor with bdi complete

            m_base_size = base_size;
            m_delta_size = delta_size;
            m_num_deltas = line->m_size / base_size;
            m_is_immediate = is_immediate;

            m_base = new int8_t[base_size]();
            memcpy(m_base, base, base_size);

            m_d_i = new int8_t[m_num_deltas * delta_size]();
            memcpy(m_d_i, d_i, m_num_deltas * delta_size);
        }

        BDILine(
            Line* line, 
            int base_size, 
            int delta_size, 
            int8_t * d_i) : 
            BaseCompressedLine(line, delta_size * (line->m_size / base_size))
        {
            m_type = IMM; // constructor without base

            m_base_size = base_size;
            m_delta_size = delta_size;
            m_num_deltas = line->m_size / base_size;
            m_is_immediate = vector<bool>(m_num_deltas, true);

            // m_base = new int8_t[base_size];
            // memset(m_base, 0, base_size);

            m_d_i = new int8_t[m_num_deltas * delta_size]();
            memcpy(m_d_i, d_i, m_num_deltas * delta_size);
        }

        BDILine(
            Line* line, 
            int base_size, 
            u_int8_t * base) : 
            BaseCompressedLine(line, base_size)
        {
            m_type = B; // constructor with base only

            m_base_size = base_size;
            m_delta_size = 0;
            m_num_deltas = 0;
            m_is_immediate = vector<bool>(0, false);

            m_base = new int8_t[base_size]();
            memcpy(m_base, base, base_size);

            // m_d_i = new int8_t[1];
            // memset(m_d_i, 0, 1);
        }

        BDILine(
            Line* line,
            bool is_zero) : 
            BaseCompressedLine(line, is_zero ? 1 : line->m_size)
        {
            if (is_zero) {
                m_type = Z; // constructor with no base or deltas

                m_base_size = 0;
                m_delta_size = 0;
                m_num_deltas = 0;
                m_is_immediate = vector<bool>(0, false);

                // m_base = new int8_t[1];
                // memset(m_base, 0, 1);

                // m_d_i = new int8_t[1];
                // memset(m_d_i, 0, 1);
            } else {
                m_type = RAW; // constructor with no compression
                
                m_base_size = 0;
                m_delta_size = 0;
                m_num_deltas = 0;
                m_is_immediate = vector<bool>(0, false);

                // m_base = new int8_t[1];
                // memset(m_base, 0, 1);

                // m_d_i = new int8_t[1];
                // memset(m_d_i, 0, 1);
            }
        }

        ~BDILine()
        {
            if (m_type == BDI || m_type == B) {
                delete[] m_base;
            }
            if (m_type == BDI || m_type == IMM) {
                delete[] m_d_i;
            }
        }

        void print()
        {
            if (m_type == RAW) {
                Line::print();
            } else {
                printf("[ BDI compressed line type: %d ,addr: %16lx, physical size: %d, set: %d, bank: %d, base size: %d, delta size: %d, num deltas: %d", 
                    m_type, m_addr, get_compressed_size(), m_set, m_bank, m_base_size, m_delta_size, m_num_deltas);
                
                if (m_type == BDI || m_type == B) {
                    if (m_type == BDI) {
                        printf(", base: ");
                    } else {
                        printf(", rep: ");
                    }
                    for (int i = 0; i < m_base_size; i++) {
                        printf("%02x ", (u_int8_t)m_base[i]);
                    }
                    // // print as int64_t
                    // int64_t * base_int = reinterpret_cast<int64_t*>(m_base);
                    // int64_t base_decimal = base_int[0];
                    // printf("(%ld)", base_decimal);
                }

                if (m_type == BDI) {
                    printf(", deltas: ");
                    for (int i = 0; i < m_num_deltas; i++) {
                        if (m_is_immediate[i]) {
                            for (int j = 0; j < m_delta_size; j++) {
                                printf("xx ");
                            }
                        } else {
                            for (int j = 0; j < m_delta_size; j++) {
                                printf("%02x ", (u_int8_t)m_d_i[i * m_delta_size + j]);
                            }
                        }
                        printf(", ");
                    }
                }

                if (m_type == IMM || m_type == BDI) {
                    printf(", immediate: ");
                    for (int i = 0; i < m_num_deltas; i++) {
                        if (m_is_immediate[i]) {
                            for (int j = 0; j < m_delta_size; j++) {
                                printf("%02x ", (u_int8_t)m_d_i[i * m_delta_size + j]);
                            }
                        } else {
                            for (int j = 0; j < m_delta_size; j++) {
                                printf("xx ");
                            }
                        }
                        printf(", ");
                    }
                }
                
                printf(" ]\n");
            }
        }
};

class ThesaurusLine : public BaseCompressedLine
{
    public:
        bool m_zeroline;
        bool m_uncompressed;
        bool m_base_only;
        bool m_zero_plus_diff;
        vector<int> m_byte_diff_map;
        vector<u_int8_t> m_diff_bytes;
        int m_num_diff_bytes;
        Line* m_centroid_ptr;
        ThesaurusLine(Line* line,
            Line* centroid_nonzero,
            bool is_zeroline, bool is_zerocentroid) : 
            BaseCompressedLine(line, -1)
        {
            if (is_zeroline) {
                m_zeroline = true;
                m_zero_plus_diff = false;
                m_uncompressed = false;
                m_num_diff_bytes = -1;
                m_base_only = false;
                m_centroid_ptr = NULL;
                m_compressed_size = 0;
                return;
            }
            m_zeroline = false; // has its own constructor
            int true_size = -1; // update m_compressed_size at the end!
            if (is_zerocentroid) {
                m_base_only = false;
                m_num_diff_bytes = 0;
                for (int i=0; i < line->m_size; i++) {
                    if (line->m_segs[i] != 0) {
                        m_byte_diff_map.push_back(1);
                        m_diff_bytes.push_back(line->m_segs[i]);
                        m_num_diff_bytes++;
                    } else {
                        m_byte_diff_map.push_back(0);
                    }
                }
                assert(m_num_diff_bytes > 0);
                true_size = m_num_diff_bytes + int(line->m_size / 8);
                if (true_size >= line->m_size) {
                    true_size = line->m_size;
                    m_uncompressed = true;
                    m_centroid_ptr = NULL;
                    m_zero_plus_diff = false;
                } else {
                    m_uncompressed = false;
                    m_centroid_ptr = centroid_nonzero;
                    m_zero_plus_diff = true;
                }
            } else {
                m_zero_plus_diff = false;
                m_num_diff_bytes = 0;
                for (int i=0; i < line->m_size; i++) {
                    if (line->m_segs[i] != centroid_nonzero->m_segs[i]) {
                        m_byte_diff_map.push_back(1);
                        m_diff_bytes.push_back(line->m_segs[i]);
                        m_num_diff_bytes++;
                    } else {
                        m_byte_diff_map.push_back(0);
                    }
                }
                if (m_num_diff_bytes == 0) {
                    m_base_only = true;
                    true_size = 0;
                    m_uncompressed = false;
                    m_centroid_ptr = centroid_nonzero;
                    m_byte_diff_map.clear();
                    m_diff_bytes.clear();
                } else {
                    m_base_only = false;
                    true_size = m_num_diff_bytes + int(line->m_size / 8);
                    if (true_size >= line->m_size) {
                        true_size = line->m_size;
                        m_uncompressed = true;
                        m_centroid_ptr = NULL;
                    } else {
                        m_uncompressed = false;
                        m_centroid_ptr = centroid_nonzero;
                    }
                }

            }
            
            m_compressed_size = true_size;
        }
        
        ThesaurusLine(Line* zeroline) : 
            BaseCompressedLine(zeroline, 0)
        {
            m_centroid_ptr = NULL;
            m_zero_plus_diff = false;
            m_zeroline = true;
            m_uncompressed = false;
            m_num_diff_bytes = -1;
            m_base_only = false;
        }

        ~ThesaurusLine()
        {
            m_byte_diff_map.clear();
            m_diff_bytes.clear();
        }

        void print()
        {
            printf("[ Thesaurus compressed line: zero: %d, baseonly: %d, 0+diff: %d, compressed: %d, addr: %16lx, physical size: %d, set: %d, bank: %d\n", 
                m_zeroline, m_base_only, m_zero_plus_diff,m_uncompressed, m_addr, get_compressed_size(), m_set, m_bank);
            printf(", num diff bytes: %d, diff byte map: ( ", m_num_diff_bytes);
            for (unsigned i = 0; i < m_byte_diff_map.size(); i++) {
                printf("%d ", m_byte_diff_map[i]);
            }
            printf(")\n");

            if (m_centroid_ptr != NULL) {
                printf(", centroid line addr: %16lx", m_centroid_ptr->m_addr);
            }

            printf(" ]\n");

        }
};

class SparseByteLine : public BaseCompressedLine
{
    public:
    SparseByteLine(Line* line): BaseCompressedLine(line, -1)
    {
        int true_size = -1;
        int num_diff_bytes = 0;
        for (int i = 0; i < line->m_size; i++) {
            if (line->m_segs[i] != 0) {
                num_diff_bytes++;
            }
        }
        true_size = num_diff_bytes + int(line->m_size / 8);
        if (true_size >= line->m_size) {
            true_size = line->m_size;
        }
        m_compressed_size = true_size;
    }
    ~SparseByteLine()
    {
    }
    void print()
    {
        printf("[ SparseByte compressed line addr: %16lx, physical size: %d, set: %d, bank: %d", 
            m_addr, m_compressed_size, m_set, m_bank);
        // do not print the data for now
        printf(" ]\n");
    }
};

#endif // CACHE_COMPRESSEDLINE_HH