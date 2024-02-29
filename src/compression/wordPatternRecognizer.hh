#ifndef COMPRESSION_WORDPATTERNRECOGNIZER_HH
#define COMPRESSION_WORDPATTERNRECOGNIZER_HH
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <assert.h>
#include "common/bit/bitvec.hh"

template <typename T>
class BaseWordPatternRecognizer
{
    public:
        int m_num_patterns;
        BaseWordPatternRecognizer(int num_patterns) : m_num_patterns(num_patterns) {
        }
        virtual ~BaseWordPatternRecognizer() {}
        virtual T get_word_pattern_type(const u_int8_t* word) const = 0;
        int get_num_patterns() const 
        {
            return m_num_patterns;
        }
        int get_pattern_size_in_bits() const
        {
            return int(ceil(log2(get_num_patterns())));
        }
        virtual void print() const = 0;
};

typedef enum {
    ONE_INT_WITH_PADDING_0,//0
    ONE_INT_WITH_PADDING_1,//1
    TWO_INT,//2
    // TWO_INT_1,
    INT_AND_POINTER_0,//3
    INT_AND_POINTER_1,//4
    TWO_FP,//5
    ONE_FP_ONE_INT,//6
    OTHER,//7
} EPCWordPattern_t;
class EPCWordPatternRecognizer : public BaseWordPatternRecognizer<EPCWordPattern_t>
{
    public:
        EPCWordPatternRecognizer() : BaseWordPatternRecognizer<EPCWordPattern_t>(EPCWordPattern_t::OTHER + 1) {
        }

        ~EPCWordPatternRecognizer() {
        }

        void print() const
        {
            printf("EPCWordPatternRecognizer, total 8 patterns\n");
            // list all patterns
            printf("ONE_INT_WITH_PADDING_0: %d\n", ONE_INT_WITH_PADDING_0);
            printf("ONE_INT_WITH_PADDING_1: %d\n", ONE_INT_WITH_PADDING_1);
            printf("TWO_INT: %d\n", TWO_INT);
            printf("INT_AND_POINTER_0: %d\n", INT_AND_POINTER_0);
            printf("INT_AND_POINTER_1: %d\n", INT_AND_POINTER_1);
            printf("TWO_FP: %d\n", TWO_FP);
            printf("ONE_FP_ONE_INT: %d\n", ONE_FP_ONE_INT);
            printf("OTHER: %d\n", OTHER);
        }

        EPCWordPattern_t get_word_pattern_type(const u_int8_t* word) const
        {   
            EPCWordPattern_t type = EPCWordPattern_t::OTHER;
            if (countSetBits(word + sizeof(u_int8_t) * 3, 5) == 0) {
                type = EPCWordPattern_t::ONE_INT_WITH_PADDING_0; //0
            } else if (countSetBits(word + sizeof(u_int8_t) * 3, 5) == 40) {
                type = EPCWordPattern_t::ONE_INT_WITH_PADDING_1; //1
            } else if ((countSetBits(word + sizeof(u_int8_t) * 7, 1) == 0 || countSetBits(word + sizeof(u_int8_t) * 7, 1) == 8) && 
                (countSetBits(word + sizeof(u_int8_t) * 3, 1) == 0 || countSetBits(word + sizeof(u_int8_t) * 3, 1) == 8)) {
                type = EPCWordPattern_t::TWO_INT; //2
            } else if (countSetBits(word + sizeof(u_int8_t) * 7, 1) == 0) {
                type = EPCWordPattern_t::INT_AND_POINTER_0; //3
            } else if (countSetBits(word + sizeof(u_int8_t) * 7, 1) == 8) {
                type = EPCWordPattern_t::INT_AND_POINTER_1; //4
            } else if (word[3] == word[7]) {
                type = EPCWordPattern_t::TWO_FP; //5
            } else if (countSetBits(word + sizeof(u_int8_t) * 3, 1) == 0 || countSetBits(word + sizeof(u_int8_t) * 3, 1) == 8) {
                type = EPCWordPattern_t::ONE_FP_ONE_INT; //6
            } else {
                type = EPCWordPattern_t::OTHER; //7
            }
            // print the 8 bytes and the pattern
            // for (int i = 0; i < 8; i++) {
            //     printf("%02x ", word[i]);
            // }
            // printf("type: %d\n", type);
            return type;
        }
        
};

