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
//     bool cascade = false;

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     vector<HashFunction *> hash_functions;

//     int fp_size_in_bits = 32;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     printf("fp_size_in_bits: %d\n", fp_size_in_bits);
//     printf("num_clusters: %ld\n", num_clusters);

//     int seg_size = 16;
//     int funct_to_concact = 0;
//     for (int i = 0; i < line_size/seg_size; i++){
//         ByteSelectionHash * bs = new ByteSelectionHash(seg_size*i+seg_size-1);
//         bs->print();
//         hash_functions.push_back(bs);
//         funct_to_concact++;
//     }
//     hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));




//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions, cascade, funct_to_concact);

//     cache->populate_lines(filenames_data, filenames_addr);
//     // double bit_entropy = cache->get_bit_entropy();
//     // printf("bit entropy: %f\n", bit_entropy);
//     cache->print();
//     delete cache;

//     return 0;
// }