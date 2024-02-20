#ifndef COMPRESSION_HASHFUNCTION_HH
#define COMPRESSION_HASHFUNCTION_HH
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <assert.h>
#include <set>
#include "common/bit/bitvec.hh"

class HashFunction
{
    public:
    HashFunction()
    {
        m_seed = 12345;
    }
    HashFunction(unsigned seed)
    {
        m_seed = seed;
    }

    virtual ~HashFunction() {
    }

    virtual u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) = 0;

    void set_seed(unsigned seed) {
        m_seed = seed;
    }


    void print() const {
        printf("HashFunction\n");
    }

    unsigned m_seed;
};

class FullBitShuffleHash : public HashFunction
{
    public:
    FullBitShuffleHash() : HashFunction() 
    {
    }

    FullBitShuffleHash(unsigned seed) : HashFunction(seed) 
    {
    }

    virtual ~FullBitShuffleHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        unsigned setbits = countSetBits(line_data, data_size_in_bit/8);
        u_int8_t * fingerprint = scramble_bit(line_data, data_size_in_bit, m_seed);
        assert (setbits == countSetBits(fingerprint, data_size_in_bit/8));
        fingerprint_size_in_bit = data_size_in_bit;
        return fingerprint;
    }

    void print() const {
        printf("FullBitShuffleHash\n");
    }
};

class ByteBitShuffleHash : public HashFunction
{
    public:
    ByteBitShuffleHash(bool use_same_seed_for_all_byte) : HashFunction() 
    {
        m_use_same_seed_for_all_bytes = use_same_seed_for_all_byte;
    }

    ByteBitShuffleHash(bool use_same_seed_for_all_byte, unsigned seed) : HashFunction(seed) 
    {
        m_use_same_seed_for_all_bytes = use_same_seed_for_all_byte;
    }

    virtual ~ByteBitShuffleHash() {
    }

    unsigned * gen_seeds_arr(int size)
    {
        unsigned * seeds_arr = new unsigned[size]();
        if (m_use_same_seed_for_all_bytes) {
            for (int i = 0; i < size; i++) {
                seeds_arr[i] = m_seed;
            }
        } else {
            for (int i = 0; i < size; i++) {
                seeds_arr[i] = m_seed + i;
            }
        }
        return seeds_arr;
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        unsigned * seeds_arr = gen_seeds_arr(data_size_in_bit / 8);
        u_int8_t * fingerprint = scramble_bit_within_byte(line_data, data_size_in_bit, seeds_arr);
        fingerprint_size_in_bit = data_size_in_bit;
        delete[] seeds_arr;
        return fingerprint;
    }

    void print() const {
        printf("ByteBitShuffleHash [ use_same_seed: %d ]\n", m_use_same_seed_for_all_bytes);
    }

    bool m_use_same_seed_for_all_bytes;
};

class XORFoldingHash : public HashFunction
{
    public:
    XORFoldingHash(int fingerprint_size_bit) : HashFunction() 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
    }

    virtual ~XORFoldingHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = xor_folding(line_data, data_size_in_bit, m_fingerprint_size_bit);
        fingerprint_size_in_bit = m_fingerprint_size_bit;
        return fingerprint;
    }

    void print() const {
        printf("XORFoldingHash [ fp_bits: %d ]\n", m_fingerprint_size_bit);
        // print hash scheme, i.e., which bits are xored
        print_xor_folding_equation(512, m_fingerprint_size_bit);
    }

    int m_fingerprint_size_bit;
};

class ThesaurusLSHash : public HashFunction
{
    public:
    ThesaurusLSHash(int fingerprint_size_bit, int data_size) : HashFunction() 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
        m_data_size = data_size;
        gen_lsh_matrix(data_size);
    }
    ThesaurusLSHash(int fingerprint_size_bit, int data_size, unsigned seed) : HashFunction(seed) 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
        m_data_size = data_size;
        gen_lsh_matrix(data_size);
    }

    ~ThesaurusLSHash() {
        // printf("ThesaurusLSHash destructor\n");
        for (int i=0; i < m_fingerprint_size_bit; i++) {
            m_lsh_matrix[i].clear();
        }
    }

    void gen_lsh_matrix(int data_size) {
        for (int i=0; i < m_fingerprint_size_bit; i++) {
            vector<int> row;
            for (int j=0; j < data_size; j++) {
                int random = rand() % 6;
                if (random == 0) {
                    row.push_back(1);
                } else if (random == 1) {
                    row.push_back(-1);
                } else {
                    row.push_back(0);
                }
            }
            m_lsh_matrix.push_back(row);
        }
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        // printf("thesaurus hash\n");
        assert(data_size_in_bit == m_data_size*8);
        fingerprint_size_in_bit = m_fingerprint_size_bit;
        int fp_num_byte = ceil(m_fingerprint_size_bit / 8.0);

        u_int8_t * fingerprint = new u_int8_t[fp_num_byte]();
        memset(fingerprint, 0, fp_num_byte);

        for (unsigned i=0; i < m_lsh_matrix.size(); i++) {
            vector<int> row = m_lsh_matrix[i];
            int sum = 0;
            for (int j=0; j < m_data_size; j++) {
                sum += m_lsh_matrix[i][j] * (unsigned int)line_data[j];
            }
            if (sum > 0) {
                fingerprint[i/8] |= (1 << (i%8));
                
                // print the fingerprint
                // printf("i=%d: ", i);
                // for (int i = 0; i < fp_num_byte; i++) {
                //     printf("%d ", fingerprint[i]);
                // }
                // printf("\n");
            }
        }
        return fingerprint;
    }

    void print() const {
        printf("ThesaurusLSHash [ fp_bits: %d, data_size_byte: %d ]\n", 
            m_fingerprint_size_bit, m_data_size);
    }

    int m_fingerprint_size_bit;
    int m_data_size;
    vector<vector<int>> m_lsh_matrix;
};

