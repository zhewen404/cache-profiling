#include "cache/xorCache.hh"

#include "common/bit/bitvec.hh"
#include "common/vector/vector.hh"
#include <vector>
#include <unordered_map>

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

double
RandBankXORCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (int i = 0; i < m_num_banks; i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            if (m_lines[i][j]->m_line_cluster_size == 1) continue;
            unsigned num1 = countSetBits(m_lines[i][j]->m_segs, m_lines[i][j]->m_size);
            hamming_distance.push_back((int)num1);
        }
    }
    double mean = calculateMean(hamming_distance);
    return mean;
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

double
RandBankXORCache::get_false_positive_rate() const
{
    return (double) 0;
}

vector<double> * 
RandBankXORCache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            for (unsigned j=0; j < m_lines[i].size(); j++) { // for every line
                u_int8_t byte = m_lines[i][j]->m_segs[b];
                if (byte_count.find(byte) == byte_count.end()) {
                    byte_count[byte] = 1;
                } else {
                    byte_count[byte]++;
                }
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
RandBankXORCache::get_per_byte_entropy_only_thoses_xored() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            for (unsigned j=0; j < m_lines[i].size(); j++) { // for every line
                if (m_lines[i][j]->m_line_cluster_size == 1) continue;
                u_int8_t byte = m_lines[i][j]->m_segs[b];
                if (byte_count.find(byte) == byte_count.end()) {
                    byte_count[byte] = 1;
                } else {
                    byte_count[byte]++;
                }
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
RandBankXORCache::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            for (unsigned j=0; j < m_lines[i].size(); j++) { // for every line
                u_int8_t byte = m_lines[i][j]->m_segs[b];
                hamming_byte += byte;
            }
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
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

double
RandSetXORCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (int i = 0; i < m_num_sets; i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            if (m_lines[i][j]->m_line_cluster_size == 1) continue;
            unsigned num1 = countSetBits(m_lines[i][j]->m_segs, m_lines[i][j]->m_size);
            hamming_distance.push_back((int)num1);
        }
    }
    double mean = calculateMean(hamming_distance);
    return mean;
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

double
RandSetXORCache::get_false_positive_rate() const
{
    return (double) 0;
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

////////////////////////////////////////////////////////////////
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

double
HashXORCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (unsigned i=0; i < m_lines.size(); i++) {
        if (m_lines[i]->m_line_cluster_size == 1) continue;
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        hamming_distance.push_back((int)num1);
    }
    double mean = calculateMean(hamming_distance);
    return mean;
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

vector<double> * 
HashXORCache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}
vector<double> * 
HashXORCache::get_per_byte_entropy_only_thoses_xored() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            if (m_lines[i]->m_line_cluster_size == 1) continue;
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
HashXORCache::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            u_int8_t byte = m_lines[i]->m_segs[b];
            hamming_byte += byte;
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
}

void 
HashXORCache::print() const
{
    printf("HashXORCache [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());

    //print per byte entropy
    vector<double> * entropies = get_per_byte_entropy();
    printf("[ per byte entropy: ");
    for (unsigned i=0; i < entropies->size(); i++) {
        printf("%.2f ", entropies->at(i));
    }
    printf(" ]\n");
    entropies->clear();
    delete entropies;

    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}

////////////////////////////////////////////////////////////////
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

double
HashDeltaCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (unsigned i=0; i < m_lines.size(); i++) {
        if (m_lines[i]->m_line_cluster_size == 1) continue;
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        hamming_distance.push_back((int)num1);
    }
    double mean = calculateMean(hamming_distance);
    return mean;
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
vector<double> * 
HashDeltaCache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
HashDeltaCache::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            u_int8_t byte = m_lines[i]->m_segs[b];
            hamming_byte += byte;
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
}


void 
HashDeltaCache::print() const
{
    printf("HashDeltaCache [ num_delta_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_delta_lines, get_bit_entropy(), get_compression_ratio());

    //print per byte entropy
    vector<double> * entropies = get_per_byte_entropy();
    printf("[ per byte entropy: ");
    for (unsigned i=0; i < entropies->size(); i++) {
        printf("%.2f ", entropies->at(i));
    }
    printf(" ]\n");
    entropies->clear();
    delete entropies;

    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}

////////////////////////////////////////////////////////
double
IdealBankXORCache::get_bit_entropy() const
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

double
IdealBankXORCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (unsigned i=0; i < m_lines.size(); i++) {
        if (m_lines[i]->m_line_cluster_size == 1) continue;
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        hamming_distance.push_back((int)num1);
    }
    double mean = calculateMean(hamming_distance);
    return mean;
}

int 
IdealBankXORCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        size += m_lines[i]->m_size;
    }
    return size;
}
int
IdealBankXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
IdealBankXORCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}


