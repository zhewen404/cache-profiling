#include "common/bit/bitvec.hh"
#include <array>
#include <algorithm>
#include <math.h>
#include <random>
#include <cstring>
#include <assert.h>
#include <bitset>


// Function to extract bits from small to large inclusively
u_int64_t bitExtractedInclusive(u_int64_t number, int large, int small)
{
   int k = large - small + 1;
   return (u_int64_t)(((1 << k) - 1) & (number >> (small)));
}

bool bitExtractedAtIndex(u_int8_t * number, int index)
{
   int byte_index = index / 8;
   int bit_index = index % 8;
   u_int8_t byte = number[byte_index];
   // printf("byte[%d]=%d\n", byte_index, byte);
   u_int8_t shifted = byte >> bit_index;
   // printf("shifted=%d\n", shifted);
   u_int8_t final = shifted & 1;
   // printf("bit[%d]=%d\n", index, final);
   return final;
}

// Function to extract bytes at a given index
u_int8_t byteExtractedAtIndex(u_int8_t * number, int index, int num_total_bits)
{
   assert(index < num_total_bits / 8);
   return number[index];
}

// Function to count set bits in an 64 bit unsigned integer
template <typename T>
unsigned countSetBits(T n)
{
   unsigned count = 0;
   while (n != 0) {
      count += n & 1;
      n = n >> 1;
   }
   return count;
}

unsigned countSetBits(const u_int8_t* data, const int data_size) 
{
   unsigned count = 0;

   for (int i = 0; i < data_size; i++) {
      u_int8_t byte = data[i];
      count += countSetBits<u_int8_t>(byte);
   }

   return count;
}

void print_xor_folding_equation(int data_size_in_bit, int fingerprint_size_in_bit) {
   int num_chunks = ceil((double)data_size_in_bit / (double)fingerprint_size_in_bit);
   int totoal_size = fingerprint_size_in_bit * num_chunks; // in bit
   printf("data_size_in_bit=%d, fingerprint_size_in_bit=%d, totoal_size=%d, num_chunks=%d\n", data_size_in_bit, fingerprint_size_in_bit, totoal_size, num_chunks);

   printf("fingerprint[%d] = {\n", fingerprint_size_in_bit);
   for (int i = 0; i < fingerprint_size_in_bit; i++)
   {
      for (int j = 0; j < num_chunks; j++)
      {
         int bit_index = i+j*(fingerprint_size_in_bit);
         int bit_index_next = i+(j+1)*(fingerprint_size_in_bit);
         if (bit_index < data_size_in_bit) {
            printf("data[%d]", bit_index);
         } 
         else {
            printf("0");
         }
         if (j < num_chunks - 1 && bit_index_next < data_size_in_bit) {
            printf(" ^ ");
         }
      }
      if (i < fingerprint_size_in_bit) {
         printf(", \n");
      }
   }
   printf("}\n");
}

// Function to perform regular xor-folding
u_int8_t* xor_folding(u_int8_t* data, int data_size_in_bit, int fingerprint_size_in_bit)
{
   u_int8_t * fingerprint;
   fingerprint = new u_int8_t[int(ceil(fingerprint_size_in_bit / 8.0))]();
   // note that fingerprint_size_in_bit can be arbitrary, say non-power-of-2, so we may need to pad the data
   
   // compute the padding and number of times folding is needed
   int num_chunks = ceil((double)data_size_in_bit / (double)fingerprint_size_in_bit);
   // int totoal_size = fingerprint_size_in_bit * num_chunks; // in bit

   for (int i = 0; i < fingerprint_size_in_bit; i++)
   {
      int j = 0;
      int bit_index = i+j*(fingerprint_size_in_bit);
      bool bit;
      if (bit_index >= data_size_in_bit) {
         bit = 0;
      } else {
         bit = bitExtractedAtIndex(data, bit_index);
      }
      // printf("    0th chunk, bit[%d]=%d\n", bit_index, bit);

      for (int j = 1; j < num_chunks; j++)
      {
         int bit_index = i+j*(fingerprint_size_in_bit);
         bool new_bit;
         if (bit_index < data_size_in_bit) {
            new_bit = bitExtractedAtIndex(data, bit_index);
            bit = bit ^ new_bit;
            // printf("    %dth chunk, bit[%d]=%d\n", j, bit_index, bit);
         } 
         // else {
         //    new_bit = 0;
         // }
         // printf("XORing data[%d] with data[%d]\n", i+(j-1)*(fingerprint_size_in_bit), bit_index);  // Add this line

      }
      // set the bit at figerprint_size_in_bit-i to be bit
      int ind = i;
      int byte_set = (ind) / 8;
      int bit_set = (ind) % 8;
      // printf("setting bit %d to %d\n", ind, bit);
      fingerprint[byte_set] |= bit << bit_set;
      // printf("\n");
   }
   return fingerprint;

}

