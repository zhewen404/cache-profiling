// #include "common/bit/bitvec.hh"
// #include <iostream>
// #include <math.h>

// int main(void) {
//     int data_size = 16;
//     int fingerprint_size_bits = 9;
//     int fingerprint_size_bytes = ceil(fingerprint_size_bits / 8.0);
//     unsigned seed = 12345;

//     u_int8_t data[data_size] = {
//         0b10101010, 0b01010101, 0b10101010, 0b01010101,
//         0b10101010, 0b01010111, 0b10100010, 0b01010101,
//         0b10101010, 0b01010101, 0b10101010, 0b01010101,
//         0b10101010, 0b01010111, 0b11111111, 0b00000000,
//     };
//     int set_bits = countSetBits(data, data_size);
//     printf("<set_bits=%d>\n", set_bits);

//     u_int8_t * fingerprint = xor_folding(data, data_size*8, fingerprint_size_bits);

//     // print that value
//     for (int i = 0; i < fingerprint_size_bytes; i++) {
//         printf("regular fingerprint[%d]=%d\n", i, fingerprint[i]);
//     }
//     printf("\n");
//     delete [] fingerprint;




//     fingerprint = xor_folding_scramble_bit(data, data_size*8, fingerprint_size_bits, seed);

//     // print that value
//     for (int i = 0; i < fingerprint_size_bytes; i++) {
//         printf("bitscrable fingerprint[%d]=%d\n", i, fingerprint[i]);
//     }
//     printf("\n");
//     delete [] fingerprint;




//     u_int8_t * data_scramble = scramble_bit(data, data_size*8, seed);
//     int set_bits_scrambled = countSetBits(data_scramble, data_size);
//     for (int i = 0; i < data_size; i++) {
//         printf("scramble data[%d]=%x\n", i, data_scramble[i]);
//     }
//     delete [] data_scramble;
//     printf("<set_bits_scrambled=%d>\n", set_bits_scrambled);
//     printf("\n");





//     unsigned seeds[data_size];
//     for (int i = 0; i < data_size; i++) {
//         seeds[i] = seed+i;
//     }
//     fingerprint = xor_folding_scramble_bit_within_byte(data, data_size*8, fingerprint_size_bits, seeds);

//     // print that value
//     for (int i = 0; i < fingerprint_size_bytes; i++) {
//         printf("bitscramble (within b) fingerprint[%d]=%d\n", i, fingerprint[i]);
//     }
//     printf("\n");
//     delete [] fingerprint;



//     u_int8_t * data_scramble_within_b = scramble_bit_within_byte(data, data_size*8, seeds);
//     int set_bits_scrambled_within_b = countSetBits(data_scramble_within_b, data_size);
//     for (int i = 0; i < data_size; i++) {
//         printf("scramble_within_b data[%d]=%x\n", i, data_scramble_within_b[i]);
//     }
//     delete [] data_scramble_within_b;
//     printf("<set_bits_scrambled_within_b=%d>\n", set_bits_scrambled_within_b);
//     printf("\n");

//     u_int8_t * byte_map_data = byte_map(data, data_size*8);
//     // print that value
//     for (int i = 0; i < data_size/8; i++) {
//         printf("byte_map_data[%d]=%d\n", i, byte_map_data[i]);
//     }
//     delete [] byte_map_data;

//     u_int8_t * byte_map_data_min = byte_map_min(data, data_size*8);
//     u_int8_t * byte_map_data_max = byte_map_max(data, data_size*8);
//     // print that value
//     for (int i = 0; i < data_size/8; i++) {
//         printf("byte_map_data_min[%d]=%d\n", i, byte_map_data_min[i]);
//         printf("byte_map_data_max[%d]=%d\n", i, byte_map_data_max[i]);
//     }
//     delete [] byte_map_data_min;
//     delete [] byte_map_data_max;
// }
