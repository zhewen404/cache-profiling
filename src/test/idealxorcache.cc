// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
// #include "common/file/file_read.hh"
// #include "compression/intraCompressor.hh"
// #include "cache/compressedCache.hh"
// #include <stdio.h>
// #include <string>
// #include <iostream>

// int main(void) {
//     int num_banks = 1;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input_perfect";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     Cache * cache;
//     cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
//     cache->populate_lines(filenames_data, filenames_addr);
//     double entropy = cache->get_bit_entropy();
//     printf("vanila bit entropy: %f\n", entropy);

//     IdealBankXORCache * xorIdealBankCache;
//     xorIdealBankCache = new IdealBankXORCache(*cache, new BDICompressor());
//     entropy = xorIdealBankCache->get_bit_entropy();
//     printf("ideal bank bit entropy: %f\n", entropy);
//     // xorIdealBankCache->print();

//     BDICompressedXORCache * xorCompressedCache;
//     xorCompressedCache = new BDICompressedXORCache(*xorIdealBankCache, true, false);
//     double inter = xorCompressedCache->get_inter_compression_ratio();
//     double intra = xorCompressedCache->get_intra_compression_ratio();
//     printf("inter compression ratio: %f\n", inter);
//     printf("intra compression ratio: %f\n", intra);


//     delete xorIdealBankCache;
//     delete xorCompressedCache;

//     delete cache;

//     return 0;
// }