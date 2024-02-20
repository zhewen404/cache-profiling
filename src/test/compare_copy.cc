// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
// #include "common/file/file_read.hh"
// #include "common/plot/plot.hh"
// #include <stdio.h>
// #include <string>
// #include <vector>

// void try_bbsxf(int fp_size_in_bits, double &cr, double &entropy_reduction){
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;
//     // int fp_size_in_bits = 32;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     vector<HashFunction *> hash_functions;
//     // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
//     // hash_functions.push_back(the);

//     ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
//     hash_functions.push_back(bbs);

//     // FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     // hash_functions.push_back(fbs);
//     XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     hash_functions.push_back(x);



//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions);

//     cache->populate_lines(filenames_data, filenames_addr);
//     double vanilla_bit_entropy = cache->get_bit_entropy();
//     // printf("vanilla bit entropy: %f\n", bit_entropy);
//     // cache->print();



//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*cache);
//     double bit_entropy = hxorCache->get_bit_entropy();

//     entropy_reduction = vanilla_bit_entropy - bit_entropy;
//     cr = hxorCache->get_compression_ratio();

//     printf("bbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
//     // hxorCache->print();
//     delete hxorCache;

//     delete cache;
// }


// void try_fbsxf(int fp_size_in_bits, double &cr, double &entropy_reduction){
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

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

//     FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     hash_functions.push_back(fbs);
//     XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     hash_functions.push_back(x);



//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions);

//     cache->populate_lines(filenames_data, filenames_addr);
//     double vanilla_bit_entropy = cache->get_bit_entropy();
//     // printf("vanilla bit entropy: %f\n", bit_entropy);
//     // cache->print();



//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*cache);
//     double bit_entropy = hxorCache->get_bit_entropy();

//     entropy_reduction = vanilla_bit_entropy - bit_entropy;
//     cr = hxorCache->get_compression_ratio();

//     printf("fbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
//     // hxorCache->print();
//     delete hxorCache;

//     delete cache;
// }



// void try_thesaurus(int fp_size_in_bits, double &cr, double &entropy_reduction){
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;
//     // int fp_size_in_bits = 32;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     vector<HashFunction *> hash_functions;
//     ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
//     hash_functions.push_back(the);

//     // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
//     // hash_functions.push_back(bbs);

//     // FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     // hash_functions.push_back(fbs);
//     // XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     // hash_functions.push_back(x);



//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions);

//     cache->populate_lines(filenames_data, filenames_addr);
//     double vanilla_bit_entropy = cache->get_bit_entropy();
//     // printf("vanilla bit entropy: %f\n", bit_entropy);
//     // cache->print();



//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*cache);
//     double bit_entropy = hxorCache->get_bit_entropy();

//     entropy_reduction = vanilla_bit_entropy - bit_entropy;
//     cr = hxorCache->get_compression_ratio();

//     printf("thesaurus lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
//     // hxorCache->print();
//     delete hxorCache;

//     delete cache;
// }



// void try_bs(int fp_size_in_bits, double &cr, double &entropy_reduction){
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;
//     // int fp_size_in_bits = 32;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     ClusteredCache * cache;

//     // vector<HashFunction *> hash_functions;
//     // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
//     // hash_functions.push_back(the);

//     vector<HashFunction *> hash_functions;
//     BitSamplingLSHash * bs = new BitSamplingLSHash(fp_size_in_bits);
//     hash_functions.push_back(bs);

//     // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
//     // hash_functions.push_back(bbs);

//     // FullBitShuffleHash * fbs = new FullBitShuffleHash();
//     // hash_functions.push_back(fbs);
//     // XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     // hash_functions.push_back(x);



//     cache = new ClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions);

//     cache->populate_lines(filenames_data, filenames_addr);
//     double vanilla_bit_entropy = cache->get_bit_entropy();
//     // printf("vanilla bit entropy: %f\n", bit_entropy);
//     // cache->print();



//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*cache);
//     double bit_entropy = hxorCache->get_bit_entropy();

//     entropy_reduction = vanilla_bit_entropy - bit_entropy;
//     cr = hxorCache->get_compression_ratio();

//     printf("bs lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
//     // hxorCache->print();
//     delete hxorCache;

//     delete cache;
// }


// void all(vector <double> &crs, vector <double> &ers, vector<double> fbs, void (*func)(int, double &, double &)){
//     double cr, entropy_reduction;

//     for (unsigned i = 0; i < fbs.size(); i++){
//         func(fbs[i], cr, entropy_reduction);
//         crs.push_back(cr);
//         ers.push_back(entropy_reduction);
//     }
// }

// int main(void) {
//     vector<double> fbs = {
//         1,2,3,4,5,6,7, 8, 9, 10, 
//         11, 12, 13,14,15,16,17,18,19,20,
//         21,22,23,24,25,26,27,28,29,30,
//         31,32,33,34,35,36,37,38,39,40,
//         41,42,43,44,45,46,47,48,49,50,
//         51,52,53,53,55,56,57,58,59,60
//     };
//     // vector<int> fbs = {8};


//     vector<double> crs, ers;
//     all(crs, ers, fbs, &try_bbsxf);
//     vector<vector<double>> data = {crs, ers};
//     plot2dx2<double>(fbs, data, {"", ""}, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
//         "img/bbsxf.pdf");
//     crs.clear();
//     ers.clear();
//     data.clear();

//     all(crs, ers, fbs, &try_fbsxf);
//     data = {crs, ers};
//     plot2dx2<double>(fbs, data, {"", ""}, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
//         "img/fbsxf.pdf");
//     crs.clear();
//     ers.clear();
//     data.clear();

//     all(crs, ers, fbs, &try_thesaurus);
//     data = {crs, ers};
//     plot2dx2<double>(fbs, data, {"", ""}, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
//         "img/thesaurus.pdf");
//     crs.clear();
//     ers.clear();
//     data.clear();

//     all(crs, ers, fbs, &try_bs);
//     data = {crs, ers};
//     plot2dx2<double>(fbs, data, {"", ""}, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
//         "img/bs.pdf");
//     crs.clear();
//     ers.clear();
//     data.clear();

//     fbs.clear();
//     return 0;


// }