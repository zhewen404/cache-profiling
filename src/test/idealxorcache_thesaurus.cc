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

//     IdealBankXORCacheThesaurus * xorIdealBankCache;
//     xorIdealBankCache = new IdealBankXORCacheThesaurus(*cache, new SparseByteCompressor());
//     entropy = xorIdealBankCache->get_bit_entropy();
//     printf("ideal bank bit entropy: %f\n", entropy);
//     // xorIdealBankCache->print();

//     // for (unsigned i = 0; i < xorIdealBankCache->m_lines.size(); i++) {
//     //     XORedLine* xored_line = xorIdealBankCache->m_lines[i];
//     //     if (xored_line->m_line_ptrs.size() == 2) {
//     //         vector<u_int8_t> arr0 = xored_line->m_line_ptrs[0]->get_byte_arrary();
//     //         vector<u_int8_t> arr1 = xored_line->m_line_ptrs[1]->get_byte_arrary();
//     //         if (arr0 == arr1) {
//     //             printf("%d: exactly identical line pair\n", i);
//     //         } else {
//     //             printf("%d: \nA: [ ", i);
//     //             for (unsigned j = 0; j < arr0.size(); j++) {
//     //                 printf("%03d ", arr0[j]);
//     //             }
//     //             printf("], \nB: [ ");
//     //             for (unsigned j = 0; j < arr1.size(); j++) {
//     //                 printf("%03d ", arr1[j]);
//     //             }
//     //             printf("]\n");
//     //         }
//     //         arr0.clear();
//     //         arr1.clear();
//     //     }
//     // }

//     ThesaurusCompressedXORCache * xorCompressedCache;
//     xorCompressedCache = new ThesaurusCompressedXORCache(*xorIdealBankCache, 12345);
//     double inter = xorCompressedCache->get_inter_compression_ratio();
//     double intra = xorCompressedCache->get_intra_compression_ratio();
//     printf("inter compression ratio: %f\n", inter);
//     printf("intra compression ratio: %f\n", intra);

//     int uncompressed_size = xorCompressedCache->get_uncompressed_size();
//     int inter_compressed_size = xorCompressedCache->get_xor_compressed_size();
//     int all_compressed_size = xorCompressedCache->get_all_compressed_size();   
//     printf("uncompressed size: %d\n", uncompressed_size);
//     printf("inter compressed size: %d\n", inter_compressed_size);
//     printf("all compressed size: %d\n", all_compressed_size);

//     delete xorIdealBankCache;
//     // delete xorCompressedCache;

//     delete cache;

//     return 0;
// }