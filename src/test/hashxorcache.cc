// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
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
//     int fp_size_in_bits = 10;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     printf("num_clusters: %ld\n", num_clusters);

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     vector<HashFunction *> hash_functions;
//     // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
//     // hash_functions.push_back(the);

//     // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(false);
//     // hash_functions.push_back(bbs);

//     // FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     // hash_functions.push_back(fbs);


//     ByteMapHash * bm = new ByteMapHash();
//     hash_functions.push_back(bm);


//     // TernaryByteMapHash * tbm = new TernaryByteMapHash();
//     // tbm->print();
//     // hash_functions.push_back(tbm);

//     XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     x->print();
//     hash_functions.push_back(x);




//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions,1);

//     cache->populate_lines(filenames_data, filenames_addr);
//     double bit_entropy = cache->get_bit_entropy();
//     printf("vanilla bit entropy: %f\n", bit_entropy);
//     // cache->print();



//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*cache);
//     bit_entropy = hxorCache->get_bit_entropy();
//     printf("hxor bit entropy: %f\n", bit_entropy);
//     hxorCache->print();
//     int num_xor = hxorCache->m_num_xored_lines;
//     int num_false_positive = hxorCache->m_num_false_positive;
//     double false_positive_rate = (double)num_false_positive / (double)num_xor * 100;
//     printf("false positive rate: %f = %d/%d\n", false_positive_rate, num_false_positive, num_xor);

//     delete hxorCache;





//     delete cache;
    
//     // for (HashFunction * hf : hash_functions) {
//     //     delete hf;
//     // }

//     return 0;
// }