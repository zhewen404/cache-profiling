// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
// #include "common/file/file_read.hh"
// #include "compression/intraCompressor.hh"
// #include "cache/compressedCache.hh"
// #include <stdio.h>
// #include <string>
// #include <iostream>
// #include "common/plot/plot.hh"

// int main(void) {
//     int num_banks = 4;
//     int line_size = 64;
//     int assoc = 16;
//     int KB_per_bank = 256;
//     int shift_bank = 0;
//     int shift_set = 0;
//     int fp_size_in_bits = 12;
//     u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     printf("num_clusters: %ld\n", num_clusters);

//     vector<string> filenames_data;
//     vector<string> filenames_addr;
//     string dir = "test_input";
//     fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

//     Cache * cache;
//     cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
//     cache->populate_lines(filenames_data, filenames_addr);
//     double entropy = cache->get_bit_entropy();
//     printf("vanila bit entropy: %f\n", entropy);
    
//     ThesaurusCompressedCache * tCache;
//     tCache = new ThesaurusCompressedCache(*cache, 12345);
//     // printf("done with tCache creation\n");

//     double cr_the_baseline = tCache->get_compression_ratio();
//     printf("thesaurus baseline compression ratio: %f\n", cr_the_baseline);
//     tCache->print();

//     printf("\n");
//     printf("\n");
    
//     BankedClusteredCache * ccache;

//     vector<HashFunction *> hash_functions;
//     ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size, 12345);
//     hash_functions.push_back(the);
//     // int funct_to_concact = 0;
//     // int funct_true_hash = 1;
//     // bool cascade = false;
    
//     ccache = new BankedClusteredCache(
//         num_banks, KB_per_bank, assoc, line_size, 
//         shift_bank, shift_set, 
//         num_clusters, hash_functions, 1);

//     ccache->populate_lines(filenames_data, filenames_addr);
//     double bit_entropy = ccache->get_bit_entropy();
//     printf("vanilla bit entropy: %f\n", bit_entropy);
//     // ccache->print();

//     vector<double> * entropies_before = ccache->get_per_byte_entropy();
//     vector<double> xvector;
//     vector<double> maxvec;
//     for (unsigned i = 0; i < entropies_before->size(); i++) {
//         xvector.push_back(i);
//         maxvec.push_back(8);
//     }
//     printf("\n");
//     printf("\n");


//     HashXORCache * hxorCache;
//     hxorCache = new HashXORCache(*ccache, 12345); 
//     entropy = hxorCache->get_bit_entropy();
//     printf("hxor bit entropy: %f\n", entropy);
//     int compsize = hxorCache->get_compressed_size();
//     printf("XOR compressed size: %d\n", compsize);
    
//     // hxorCache->print();
//     int num_xor = hxorCache->m_num_xored_lines;
//     int num_false_positive = hxorCache->m_num_false_positive;
//     double false_positive_rate = (double)num_false_positive / (double)num_xor * 100;
//     printf("false positive rate: %f = %d/%d\n", false_positive_rate, num_false_positive, num_xor);

//     vector<double> * entropies_after = hxorCache->get_per_byte_entropy();
//     vector<vector<double>> yvectors;
//     yvectors.push_back(maxvec);
//     yvectors.push_back(*entropies_before);
//     yvectors.push_back(*entropies_after);
//     string name = "thesaurus12";
//     plot2dx1xn<double>(xvector, yvectors, 
//         {"max", "before", "after"}, 
//         "byte position", "entropy", 
//         "byte_entropy_compare-"+name+".pdf");
    
//     delete entropies_before;
//     delete entropies_after;
//     yvectors.clear();
//     printf("\n");
//     printf("\n");

//     ThesaurusCompressedXORCache * xorCompressedCache;
//     xorCompressedCache = new ThesaurusCompressedXORCache(*hxorCache, 12345);
//     xorCompressedCache->print();
//     double inter = xorCompressedCache->get_inter_compression_ratio();
//     double intra = xorCompressedCache->get_intra_compression_ratio();
//     printf("inter compression ratio: %f\n", inter);
//     printf("intra compression ratio: %f\n", intra);
//     printf("compare it to the baseline cr: %f\n", cr_the_baseline);

//     int uncompressed_size = xorCompressedCache->get_uncompressed_size();
//     int inter_compressed_size = xorCompressedCache->get_xor_compressed_size();
//     int all_compressed_size = xorCompressedCache->get_all_compressed_size();   
//     printf("uncompressed size: %d\n", uncompressed_size);
//     printf("inter compressed size: %d\n", inter_compressed_size);
//     printf("all compressed size: %d\n", all_compressed_size);

//     delete xorCompressedCache;
//     delete hxorCache;

//     delete ccache;
//     delete tCache;
//     delete cache;

//     return 0;
// }