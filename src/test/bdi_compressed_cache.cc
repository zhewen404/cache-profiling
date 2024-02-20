// #include "cache/cache.hh"
// #include "cache/compressedCache.hh"
// #include "common/file/file_read.hh"
// #include <stdio.h>
// #include <string>

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

//     bool use_little_endian = true;
//     bool allow_immo = true;

//     BDICompressedCache * bdiCache;
//     bdiCache = new BDICompressedCache(*cache, use_little_endian, allow_immo);
//     delete cache;


//     bdiCache->print();

//     delete bdiCache;

//     return 0;
// }