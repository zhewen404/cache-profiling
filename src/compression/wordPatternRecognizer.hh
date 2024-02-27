#ifndef COMPRESSION_WORDPATTERNRECOGNIZER_HH
#define COMPRESSION_WORDPATTERNRECOGNIZER_HH
#include <string>
#include <vector>
#include <cmath>
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
    ONE_INT_WITH_PADDING_0,
    ONE_INT_WITH_PADDING_1,
    TWO_INT,
    // TWO_INT_1,
    INT_AND_POINTER_0,
    INT_AND_POINTER_1,
    TWO_FP,
    ONE_FP_ONE_INT,
    OTHER,
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

#endif