typedef enum {
    STRONG_ALL_ZERO,
    STRONG_ONE_INT_WITH_PADDING_0,
    STRONG_ONE_INT_WITH_PADDING_1,
    STRONG_OTHER,
} StrongWordPattern_t;
class StrongWordPatternRecognizer : public BaseWordPatternRecognizer<StrongWordPattern_t>
{
    public:
        StrongWordPatternRecognizer() : BaseWordPatternRecognizer<StrongWordPattern_t>(StrongWordPattern_t::STRONG_OTHER + 1) {
        }

        ~StrongWordPatternRecognizer() {
        }

        void print() const
        {
            printf("StrongWordPatternRecognizer, total %d patterns\n", m_num_patterns);
            // list all patterns
        }

        StrongWordPattern_t get_word_pattern_type(const u_int8_t* word) const
        {   
            StrongWordPattern_t type = StrongWordPattern_t::STRONG_OTHER;
            if (countSetBits(word, 8) == 0) {
                type = StrongWordPattern_t::STRONG_ALL_ZERO; //0
            } else if (countSetBits(word + sizeof(u_int8_t) * 3, 5) == 0) {
                type = StrongWordPattern_t::STRONG_ONE_INT_WITH_PADDING_0; //1
            } else if (countSetBits(word + sizeof(u_int8_t) * 3, 5) == 40) {
                type = StrongWordPattern_t::STRONG_ONE_INT_WITH_PADDING_1; //2
            } else {
                type = StrongWordPattern_t::STRONG_OTHER; //3
            }
            // print the 8 bytes and the pattern
            // for (int i = 0; i < 8; i++) {
            //     printf("%02x ", word[i]);
            // }
            // printf("type: %d\n", type);
            return type;
        }
        
};

typedef enum {
    HYCOMP_POS_NULL,
    HYCOMP_NEG_NULL,
    HYCOMP_TWO_DUPLICATES,
    HYCOMP_POS_INT,
    HYCOMP_NEG_INT,
    HYCOMP_POINTER,
    HYCOMP_TWO_SIM_FLOATS,
    HYCOMP_OTHER, // double, instruction, ...
} HyCompWordPattern_t;
class HyCompWordPatternRecognizer : public BaseWordPatternRecognizer<HyCompWordPattern_t>
{
    public:
        HyCompWordPatternRecognizer() : BaseWordPatternRecognizer<HyCompWordPattern_t>(HyCompWordPattern_t::HYCOMP_OTHER + 1) {
        }

        ~HyCompWordPatternRecognizer() {
        }

        void print() const
        {
            printf("HyCompWordPatternRecognizer, total %d patterns\n", m_num_patterns);
            // list all patterns
        }

