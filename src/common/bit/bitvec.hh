#ifndef COMMON_BIT_BITVEC_HH
#define COMMON_BIT_BITVEC_HH

#include <stdlib.h>
using namespace std;

// Function to extract bits from small to large inclusively
u_int64_t bitExtractedInclusive(u_int64_t number, int large, int small);
// Function to extract one bit at a given index
bool bitExtractedAtIndex(u_int8_t * number, int index);
u_int8_t byteExtractedAtIndex(u_int8_t * number, int index, int num_total_bits);
// Function to count set bits in an 64 bit unsigned integer
template <typename T>
unsigned countSetBits(T n);
unsigned countSetBits(const u_int8_t* data, const int data_size);

void print_xor_folding_equation(int data_size_in_bit, int fingerprint_size_in_bit);
// Function to perform regular xor-folding
u_int8_t * xor_folding(u_int8_t* data, int data_size_in_bit, int fingerprint_size_in_bit);
u_int8_t* xor_folding_scramble_bit(u_int8_t* data, int data_size_in_bit, int fingerprint_size_in_bit, unsigned seed);
u_int8_t* xor_folding_scramble_bit_within_byte(u_int8_t* data, int data_size_in_bit, int fingerprint_size_in_bit, unsigned * seeds);

u_int8_t* scramble_bit(u_int8_t* data, int data_size_in_bit, unsigned seed);
u_int8_t* scramble_bit_within_byte(u_int8_t* data, int data_size_in_bit, unsigned * seeds);

// convert data into a byte map, length shrink to 1/8
u_int8_t * byte_map(u_int8_t * data, int data_size_in_bit);
u_int8_t * byte_map_max(u_int8_t * data, int data_size_in_bit);
u_int8_t * byte_map_min(u_int8_t * data, int data_size_in_bit);
u_int8_t * nbyte_map(u_int8_t * data, int data_size_in_bit, int nByte);
u_int8_t * sparse_byte_map(u_int8_t * data, int data_size_in_bit, int everyNByte);
u_int8_t * sparse_byte_map(u_int8_t * data, int data_size_in_bit, int everyNByte, int bytes_to_take);

#endif // COMMON_BIT_BITVEC_HH