#ifndef CACHE_COMPRESSEDLINE_HH
#define CACHE_COMPRESSEDLINE_HH
#include "cache/line.hh"
#include <stdio.h>
#include <vector>
#include <cstring>

using namespace std;

class BaseCompressedLine : public Line
{
    int m_compressed_size;
    public:
        // BaseCompressedLine should be able to be constructed from a Line
        BaseCompressedLine(Line* line, int compressed_size) : Line(line->m_size, line->m_set, line->m_bank, line->m_addr, line->m_segs)
        {
            m_compressed_size = compressed_size;
        }
        ~BaseCompressedLine()
        {
        }

        int get_compressed_size() const
        {
            return m_compressed_size;
        }

        void print()
        {
            printf("[ base compressed line addr: %16lx, physical size: %d, set: %d, bank: %d, data: ", 
                m_addr, m_compressed_size, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

// class for a line that is compressed using BDI algorithm, which contains a base and an array of deltas/immediates
class BDILine : public BaseCompressedLine
{
    int m_base_size;
    int m_delta_size;
    int m_num_deltas;
    vector<bool> m_is_immediate;
    int8_t * m_base;
    int8_t * m_d_i;
    enum { BDI, IMM, B, Z, RAW } m_type;

    public:
        BDILine(
            Line* line, 
            int base_size, 
            int delta_size, 
            vector<bool> is_immediate, 
            int8_t * base, 
            int8_t * d_i) : 
            BaseCompressedLine(line, base_size + delta_size * (line->m_size / base_size))
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



#endif // CACHE_COMPRESSEDLINE_HH