// #include "cache/cache.hh"
// #include "common/file/file_read.hh"
// #include <stdio.h>
// #include <string>

// int main(void) {
//     // int num_banks = 4;
//     int num_banks = 16;
//     int line_size = 64;
//     int assoc = 16;
//     // int KB_per_bank = 256;
//     int KB_per_bank = 64;
//     int shift_bank = 0;
//     int shift_set = 0;

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input_parsec";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     Cache * cache;
//     cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
//     cache->populate_lines(filenames_data, filenames_addr);
//     double bit_entropy = cache->get_bit_entropy();
//     printf("bit entropy: %f\n", bit_entropy);
//     cache->print();
//     delete cache;

//     return 0;
// }