class BitSamplingLSHash : public HashFunction
{
    public:
    BitSamplingLSHash(int fingerprint_size_bit) : HashFunction() 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
    }
    BitSamplingLSHash(int fingerprint_size_bit, unsigned seed) : HashFunction(seed) 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
    }

    virtual ~BitSamplingLSHash() {
    }

    vector<unsigned> gen_rand_indices(int data_size_in_bit, int fingerprint_size) {
        vector<unsigned> rand_indices;
        set<unsigned> rand_indices_set;
        while (rand_indices.size() < (unsigned)fingerprint_size) {
            unsigned rand_index = rand() % data_size_in_bit;
            if (rand_indices_set.find(rand_index) == rand_indices_set.end()) { // dedup
                rand_indices_set.insert(rand_index);
                rand_indices.push_back(rand_index);
            }
        }
        rand_indices_set.clear();
        return rand_indices;
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        assert (data_size_in_bit >= m_fingerprint_size_bit);
        vector<unsigned> rand_indices = gen_rand_indices(data_size_in_bit, m_fingerprint_size_bit);

        // print the random indices
        // printf("rand_indices: ");
        // for (unsigned i = 0; i < rand_indices.size(); i++) {
        //     printf("%d ", rand_indices[i]);
        // }
        // printf("\n");

        u_int8_t * fingerprint = new u_int8_t[int(ceil(m_fingerprint_size_bit/8.0))]();
        memset(fingerprint, 0, int(ceil(m_fingerprint_size_bit/8.0)));
        fingerprint_size_in_bit = m_fingerprint_size_bit;

        for (unsigned i=0; i < rand_indices.size(); i++) {
            unsigned rand_index = rand_indices[i];
            unsigned byte_index = rand_index / 8;
            unsigned bit_index = rand_index % 8;
            unsigned bit = (line_data[byte_index] >> bit_index) & 1;
            fingerprint[i/8] |= (bit << (i%8));
        }

        return fingerprint;
    }

    void print() const {
        printf("BitSamplingLSHash [ fp_bits: %d ]\n", m_fingerprint_size_bit);
    }

    int m_fingerprint_size_bit;
};

class ByteMapHash : public HashFunction
{
    public:
    ByteMapHash() : HashFunction() 
    {
    }

    virtual ~ByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = byte_map(line_data, data_size_in_bit);
        fingerprint_size_in_bit = data_size_in_bit/8;
        return fingerprint;
    }

    void print() const {
        printf("ByteMapHash \n");
    }

};

class SparseByteMapHash : public HashFunction
{
    public:
    SparseByteMapHash(int everyNByte) : HashFunction() 
    {
        m_everyNByte = everyNByte;
    }

    virtual ~SparseByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = sparse_byte_map(line_data, data_size_in_bit, m_everyNByte);
        fingerprint_size_in_bit = data_size_in_bit/8/m_everyNByte;
        return fingerprint;
    }

    void print() const {
        printf("SparseByteMapHash [ every %d byte ]\n", m_everyNByte);
    }

    int m_everyNByte;

};


class NByteMapHash : public HashFunction
{
    public:
    NByteMapHash(int nByte) : HashFunction() 
    {
        m_nByte = nByte;
    }

    virtual ~NByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = nbyte_map(line_data, data_size_in_bit, m_nByte);
        fingerprint_size_in_bit = int(ceil(data_size_in_bit/(m_nByte*8.0)));
        return fingerprint;
    }

    void print() const {
        printf("NByteMapHash [ nByte: %d ]\n", m_nByte);
    }

    int m_nByte;

};

class TernaryByteMapHash : public HashFunction
{
    public:
    TernaryByteMapHash() : HashFunction() 
    {
    }

    virtual ~TernaryByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint_test_min = byte_map_min(line_data, data_size_in_bit);
        u_int8_t * fingerprint_test_max = byte_map_max(line_data, data_size_in_bit);

        //print these two
        // printf("fingerprint_test_min: ");
        // for (int i = 0; i < int(ceil(data_size_in_bit/64.0)); i++) {
        //     printf("%d ", fingerprint_test_min[i]);
        // }
        // printf("\n");
        // printf("fingerprint_test_max: ");
        // for (int i = 0; i < int(ceil(data_size_in_bit/64.0)); i++) {
        //     printf("%d ", fingerprint_test_max[i]);
        // }
        // printf("\n");

