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
#include "compression/wordPatternRecognizer.hh"

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
        srand(m_seed);
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


    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        assert (data_size_in_bit >= m_fingerprint_size_bit);
        u_int8_t * scrambled = scramble_bit(line_data, data_size_in_bit, m_seed);
        fingerprint_size_in_bit = m_fingerprint_size_bit;
        u_int8_t * fingerprint = new u_int8_t[int(ceil(m_fingerprint_size_bit/8.0))]();
        memset(fingerprint, 0, int(ceil(m_fingerprint_size_bit/8.0)));
        for (int i = 0; i < m_fingerprint_size_bit; i++) {
            fingerprint[i/8] |= (scrambled[i/8] & (1 << (i%8)));
        }
        delete[] scrambled;
        return fingerprint;
    }

    void print() const {
        printf("BitSamplingLSHash [ fp_bits: %d ]\n", m_fingerprint_size_bit);
    }

    int m_fingerprint_size_bit;
};

class MaskedBitSamplingLSHash : public HashFunction
{
    public:
    int m_everyNbyte;
    int m_bits_to_take;

    MaskedBitSamplingLSHash(int fingerprint_size_bit, int everyNbyte, int bits_to_take) : HashFunction() 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
        m_everyNbyte = everyNbyte;
        m_bits_to_take = bits_to_take;
    }
    MaskedBitSamplingLSHash(int fingerprint_size_bit, int everyNbyte, int bits_to_take, unsigned seed) : HashFunction(seed) 
    {
        m_fingerprint_size_bit = fingerprint_size_bit;
        m_everyNbyte = everyNbyte;
        m_bits_to_take = bits_to_take;
    }

    virtual ~MaskedBitSamplingLSHash() {
    }


    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        int max_sampled_bits = (data_size_in_bit/8/m_everyNbyte)*m_bits_to_take;
        // printf("max_sampled_bits: %d\n", max_sampled_bits);
        assert (max_sampled_bits >= m_fingerprint_size_bit);
        assert(data_size_in_bit >= max_sampled_bits);
        int max_sampled_bytes = max_sampled_bits/8;
        u_int8_t * sampled = new u_int8_t[max_sampled_bytes]();
        memset(sampled, 0, max_sampled_bytes);
        int bits_to_take_to_byte = int(ceil(m_bits_to_take/8.0));
        for (int i = 0; i < data_size_in_bit/8/m_everyNbyte; i++){
            int read_byte_ind = m_everyNbyte*i+m_everyNbyte-bits_to_take_to_byte;
            for (int j = 0; j < m_bits_to_take; j++){
                int read_bit_ind = read_byte_ind*8+j;
                int bit = bitExtractedAtIndex(line_data, read_bit_ind);
                int write_byte_ind = i*m_bits_to_take/8+j/8;
                int write_bit_ind = (j%8);
                sampled[write_byte_ind] |= (bit << (write_bit_ind));
                // printf("i: %d, j: %d, rd_byte_ind:%d, rd_bit_ind: %d, wr_byte_ind: %d, wr_bit_ind: %d\n", i, j, 
                //     read_byte_ind, read_bit_ind, write_byte_ind, write_bit_ind);
            }
        }

        u_int8_t * scrambled = scramble_bit(sampled, max_sampled_bits, m_seed);
        fingerprint_size_in_bit = m_fingerprint_size_bit;
        u_int8_t * fingerprint = new u_int8_t[int(ceil(m_fingerprint_size_bit/8.0))]();
        memset(fingerprint, 0, int(ceil(m_fingerprint_size_bit/8.0)));
        for (int i = 0; i < m_fingerprint_size_bit; i++) {
            fingerprint[i/8] |= (scrambled[i/8] & (1 << (i%8)));
        }
        delete[] scrambled;
        delete[] sampled;
        return fingerprint;
    }

    void print() const {
        printf("MaskedBitSamplingLSHash [ fp_bits: %d, every %d byte, %d bis to take ]\n", 
            m_fingerprint_size_bit, m_everyNbyte, m_bits_to_take);
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
class MaxByteMapHash : public HashFunction
{
    public:
    MaxByteMapHash() : HashFunction() 
    {
    }

    virtual ~MaxByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = byte_map_max(line_data, data_size_in_bit);
        fingerprint_size_in_bit = data_size_in_bit/8;
        return fingerprint;
    }

    void print() const {
        printf("MaxByteMapHash \n");
    }

};

class SparseByteMapHash : public HashFunction
{
    public:
    SparseByteMapHash(int everyNByte) : HashFunction() 
    {
        m_everyNByte = everyNByte;
        m_bytes_to_take = 1;
    }
    SparseByteMapHash(int everyNByte, int bytes_to_take) : HashFunction() 
    {
        m_everyNByte = everyNByte;
        m_bytes_to_take = bytes_to_take;
    }

    virtual ~SparseByteMapHash() {
    }

