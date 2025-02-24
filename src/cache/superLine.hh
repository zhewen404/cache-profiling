#ifndef CACHE_SUPERLINE_HH
#define CACHE_SUPERLINE_HH
#include "cache/line.hh"
#include <vector>
#include <stdio.h>
#include <assert.h>
#include <cstring>

using namespace std;

class BaseSuperLine : public Line
{
    public:
        vector<Line*> m_line_ptrs;
        int m_line_cluster_size;

        // BaseCompressedLine should be able to be constructed from a Line
        BaseSuperLine(vector<Line*> lines) : 
            // Line(lines[0]->m_size, lines[0]->m_set, lines[0]->m_bank, 0, nullptr)
            Line(0, 0, 0, 0, nullptr)
        {
            m_line_ptrs = lines;
            m_line_cluster_size = lines.size();
            assert(m_line_cluster_size > 0);
            m_size = lines[0]->m_size;
            m_set = lines[0]->m_set;
            m_bank = lines[0]->m_bank;
            // if they agree on size, set and bank we can fill that in
            // otherwise we simply leave it as -1
            for (int i = 0; i < m_line_cluster_size; i++) {
                if (lines[i]->m_size != m_size) {
                    m_size = -1;
                }
                if (lines[i]->m_set != m_set) {
                    m_set = -1;
                }
                if (lines[i]->m_bank != m_bank) {
                    m_bank = -1;
                }
            }
        }
        ~BaseSuperLine()
        {
        }


        void print() const
        {
            printf("[ base supper line of cluster of %d, size: %d, bank: %d, set: %d", 
                m_line_cluster_size, m_size, m_bank, m_set);
            printf(" ]\n");
        }
};


class XORedLine : public BaseSuperLine
{
    public:
        XORedLine(vector<Line*> lines) : BaseSuperLine(lines)
        {
            assert(lines.size() <= 2);
            xor_lines(lines);
        }
        ~XORedLine()
        {
        }
        void print() const
        {
            printf("[ XORed line of %d lines, size: %d", 
                m_line_cluster_size, m_size);
            
            if (m_bank != -1) {
                printf(", bank: %d", m_bank);
            } else {
                printf(", bank: x");
            }

            if (m_set != -1) {
                printf(", set: %d", m_set);
            } else {
                printf(", set: x");
            }

            printf(", hamming weight b: %d, hamming weight B: %d", get_byte_hamming_weight(), get_bit_hamming_weight());

            printf(", segs: ");

            for (int i = 0; i < m_size; i++) {
                printf(" %02x ", m_segs[i]);
            }

            printf(" ]\n");
        }
        void xor_lines(vector<Line*> lines) {
            if (lines.size() == 1) {
                m_alloc = true;
                m_segs = new u_int8_t[m_size];
                memcpy(m_segs, lines[0]->m_segs, m_size);
            } else if (lines.size() == 2) {
                m_alloc = true;
                m_segs = new u_int8_t[m_size];
                memcpy(m_segs, lines[0]->m_segs, m_size);
                for (int i = 0; i < m_size; i++) {
                    m_segs[i] ^= lines[1]->m_segs[i];
                }
            }
        }
};


class DeltaLine : public BaseSuperLine
{
    public:
        DeltaLine(vector<Line*> lines) : BaseSuperLine(lines)
        {
            assert(lines.size() <= 2);
            delta_lines(lines);
        }
        ~DeltaLine()
        {
        }
        void print() const
        {
            printf("[ Delta line of %d lines, size: %d", 
                m_line_cluster_size, m_size);
            
            if (m_bank != -1) {
                printf(", bank: %d", m_bank);
            } else {
                printf(", bank: x");
            }

            if (m_set != -1) {
                printf(", set: %d", m_set);
            } else {
                printf(", set: x");
            }

            printf(" ]\n");
        }
        void delta_lines(vector<Line*> lines) {
            if (lines.size() == 1) {
                m_alloc = true;
                m_segs = new u_int8_t[m_size];
                memcpy(m_segs, lines[0]->m_segs, m_size);
            } else if (lines.size() == 2) {
                m_alloc = true;
                m_segs = new u_int8_t[m_size];
                memcpy(m_segs, lines[0]->m_segs, m_size);
                for (int i = 0; i < m_size; i++) {
                    int8_t diff = (int8_t) m_segs[i] - (int8_t)lines[1]->m_segs[i];
                    m_segs[i] = (u_int8_t) diff;
                }
            }
        }
};

#endif // CACHE_SUPERLINE_HH