vector<double> * 
IdealBankXORCache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}
vector<double> * 
IdealBankXORCache::get_per_byte_entropy_only_thoses_xored() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            if (m_lines[i]->m_line_cluster_size == 1) continue;
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
IdealBankXORCache::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            u_int8_t byte = m_lines[i]->m_segs[b];
            hamming_byte += byte;
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
}

void 
IdealBankXORCache::print() const
{
    printf("IdealBankXORCache [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());

    //print per byte entropy
    vector<double> * entropies = get_per_byte_entropy();
    printf("[ per byte entropy: ");
    for (unsigned i=0; i < entropies->size(); i++) {
        printf("%.2f ", entropies->at(i));
    }
    printf(" ]\n");
    entropies->clear();
    delete entropies;

    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}
////////////////////////////////////////////////////////
double
IdealBankXORCacheThesaurus::get_bit_entropy() const
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

double
IdealBankXORCacheThesaurus::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (unsigned i=0; i < m_lines.size(); i++) {
        if (m_lines[i]->m_line_cluster_size == 1) continue;
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        hamming_distance.push_back((int)num1);
    }
    double mean = calculateMean(hamming_distance);
    return mean;
}

int 
IdealBankXORCacheThesaurus::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        size += m_lines[i]->m_size;
    }
    return size;
}
int
IdealBankXORCacheThesaurus::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
IdealBankXORCacheThesaurus::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}


vector<double> * 
IdealBankXORCacheThesaurus::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}
vector<double> * 
IdealBankXORCacheThesaurus::get_per_byte_entropy_only_thoses_xored() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            if (m_lines[i]->m_line_cluster_size == 1) continue;
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
IdealBankXORCacheThesaurus::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            u_int8_t byte = m_lines[i]->m_segs[b];
            hamming_byte += byte;
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
}

void 
IdealBankXORCacheThesaurus::print() const
{
    printf("IdealBankXORCacheThesaurus [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());

    //print per byte entropy
    vector<double> * entropies = get_per_byte_entropy();
    printf("[ per byte entropy: ");
    for (unsigned i=0; i < entropies->size(); i++) {
        printf("%.2f ", entropies->at(i));
    }
    printf(" ]\n");
    entropies->clear();
    delete entropies;

    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}


////////////////////////////////////////////////////////
double
IdealSetXORCache::get_bit_entropy() const
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

double
IdealSetXORCache::get_hamming_distance() const
{
    vector<int> hamming_distance;

    for (unsigned i=0; i < m_lines.size(); i++) {
        if (m_lines[i]->m_line_cluster_size == 1) continue;
        unsigned num1 = countSetBits(m_lines[i]->m_segs, m_lines[i]->m_size);
        hamming_distance.push_back((int)num1);
    }
    double mean = calculateMean(hamming_distance);
    return mean;
}

int 
IdealSetXORCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        size += m_lines[i]->m_size;
    }
    return size;
}
int
IdealSetXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double 
IdealSetXORCache::get_compression_ratio() const
{
    return (double)get_uncompressed_size() / (double)get_compressed_size();
}


vector<double> * 
IdealSetXORCache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}
vector<double> * 
IdealSetXORCache::get_per_byte_entropy_only_thoses_xored() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_lines[0]->m_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) { // for every line
            if (m_lines[i]->m_line_cluster_size == 1) continue;
            u_int8_t byte = m_lines[i]->m_segs[b];
            if (byte_count.find(byte) == byte_count.end()) {
                byte_count[byte] = 1;
            } else {
                byte_count[byte]++;
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

vector<double> * 
IdealSetXORCache::get_per_byte_hamming() const
{
    vector<double> * hamming = new vector<double>();
    double hamming_byte = 0;
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (unsigned i=0; i < m_lines.size(); i++) {
            u_int8_t byte = m_lines[i]->m_segs[b];
            hamming_byte += byte;
        }
        if (hamming_byte == 0) hamming_byte = 0;
        else hamming_byte /= m_lines.size();
        hamming->push_back(hamming_byte);
    }
    return hamming;
}

void 
IdealSetXORCache::print() const
{
    printf("IdealSetXORCache [ num_xored_lines: %d, bit_entropy: %f, inter_cr: %f ]\n", 
        m_num_xored_lines, get_bit_entropy(), get_compression_ratio());

    //print per byte entropy
    vector<double> * entropies = get_per_byte_entropy();
    printf("[ per byte entropy: ");
    for (unsigned i=0; i < entropies->size(); i++) {
        printf("%.2f ", entropies->at(i));
    }
    printf(" ]\n");
    entropies->clear();
    delete entropies;

    for (unsigned i = 0; i < m_lines.size(); i++) {
        m_lines[i]->print();
    }
}
