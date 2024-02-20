#include "cache/cache.hh"
#include "cache/xorCache.hh"
#include "cache/compressedCache.hh"
#include "common/file/file_read.hh"
#include <stdio.h>
#include <string>
#include "common/plot/plot.hh"

int main(void) {
    int num_banks = 4;
    int line_size = 64;
    int assoc = 16;
    int KB_per_bank = 256;
    int shift_bank = 0;
    int shift_set = 0;
    int fp_size_in_bits = 12;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);
    printf("num_clusters: %ld\n", num_clusters);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    string dir = "test_input";
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    // hash_functions.push_back(the);

    // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(false);
    // hash_functions.push_back(bbs);

    // FullBitShuffleHash * fbs = new FullBitShuffleHash();
    // hash_functions.push_back(fbs);


    // TernaryByteMapHash * tbm = new TernaryByteMapHash();
    // tbm->print();
    // hash_functions.push_back(tbm);

    string name = "bytemapshuffled";
    bool cascade = true;
    int funct_to_concact = 0;
    int funct_true_hash = 2;
    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    x->print();
    hash_functions.push_back(x);

    // string name = "le4-16";
    // int seg_size = 4;
    // int bits_to_take = 16;
    // int bits_to_take_to_byte = int(ceil(bits_to_take/8.0));
    // int funct_to_concact = 0;
    // int funct_true_hash = 0;
    // for (int i = 0; i < line_size/seg_size; i++){
    //     int byte_ind = seg_size*i+seg_size-bits_to_take_to_byte;
    //     for (int j = 0; j < bits_to_take; j++){
    //         int bit_ind = byte_ind*8+j;
    //         // printf("i: %d, j: %d, byte_ind:%d, bit_ind: %d\n", i, j, byte_ind, bit_ind);
    //         BitSelectionHash * bs = new BitSelectionHash(bit_ind);
    //         // bs->print();
    //         hash_functions.push_back(bs);
    //         funct_to_concact++;
    //         funct_true_hash++;
    //     }
    // }
    // hash_functions.push_back(new FullBitShuffleHash()); // with shuffling
    // funct_true_hash++;
    // hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));
    // bool cascade = false;


    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, cascade, funct_to_concact, funct_true_hash);
        // num_clusters, hash_functions,funct_true_hash);

    cache->populate_lines(filenames_data, filenames_addr);
    double bit_entropy = cache->get_bit_entropy();
    printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();

    vector<double> * entropies_before = cache->get_per_byte_entropy();
    vector<double> xvector;
    vector<double> maxvec;
    for (unsigned i = 0; i < entropies_before->size(); i++) {
        xvector.push_back(i);
        maxvec.push_back(8);
    }


    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);
    bit_entropy = hxorCache->get_bit_entropy();
    printf("hxor bit entropy: %f\n", bit_entropy);
    // hxorCache->print();
    int num_xor = hxorCache->m_num_xored_lines;
    int num_false_positive = hxorCache->m_num_false_positive;
    double false_positive_rate = (double)num_false_positive / (double)num_xor * 100;
    printf("false positive rate: %f = %d/%d\n", false_positive_rate, num_false_positive, num_xor);

    vector<double> * entropies_after = hxorCache->get_per_byte_entropy();
    vector<vector<double>> yvectors;
    yvectors.push_back(maxvec);
    yvectors.push_back(*entropies_before);
    yvectors.push_back(*entropies_after);
    plot2dx1xn<double>(xvector, yvectors, 
        {"max", "before", "after"}, 
        "byte position", "entropy", 
        "byte_entropy_compare-"+name+".pdf");
    
    delete entropies_before;
    delete entropies_after;
    yvectors.clear();


    BDICompressedXORCache * bdiXorCache;
    bdiXorCache = new BDICompressedXORCache(*hxorCache);
    double inter_compression_ratio = bdiXorCache->get_inter_compression_ratio();
    double intra_compression_ratio = bdiXorCache->get_intra_compression_ratio();
    double total_compression_ratio = bdiXorCache->get_total_compression_ratio();
    printf("%s\n", name.c_str());
    printf("BDICompressedXORCache inter_compression_ratio: %f\n", inter_compression_ratio);
    printf("BDICompressedXORCache intra_compression_ratio: %f\n", intra_compression_ratio);
    printf("BDICompressedXORCache total_compression_ratio: %f\n", total_compression_ratio);

    delete bdiXorCache;
    delete hxorCache;
    delete cache;
    
    // for (HashFunction * hf : hash_functions) {
    //     delete hf;
    // }

    return 0;
}