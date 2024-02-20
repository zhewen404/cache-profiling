// #include "cache/cache.hh"
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
//     int fp_size_in_bits = 12;
//     int num_clusters = pow(2,fp_size_in_bits);
//     bool cascade = true;

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     vector<HashFunction *> hash_functions;
//     // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
//     // hash_functions.push_back(the);

//     ByteBitShuffleHash * bbs = new ByteBitShuffleHash(false);
//     hash_functions.push_back(bbs);

//     // FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     // hash_functions.push_back(fbs);
//     XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     hash_functions.push_back(x);




//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions, cascade);

//     cache->populate_lines(filenames_data, filenames_addr);
//     // double bit_entropy = cache->get_bit_entropy();
//     // printf("bit entropy: %f\n", bit_entropy);
//     cache->print();
//     delete cache;
    
//     for (HashFunction * hf : hash_functions) {
//         delete hf;
//     }

//     return 0;
// }