        HyCompWordPattern_t get_word_pattern_type(const u_int8_t* word) const
        {   
            HyCompWordPattern_t type = HyCompWordPattern_t::HYCOMP_OTHER;
            if (countSetBits(word, 8) == 0) {
                type = HyCompWordPattern_t::HYCOMP_POS_NULL; //0
            } else if (word[7] == 0x80 && countSetBits(word, 7) == 0) {
                type = HyCompWordPattern_t::HYCOMP_NEG_NULL; //1
            } else if (word[7] == word[3] && word[6] == word[2] && word[5] == word[1] && word[4] == word[0]) {
                type = HyCompWordPattern_t::HYCOMP_TWO_DUPLICATES; //2
            } else if (countSetBits(word + sizeof(u_int8_t) * 4, 4) == 0) {
                type = HyCompWordPattern_t::HYCOMP_POS_INT; //3
            } else if (countSetBits(word + sizeof(u_int8_t) * 4, 4) == 32) {
                type = HyCompWordPattern_t::HYCOMP_NEG_INT; //4
            } else if (countSetBits(word + sizeof(u_int8_t) * 6, 2) == 0 && countSetBits(word + sizeof(u_int8_t) * 4, 2) != 0) {
                type = HyCompWordPattern_t::HYCOMP_POINTER; //5
            } else if ((word[3]&0x7f) == (word[7]&0x7f)) {
                type = HyCompWordPattern_t::HYCOMP_TWO_SIM_FLOATS; //6
            } else {
                type = HyCompWordPattern_t::HYCOMP_OTHER; //7
            }
            // print the 8 bytes and the pattern
            // for (int i = 0; i < 8; i++) {
            //     printf("%02x ", word[i]);
            // }
            // printf("type: %d\n", type);
            return type;
        }
        
};
typedef enum {
    SEMANTIC_NULL,
    SEMANTIC_POS_NARROW6, // 6 MSBytes are 00
    SEMANTIC_POS_NARROW4, // 4 MSBytes are 00
    SEMANTIC_TWO_POS_NARROW3, // 3 MSBytes in both half are 00
    SEMANTIC_TWO_POS_NARROW2, // 2 MSBytes in both half are 00
    SEMANTIC_TWO_POS_NARROW1, // 1 MSBytes in both half are 00
    SEMANTIC_NEG_NARROW4, // 4 MSBytes are FF

    SEMANTIC_OTHER, 
} SemanticWordPattern_t;
class SemanticWordPatternRecognizer : public BaseWordPatternRecognizer<SemanticWordPattern_t>
{
    public:
        SemanticWordPatternRecognizer() : BaseWordPatternRecognizer<SemanticWordPattern_t>(SemanticWordPattern_t::SEMANTIC_OTHER + 1) {
        }

        ~SemanticWordPatternRecognizer() {
        }

        void print() const
        {
            printf("SemanticWordPatternRecognizer, total %d patterns\n", m_num_patterns);
            // list all patterns
        }

        SemanticWordPattern_t get_word_pattern_type(const u_int8_t* word) const
        {   
            SemanticWordPattern_t type = SemanticWordPattern_t::SEMANTIC_OTHER;
            if (countSetBits(word, 8) == 0 || (word[7] == 0x80 && countSetBits(word, 7) == 0)) {
                type = SemanticWordPattern_t::SEMANTIC_NULL; //0
            } else if (countSetBits(word + sizeof(u_int8_t) * 2, 6) == 0) {
                type = SemanticWordPattern_t::SEMANTIC_POS_NARROW6; //1
            } else if (countSetBits(word + sizeof(u_int8_t) * 4, 4) == 0) {
                type = SemanticWordPattern_t::SEMANTIC_POS_NARROW4; //2
            } else if (countSetBits(word + sizeof(u_int8_t) * 5, 3) == 0 && countSetBits(word + sizeof(u_int8_t) * 1, 3) == 0) {
                type = SemanticWordPattern_t::SEMANTIC_TWO_POS_NARROW3; //3
            } else if (countSetBits(word + sizeof(u_int8_t) * 6, 2) == 0 && countSetBits(word + sizeof(u_int8_t) * 2, 2) == 0) {
                type = SemanticWordPattern_t::SEMANTIC_TWO_POS_NARROW2; //4
            } else if (countSetBits(word + sizeof(u_int8_t) * 7, 1) == 0 && countSetBits(word + sizeof(u_int8_t) * 3, 1) == 0) {
                type = SemanticWordPattern_t::SEMANTIC_TWO_POS_NARROW1; //5
            } else if (countSetBits(word + sizeof(u_int8_t) * 4, 4) == 32) {
                type = SemanticWordPattern_t::SEMANTIC_NEG_NARROW4; //6
            // } else if (word[7] == word[6] && word[6] == word[5] && word[5] == word[4] && word[4] == word[3] && 
            //     word[3] == word[2] && word[2] == word[1] && word[1] == word[0]) {
            //     type = SemanticWordPattern_t::SEMANTIC_REP_BYTE; //5
            // } else if (memcmp(word, word + sizeof(u_int8_t) * 2, 2) == 0 && 
            //     memcmp(word + sizeof(u_int8_t) * 2, word + sizeof(u_int8_t) * 4, 2) == 0 &&
            //     memcmp(word + sizeof(u_int8_t) * 4, word + sizeof(u_int8_t) * 6, 2) == 0 ){
            //     type = SemanticWordPattern_t::SEMANTIC_REP_2BYTE; //6
            } else {
                type = SemanticWordPattern_t::SEMANTIC_OTHER; //7
            }
            // print the 8 bytes and the pattern
            // for (int i = 0; i < 8; i++) {
            //     printf("%02x ", word[i]);
            // }
            // printf("type: %d\n", type);
            return type;
        }
        
};