    // writing fingerprint size
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        u_int8_t * fingerprint = sparse_byte_map(line_data, data_size_in_bit, m_everyNByte, m_bytes_to_take);
        fingerprint_size_in_bit = (data_size_in_bit/8/m_everyNByte)*m_bytes_to_take;
        return fingerprint;
    }

    void print() const {
        printf("SparseByteMapHash [ every %d byte, take %d bytes ]\n", m_everyNByte, m_bytes_to_take);
    }

    int m_everyNByte;
    int m_bytes_to_take;

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
template <class T>
class BaseWordLabelingHash : public HashFunction
{
    public:
    T * m_word_recognizer;
    BaseWordLabelingHash(T * word_recognizer) : HashFunction(), m_word_recognizer(word_recognizer) 
    {
        assert(m_word_recognizer->get_num_patterns() > 0);
        assert(m_word_recognizer->get_num_patterns() < 256);
    }
    ~BaseWordLabelingHash() {
        delete m_word_recognizer;
    }
    int get_fingerprint_size_in_bit(int data_size_in_bit) {
        int pattern_size_in_bit = m_word_recognizer->get_pattern_size_in_bits();
        assert(pattern_size_in_bit < 8);
        return data_size_in_bit / 8 / 8 * pattern_size_in_bit;
    }
    u_int8_t * hash(u_int8_t * line_data, int data_size_in_bit, int &fingerprint_size_in_bit) {
        fingerprint_size_in_bit = get_fingerprint_size_in_bit(data_size_in_bit);
        assert(fingerprint_size_in_bit < 64);
        int num_byte = int(ceil(fingerprint_size_in_bit / 8.0));
        u_int8_t * fingerprint = new u_int8_t[num_byte]();
        memset(fingerprint, 0, num_byte);
        for (int i = 0; i < data_size_in_bit / 8 / 8; i++) {
            u_int8_t * word = line_data + i * 8;
            u_int8_t label = (u_int8_t) m_word_recognizer->get_word_pattern_type(word);
            // printf("i: %d, label: %d\n", i, label);
            int pattern_size_in_bit = m_word_recognizer->get_pattern_size_in_bits();
            for (int j = 0; j < pattern_size_in_bit; j++) {
                int write_bit_ind = i * pattern_size_in_bit + j;
                int write_byte_ind = write_bit_ind / 8;
                u_int8_t val = (label & (1 << (j % 8))) >> (j % 8) << (write_bit_ind % 8);
                fingerprint[write_byte_ind] |= val;
                // printf("write_bit_ind: %d, write_val: %d\n", write_bit_ind, val);
            }
        }
        return fingerprint;
    }
    virtual void print() const = 0;
};

class EPCWordLabelingHash : public BaseWordLabelingHash<EPCWordPatternRecognizer>
{
    public:
    EPCWordLabelingHash() : BaseWordLabelingHash<EPCWordPatternRecognizer>(new EPCWordPatternRecognizer()) 
    {
    }
    ~EPCWordLabelingHash() {
    }
    void print() const {
        printf("EPCWordLabelingHash\n");
        m_word_recognizer->print();
    }
};


class StrongWordLabelingHash : public BaseWordLabelingHash<StrongWordPatternRecognizer>
{
    public:
    StrongWordLabelingHash() : BaseWordLabelingHash<StrongWordPatternRecognizer>(new StrongWordPatternRecognizer()) 
    {
    }
    ~StrongWordLabelingHash() {
    }
    void print() const {
        printf("StrongWordLabelingHash\n");
        m_word_recognizer->print();
    }
};

class HyCompWordLabelingHash : public BaseWordLabelingHash<HyCompWordPatternRecognizer>
{
    public:
    HyCompWordLabelingHash() : BaseWordLabelingHash<HyCompWordPatternRecognizer>(new HyCompWordPatternRecognizer()) 
    {
    }
    ~HyCompWordLabelingHash() {
    }
    void print() const {
        printf("HyCompWordLabelingHash\n");
        m_word_recognizer->print();
    }
};

class SemanticWordLabelingHash : public BaseWordLabelingHash<SemanticWordPatternRecognizer>
{
    public:
    SemanticWordLabelingHash() : BaseWordLabelingHash<SemanticWordPatternRecognizer>(new SemanticWordPatternRecognizer()) 
    {
    }
    ~SemanticWordLabelingHash() {
    }
    void print() const {
        printf("SemanticWordLabelingHash\n");
        m_word_recognizer->print();
    }
};

class DensityWordLabelingHash : public BaseWordLabelingHash<DensityWordPatternRecognizer>
{
    public:
    DensityWordLabelingHash() : BaseWordLabelingHash<DensityWordPatternRecognizer>(new DensityWordPatternRecognizer()) 
    {
    }
    ~DensityWordLabelingHash() {
    }
    void print() const {
        printf("DensityWordLabelingHash\n");
        m_word_recognizer->print();
    }
};

class AverageByteMSBWordLabelingHash : public BaseWordLabelingHash<AverageByteMSBWordPatternRecognizer>
{
    public:
    AverageByteMSBWordLabelingHash(int num_msbs) : 
        BaseWordLabelingHash<AverageByteMSBWordPatternRecognizer>(new AverageByteMSBWordPatternRecognizer(num_msbs)) 
    {
    }
    ~AverageByteMSBWordLabelingHash() {
    }
    void print() const {
        printf("AverageByteMSBWordLabelingHash\n");
        m_word_recognizer->print();
    }
};


#endif