// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
// #include "common/file/file_read.hh"
// #include <stdio.h>
// #include <string>
// #include <iostream>

// int main(void) {
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     Cache * cache;
//     cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
//     cache->populate_lines(filenames_data, filenames_addr);
//     double entropy = cache->get_bit_entropy();
//     printf("vanila bit entropy: %f\n", entropy);

//     RandBankXORCache * xorRandBankCache;
//     xorRandBankCache = new RandBankXORCache(*cache);
//     entropy = xorRandBankCache->get_bit_entropy();
//     printf("rand bank bit entropy: %f\n", entropy);
//     xorRandBankCache->print();
//     delete xorRandBankCache;


//     RandSetXORCache * xorRandSetCache;
//     xorRandSetCache = new RandSetXORCache(*cache);
//     entropy = xorRandSetCache->get_bit_entropy();
//     printf("rand set bit entropy: %f\n", entropy);
//     xorRandSetCache->print();
//     delete xorRandSetCache;
    

//     delete cache;

//     return 0;
// }