// Function to perform bit-scrambling xor-folding
u_int8_t* xor_folding_scramble_bit(
   u_int8_t* data, int data_size_in_bit, int fingerprint_size_in_bit, unsigned seed)
{
   u_int8_t * data_scrambled;
   data_scrambled = scramble_bit(data, data_size_in_bit, seed);
   u_int8_t * fingerprint = xor_folding(data_scrambled, data_size_in_bit, fingerprint_size_in_bit);
   delete [] data_scrambled;
   return fingerprint;
}

// Function to perform bit-scrambling within byte boundaries xor-folding
u_int8_t* xor_folding_scramble_bit_within_byte(
   u_int8_t* data, 
   int data_size_in_bit, 
   int fingerprint_size_in_bit,
   unsigned* seeds)
{
   u_int8_t * data_scrambled;
   data_scrambled = scramble_bit_within_byte(data, data_size_in_bit, seeds);
   u_int8_t * fingerprint = xor_folding(data_scrambled, data_size_in_bit, fingerprint_size_in_bit);
   delete [] data_scrambled;
   return fingerprint;
}

u_int8_t* scramble_bit(u_int8_t* data, int data_size_in_bit, unsigned seed)
{
   u_int8_t * data_scrambled;
   assert(data_size_in_bit % 8 == 0); // data_size_in_bit must be a multiple of 8
   int data_size_in_byte = data_size_in_bit / 8;

   data_scrambled = new u_int8_t[data_size_in_byte]();
   memcpy(data_scrambled, data, data_size_in_byte);

   //Shuffle the bits randomly
   default_random_engine rng(seed);

   // Convert byte array to bit vector
   vector<bool> bits;
   for (int i = 0; i < data_size_in_byte; ++i) {
      bitset<8> byte_bits(data_scrambled[i]);
      for (int j = 0; j < 8; ++j) {
         bits.push_back(byte_bits[j]);
      }
   }

   shuffle(bits.begin(), bits.end(), rng);

   // Convert bit vector back to byte array
   for (int i = 0; i < data_size_in_byte; ++i) {
      bitset<8> byte_bits;
      for (int j = 0; j < 8; ++j) {
         byte_bits[j] = bits[i * 8 + j];
      }
      data_scrambled[i] = byte_bits.to_ulong();
   }

   return data_scrambled;
}


u_int8_t* scramble_bit_within_byte(
   u_int8_t* data, 
   int data_size_in_bit, 
   unsigned* seeds)
{
   u_int8_t * data_scrambled = new u_int8_t[data_size_in_bit / 8];
   memset(data_scrambled, 0, data_size_in_bit / 8);

   for (int i = 0; i < data_size_in_bit / 8; i++) {
      u_int8_t * byte_scrambled = scramble_bit(data+i, 8, seeds[i]);
      memcpy(data_scrambled + i, byte_scrambled, 1);
      delete [] byte_scrambled;
   }
   return data_scrambled;
}