typedef enum {
    DENSITY_MIN,
    DENSITY_LOW,
    DENSITY_MED,
    DENSITY_HIGH,
} DensityWordPattern_t;
class DensityWordPatternRecognizer : public BaseWordPatternRecognizer<DensityWordPattern_t>
{
    public:
        DensityWordPatternRecognizer() : BaseWordPatternRecognizer<DensityWordPattern_t>(DensityWordPattern_t::DENSITY_HIGH + 1) {
        }

        ~DensityWordPatternRecognizer() {
        }

        void print() const
        {
            printf("DensityWordPatternRecognizer, total %d patterns\n", m_num_patterns);
            // list all patterns
        }

        DensityWordPattern_t get_word_pattern_type(const u_int8_t* word) const
        {   
            DensityWordPattern_t type = DensityWordPattern_t::DENSITY_HIGH;
            if (countSetBits(word, 8) == 0) {
                type = DensityWordPattern_t::DENSITY_MIN; //0
            } else if (countSetBits(word, 8) > 0 && countSetBits(word, 8) <= 20) {
                type = DensityWordPattern_t::DENSITY_LOW; //1
            } else if (countSetBits(word, 8) > 20 && countSetBits(word, 8) <= 40) {
                type = DensityWordPattern_t::DENSITY_MED; //2
            } else {
                type = DensityWordPattern_t::DENSITY_HIGH; //7
            }
            // print the 8 bytes and the pattern
            // for (int i = 0; i < 8; i++) {
            //     printf("%02x ", word[i]);
            // }
            // printf("type: %d\n", type);
            return type;
        }
        
};

class AverageByteMSBWordPatternRecognizer : public BaseWordPatternRecognizer<unsigned>
{
    public:
        int m_num_msbs;
        AverageByteMSBWordPatternRecognizer(int num_msbs) : BaseWordPatternRecognizer<unsigned>(int(pow(2, num_msbs))), m_num_msbs(num_msbs) {
        }

        ~AverageByteMSBWordPatternRecognizer() {
        }

        void print() const
        {
            printf("AverageByteMSBWordPatternRecognizer, total %d patterns\n", m_num_patterns);
            // list all patterns
        }

        unsigned get_word_pattern_type(const u_int8_t* word) const
        {   
            // init an array of int of size m_num_msbs for registering count of 1s in each msb
            int *msb_count = new int[m_num_msbs];
            memset(msb_count, 0, m_num_msbs * sizeof(int));
            // for every byte in the word
            for (int i = 0; i < 8; i++) {
                // for every bit in the n_num_msb
                for (int j = 0; j < m_num_msbs; j++) {
                    int bit_index = 7-j;
                    if (word[i] & (1 << bit_index)) {
                        msb_count[j]++;
                    }
                }
            }

            // for every msb, if the count is greater than 4, set the bit to 1
            unsigned pattern = 0;
            for (int i = 0; i < m_num_msbs; i++) {
                int index = m_num_msbs - 1 - i;
                int weight = int(pow(2, index));
                if (msb_count[i] >= 4) {
                    msb_count[i] = 1;
                    pattern += weight;
                } else {
                    msb_count[i] = 0;
                }
            }
            delete [] msb_count;
            // test to make sure the pattern does not exceed limit
            assert(pattern < (unsigned)m_num_patterns);
            return pattern;
        }
        
};
#endif