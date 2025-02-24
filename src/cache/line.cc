#include "cache/line.hh"
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <assert.h>
using namespace std;

void 
Line::print() const
{
    printf("[ line addr: %16lx, size: %d, set: %d, bank: %d", m_addr, m_size, m_set, m_bank);
    if (m_alloc) {
        printf(", data: ");
        for (int i = 0; i < m_size; i++) {
            printf("%02x ", m_segs[i]);
        }
    }
    printf(" ]\n");
}

void 
Line::copyData(u_int8_t * data) 
{
    m_segs = new u_int8_t[m_size];
    // copy(data, data + m_size, m_segs);
    // m_segs = (u_int8_t *) malloc(m_size * sizeof(u_int8_t));
    memcpy(m_segs, data, m_size);
    m_alloc = true;
}

vector<u_int8_t>
Line::get_byte_arrary() const
{
    vector<u_int8_t> byte_array;
    for (int i = 0; i < m_size; i++) {
        byte_array.push_back(m_segs[i]);
    }
    return byte_array;
}

// this function calculates the number of non-zero bits in the line
int 
Line::get_bit_hamming_weight() const
{
    int weight = 0;
    for (int i = 0; i < m_size; i++) {
        u_int8_t byte = m_segs[i];
        for (int j = 0; j < 8; j++) {
            if (byte & (1 << j)) {
                weight += 1;
            }
        }
    }
    assert(weight >= 0);
    assert(weight <= m_size * 8);
    return weight;
}

// this function calculates the number of non-zero bytes in the line
int
Line::get_byte_hamming_weight() const
{
    int weight = 0;
    for (int i = 0; i < m_size; i++) {
        if (m_segs[i] != 0) {
            weight += 1;
        }
    }
    assert(weight >= 0);
    assert(weight <= m_size);
    return weight;
}