// convert data into a byte map, length shrink to 1/8
u_int8_t * byte_map(u_int8_t * data, int data_size_in_bit)
{
   u_int8_t * byte_map;
   byte_map = new u_int8_t[int(ceil(data_size_in_bit / 64.0))]();
   memset(byte_map, 0, int(ceil(data_size_in_bit / 64.0)));
   for (int i = 0; i < int(ceil(data_size_in_bit / 8.0)); i++)
   {
      int byte_index = i / 8;
      int bit_index = i % 8;
      if (data[i] != 0) {
         byte_map[byte_index] |= 1 << bit_index;
      }
   }
   return byte_map;
}

// convert data into a byte map, length shrink to 1/8
u_int8_t * byte_map_max(u_int8_t * data, int data_size_in_bit)
{
   u_int8_t * byte_map;
   byte_map = new u_int8_t[int(ceil(data_size_in_bit / 64.0))]();
   memset(byte_map, 0, int(ceil(data_size_in_bit / 64.0)));
   for (int i = 0; i < int(ceil(data_size_in_bit / 8.0)); i++)
   {
      int byte_index = i / 8;
      int bit_index = i % 8;
      if (data[i] == 255) {
         byte_map[byte_index] |= 1 << bit_index;
      }
   }
   return byte_map;
}
u_int8_t * byte_map_min(u_int8_t * data, int data_size_in_bit)
{
   u_int8_t * byte_map;
   byte_map = new u_int8_t[int(ceil(data_size_in_bit / 64.0))]();
   memset(byte_map, 0, int(ceil(data_size_in_bit / 64.0)));
   for (int i = 0; i < int(ceil(data_size_in_bit / 8.0)); i++)
   {
      int byte_index = i / 8;
      int bit_index = i % 8;
      if (data[i] == 0) {
         byte_map[byte_index] |= 1 << bit_index;
      }
   }
   return byte_map;
}

// convert data into a byte map, length shrink to 1/(n*8)
u_int8_t * nbyte_map(u_int8_t * data, int data_size_in_bit, int nByte)
{
   u_int8_t * byte_map;
   int fp_num_byte = int(ceil(data_size_in_bit / (nByte*64.0)));
   int fp_num_bit = int(ceil(data_size_in_bit / (nByte*8.0)));
   byte_map = new u_int8_t[fp_num_byte]();
   memset(byte_map, 0, fp_num_byte);
   for (int i = 0; i < fp_num_bit; i++)
   {
      int byte_index = i / 8;
      int bit_index = i % 8;
      // for all bytes in n bytes, test against zero
      bool set_bit = false;
      for (int j = 0; j < nByte; j++) {
         if (nByte*i+j >= data_size_in_bit/8) {
            break;
         }
         if (data[nByte*i+j] != 0) {
            // any byte is not zero, then set the bit
            // printf("byte[%d]=%d non zero\n", nByte*i+j, data[nByte*i+j]);
            set_bit = true;
            break;
         }
      }
      if (set_bit) {
         // printf("consecutive %dbyte[%d] is non zero\n", nByte, i);
         byte_map[byte_index] |= 1 << bit_index;
      }
   }
   return byte_map;
}

u_int8_t * sparse_byte_map(u_int8_t * data, int data_size_in_bit, int everyNByte)
{
   u_int8_t * byte_map;
   int num_byte_needed = int(ceil(data_size_in_bit / 64.0 / everyNByte));
   byte_map = new u_int8_t[num_byte_needed]();
   memset(byte_map, 0, num_byte_needed);
   //0...63
   //1, 3, 5, ..., 63  (every 2)
   //3, 7, 11, ..., 63 (every 4)

   for (int i = 0; i < int(ceil(data_size_in_bit / 8.0 / everyNByte)); i++) // for every  n byte in data
   {
      int byte_index = i / 8;
      int bit_index = i % 8;
      if (data[(i+1)*everyNByte-1] != 0) {
         byte_map[byte_index] |= 1 << bit_index;
      }
   }
   return byte_map;
}

