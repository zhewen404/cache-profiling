#ifndef CACHE_ENCODEDLINE_HH
#define CACHE_ENCODEDLINE_HH
#include "cache/line.hh"
#include "common/bit/bitvec.hh"
#include <stdio.h>
#include <vector>
#include <cstring>
#include <cmath>
#include <assert.h>

using namespace std;

class BaseEncodedLine : public Line
{
    public:
        int m_code_length_in_bit;
        double m_code_efficiency;
        
        BaseEncodedLine(Line* line, int code_length_in_bit) : 
            Line(line->m_size, line->m_set, line->m_bank, line->m_addr, line->m_segs)
        {
            m_code_length_in_bit = code_length_in_bit;
            m_code_efficiency = (double)line->m_size * 8 / code_length_in_bit;
        }

        int get_code_length() const
        {
            return m_code_length_in_bit;
        }

        int get_code_efficiency() const
        {
            return m_code_efficiency;
        }

        void print()
        {
            printf("[ base encoded line addr: %16lx, code length in bits: %d, code efficiency: %lf,  set: %d, bank: %d", 
                m_addr, m_code_length_in_bit, m_code_efficiency, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

class BaseSameLengthEncodedLine : public BaseEncodedLine
{
    public:
        int m_num_aux_bits;
        u_int8_t * m_encoded_segs; // same length as the original line
        u_int8_t * m_aux;
        int m_encoded_seg_size; // in byte
        
        BaseSameLengthEncodedLine(Line* line, int num_aux_bits) : 
            BaseEncodedLine(line, num_aux_bits+line->m_size*8)
        {
            m_num_aux_bits = num_aux_bits;
            int num_aux_bytes = (num_aux_bits+7) / 8;
            m_encoded_seg_size = line->m_size; // same size as the original line
            m_encoded_segs = new u_int8_t[m_encoded_seg_size]();
            // clear the encoded segments
            memset(m_encoded_segs, 0, m_encoded_seg_size);
            m_aux = new u_int8_t[num_aux_bytes]();
            // clear the aux bits
            memset(m_aux, 0, num_aux_bytes);
        }

        ~BaseSameLengthEncodedLine()
        {
            delete[] m_encoded_segs;
            delete[] m_aux;
        }

        int get_num_aux_bits() const
        {
            return m_num_aux_bits;
        }

        void print()
        {
            printf("[ base encoded line addr: %16lx, num aux bits: %d, set: %d, bank: %d", 
                m_addr, m_num_aux_bits, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

// https://ieeexplore.ieee.org/document/4801969
class DBIEncodedLine : public BaseSameLengthEncodedLine
{
    public:
        int m_granularity_in_bit;

        DBIEncodedLine(Line* line, int granularity_in_bit) : BaseSameLengthEncodedLine(line, line->m_size*8/granularity_in_bit)
        {
            m_granularity_in_bit = granularity_in_bit;
            int granularity_in_byte = granularity_in_bit / 8;
            assert (granularity_in_byte >= 1);
            int num_segments = line->m_size / granularity_in_byte;

            vector<bool> aux_bits(num_segments, false);
            for (int i = 0; i < num_segments; i++) {
                int num_1s_in_this_segment = countSetBits(&line->m_segs[i * granularity_in_byte], granularity_in_byte);
                if (num_1s_in_this_segment == granularity_in_bit/2) {
                    aux_bits[i] = false; // should not flip the bits
                } else if (num_1s_in_this_segment > granularity_in_bit/2) { // more 1s than 0s
                    aux_bits[i] = true; // should flip the bits
                    m_aux[i/8] |= 1 << (i%8);
                } else { // more 0s than 1s
                    aux_bits[i] = false;
                }
            }

            for (int i = 0; i < num_segments; i++) {
                for (int j = 0; j < granularity_in_byte; j++) {
                    if (aux_bits[i]) {
                        m_encoded_segs[i * granularity_in_byte + j] = ~line->m_segs[i * granularity_in_byte + j];
                    } else {
                        m_encoded_segs[i * granularity_in_byte + j] = line->m_segs[i * granularity_in_byte + j];
                    }
                }
            }

            aux_bits.clear();


        }
        ~DBIEncodedLine()
        {
        }

        void print()
        {
            printf("[ DBI encoded line addr: %16lx, granularity in bits: %d, num aux bits: %d, set: %d, bank: %d", 
                m_addr, m_granularity_in_bit, m_num_aux_bits, m_set, m_bank);
            // do not print the data for now
            printf(" ]\n");
        }
};

#endif // CACHE_COMPRESSEDLINE_HH