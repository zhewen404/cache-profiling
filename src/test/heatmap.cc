// #include "cache/cache.hh"
// #include "heatmap/axis.hh"
// #include "common/file/file_read.hh"
// #include <stdio.h>
// #include <string>

// int main(int argc, char *argv[]){
//     (void)argc;
//     (void)argv;
    
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

//     // BankedClusteredCache * cache;
//     // vector<HashFunction *> hash_functions;

//     //////// thesaurus mapfunction ////////
//     // int fp_size_in_bits = 10;
//     // u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size, 12345);
//     // hash_functions.push_back(the);
//     // cache = new BankedClusteredCache(
//     //     num_banks, KB_per_bank, assoc, line_size, 
//     //     shift_bank, shift_set, 
//     //     num_clusters, hash_functions, 1);

//     //////// sbl mapfunction ////////
//     // int true_hash = 0;
//     // int funct_to_concact = 0;
//     // bool cascade = true;
//     // int fp_size_in_bits = 7;
//     // u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     // SparseByteMapHash * bm = new SparseByteMapHash(8, 6);
//     // hash_functions.push_back(bm);
//     // true_hash += 1;
//     // FullBitShuffleHash * fbs = new FullBitShuffleHash(12345);
//     // hash_functions.push_back(fbs);
//     // true_hash += 1;
//     // XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
//     // hash_functions.push_back(x);
//     // cache = new BankedClusteredCache(
//     //     num_banks, KB_per_bank, assoc, line_size, 
//     //     shift_bank, shift_set, 
//     //     num_clusters, hash_functions, cascade, funct_to_concact, true_hash);

//     //////// bdi encoding mapfunction ////////
//     // int fp_size_in_bits = 4;
//     // u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
//     // BDILineLabelingHash * bdi_encoding_hash = new BDILineLabelingHash(true, false);
//     // hash_functions.push_back(bdi_encoding_hash);
//     // int true_hash = 0;
//     // int funct_to_concact = 0;
//     // bool cascade = true;
//     // cache = new BankedClusteredCache(
//     //     num_banks, KB_per_bank, assoc, line_size, 
//     //     shift_bank, shift_set, 
//     //     num_clusters, hash_functions, cascade, funct_to_concact, true_hash);


//     cache->populate_lines(filenames_data, filenames_addr);
//     printf("cache populated\n");
    
//     ThreeHierarchyLines * thl;
//     thl = new ThreeHierarchyLines(cache);
//     thl->generate_empty_heatmap();
//     thl->fill_XORedLines();

//     // thl->fill_heatmap("bit");
//     // thl->plot("heatmap_bdiencoding_bithamming.png");

//     // thl->fill_heatmap("byte");
//     // thl->plot("heatmap_bdiencoding_bytehamming.png");

//     // BDICompressor * m_intraCompressor = new BDICompressor();
//     // thl->fill_heatmap<BDILine>(m_intraCompressor);
//     // thl->plot("heatmap_bdi.png");
//     // delete m_intraCompressor;

//     BitPlaneCompressor * m_intraCompressor = new BitPlaneCompressor();
//     thl->fill_heatmap<BPCLine>(m_intraCompressor);
//     thl->plot("heatmap_bitplane.png");
//     delete m_intraCompressor;

//     delete cache;
//     delete thl;

// }