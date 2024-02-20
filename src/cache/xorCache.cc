#include "cache/xorCache.hh"

#include "common/bit/bitvec.hh"
#include "common/vector/vector.hh"

double
RandBankXORCache::get_bit_entropy() const
{
    unsigned num1s = 0;
    unsigned num0s = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            unsigned num1 = countSetBits(m_lines[i][j]->m_segs, m_lines[i][j]->m_size);
            unsigned num0 = m_lines[i][j]->m_size * 8 - num1;
            num1s += num1;
            num0s += num0;
        }
    }
    vector<int> vec = {(int)num1s, (int)num0s};
    double entropy = calculateEntropy(vec);
    return entropy;
}

int 
RandBankXORCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            size += m_lines[i][j]->m_size;
        }
    }
    return size;
}
int
RandBankXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
RandBankXORCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}

void 
RandBankXORCache::print() const
{
    printf("RandBankXORCache [ bit_entropy: %f, inter_cr: %f ]\n", 
     get_bit_entropy(), get_compression_ratio());
    for (unsigned i = 0; i < m_lines.size(); i++) {
        unsigned size = m_lines[i].size();
        for (unsigned j = 0; j < size; j++) {
            m_lines[i][j]->print();
        }
    }
}



double
RandSetXORCache::get_bit_entropy() const
{
    unsigned num1s = 0;
    unsigned num0s = 0;
    for (int i = 0; i < m_num_sets; i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            unsigned num1 = countSetBits(m_lines[i][j]->m_segs, m_lines[i][j]->m_size);
            unsigned num0 = m_lines[i][j]->m_size * 8 - num1;
            num1s += num1;
            num0s += num0;
        }
    }
    vector<int> vec = {(int)num1s, (int)num0s};
    double entropy = calculateEntropy(vec);
    return entropy;
}

int 
RandSetXORCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            size += m_lines[i][j]->m_size;
        }
    }
    return size;
}
int
RandSetXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
RandSetXORCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}

void 
RandSetXORCache::print() const
{
    printf("RandSetXORCache [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());
    for (unsigned i = 0; i < m_lines.size(); i++) {
        unsigned size = m_lines[i].size();
        for (unsigned j = 0; j < size; j++) {
            m_lines[i][j]->print();
        }
    }
}


double
HashXORCache::get_bit_entropy() const
{
    unsigned num1s = 0;
    unsigned num0s = 0;

    for (unsigned i=0; i < m_lines.size(); i++) {
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        unsigned num0 = m_lines[i]->m_size * 8 - num1;
        num1s += num1;
        num0s += num0;
    }
    vector<int> vec = {(int)num1s, (int)num0s};
    double entropy = calculateEntropy(vec);
    return entropy;
}

int 
HashXORCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        size += m_lines[i]->m_size;
    }
    return size;
}
int
HashXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
HashXORCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}
double
HashXORCache::get_false_positive_rate() const
{
    return (double) m_num_false_positive / (double) m_num_xored_lines;
}

void 
HashXORCache::print() const
{
    printf("HashXORCache [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());
    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}

double
HashDeltaCache::get_bit_entropy() const
{
    unsigned num1s = 0;
    unsigned num0s = 0;

    for (unsigned i=0; i < m_lines.size(); i++) {
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        unsigned num0 = m_lines[i]->m_size * 8 - num1;
        num1s += num1;
        num0s += num0;
    }
    vector<int> vec = {(int)num1s, (int)num0s};
    double entropy = calculateEntropy(vec);
    return entropy;
}

int 
HashDeltaCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        size += m_lines[i]->m_size;
    }
    return size;
}
int
HashDeltaCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
HashDeltaCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}

double
HashDeltaCache::get_false_positive_rate() const
{
    return (double) m_num_false_positive / (double) m_num_delta_lines;
}


void 
HashDeltaCache::print() const
{
    printf("HashDeltaCache [ num_delta_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_delta_lines, get_bit_entropy(), get_compression_ratio());
    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}