        u_int8_t * fingerprint = new u_int8_t[int(ceil(data_size_in_bit/4.0))]();
        memset(fingerprint, 0, int(ceil(data_size_in_bit/4.0)));
        for (int i = 0; i < int(ceil(data_size_in_bit/4.0)); i++) {
            if (i % 2 == 0) {
                // or ith bit with fingerprint_test_min at bit i/2
                int bit = (fingerprint_test_min[i/16] >> (i/2)%8) & 1;
                // printf("bit from min: %d\n", bit);
                fingerprint[i/8] |= (bit << (i%8));
            } else {
                int bit = (fingerprint_test_max[i/16] >> (i/2)%8) & 1;
                // printf("bit from max: %d\n", bit);
                fingerprint[i/8] |= (bit << (i%8));
            }
        }
        delete[] fingerprint_test_min;
        delete[] fingerprint_test_max;
        fingerprint_size_in_bit = data_size_in_bit/4;
        return fingerprint;
    }

    void print() const {
        printf("TernaryByteMapHash [2 bit (ternary) per byte ]\n");
    }

};


class ShuffledTernaryByteMapHash : public HashFunction
{
    public:
    ShuffledTernaryByteMapHash() : HashFunction() 
    {
    }
    ShuffledTernaryByteMapHash(unsigned seed) : HashFunction(seed) 
    {
    }

    virtual ~ShuffledTernaryByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint_test_min = byte_map_min(line_data, data_size_in_bit);
        u_int8_t * fingerprint_test_max = byte_map_max(line_data, data_size_in_bit);

        u_int8_t * fingerprint_test_min_shuffled = scramble_bit(fingerprint_test_min, 
            int(ceil(data_size_in_bit/8.0)), m_seed);
        u_int8_t * fingerprint_test_max_shuffled = scramble_bit(fingerprint_test_max, 
            int(ceil(data_size_in_bit/8.0)), m_seed);

        delete[] fingerprint_test_min;
        delete[] fingerprint_test_max;

        //print these two
        // printf("fingerprint_test_min: ");
        // for (int i = 0; i < int(ceil(data_size_in_bit/64.0)); i++) {
        //     printf("%d ", fingerprint_test_min[i]);
        // }
        // printf("\n");
        // printf("fingerprint_test_max: ");
        // for (int i = 0; i < int(ceil(data_size_in_bit/64.0)); i++) {
        //     printf("%d ", fingerprint_test_max[i]);
        // }
        // printf("\n");

        u_int8_t * fingerprint = new u_int8_t[int(ceil(data_size_in_bit/4.0))]();
        memset(fingerprint, 0, int(ceil(data_size_in_bit/4.0)));
        for (int i = 0; i < int(ceil(data_size_in_bit/4.0)); i++) {
            if (i % 2 == 0) {
                // or ith bit with fingerprint_test_min at bit i/2
                int bit = (fingerprint_test_min_shuffled[i/16] >> (i/2)%8) & 1;
                // printf("bit from min: %d\n", bit);
                fingerprint[i/8] |= (bit << (i%8));
            } else {
                int bit = (fingerprint_test_max_shuffled[i/16] >> (i/2)%8) & 1;
                // printf("bit from max: %d\n", bit);
                fingerprint[i/8] |= (bit << (i%8));
            }
        }
        delete[] fingerprint_test_min_shuffled;
        delete[] fingerprint_test_max_shuffled;
        fingerprint_size_in_bit = data_size_in_bit/4;
        return fingerprint;
    }

    void print() const {
        printf("TernaryByteMapHash [2 bit (ternary) per byte ]\n");
    }

};


class ByteSelectionHash : public HashFunction
{
    public:
    ByteSelectionHash(int index) : HashFunction() 
    {
        m_index = index;
    }

    virtual ~ByteSelectionHash() {
    }

    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        fingerprint_size_in_bit = 8;
        u_int8_t * fingerprint = new u_int8_t[1]();
        fingerprint[0] = byteExtractedAtIndex(line_data, m_index, data_size_in_bit);
        return fingerprint;
    }

    void print() const {
        printf("ByteSelectionHash [ at index %d ]\n", m_index);
    }

    int m_index;

};

class BitSelectionHash : public HashFunction
{
    public:
    BitSelectionHash(int index) : HashFunction() 
    {
        m_index = index;
    }

    virtual ~BitSelectionHash() {
    }

    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        assert (m_index < data_size_in_bit);
        fingerprint_size_in_bit = 1;
        u_int8_t * fingerprint = new u_int8_t[1]();
        fingerprint[0] = (u_int8_t)bitExtractedAtIndex(line_data, m_index);
        return fingerprint;
    }

    void print() const {
        printf("BitSelectionHash [ at index %d ]\n", m_index);
    }

    int m_index;

};

#endif