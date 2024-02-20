#include "function/clustercache.hh"
#include "cache/cache.hh"
#include "cache/xorCache.hh"
#include "common/file/file_read.hh"
#include <stdio.h>


void try_bbsxf(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    // hash_functions.push_back(the);

    ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
    hash_functions.push_back(bbs);

    // FullBitShuffleHash * fbs = new FullBitShuffleHash();
    // hash_functions.push_back(fbs);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();

        false_rate = hxorCache->get_false_positive_rate();

        printf("bbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("bbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_fbsxf(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    // hash_functions.push_back(the);

    // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(false);
    // hash_functions.push_back(bbs);

    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("fbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {

        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("fbsxf %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;
    }

    delete cache;
}


void try_thesaurus(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    hash_functions.push_back(the);

    // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
    // hash_functions.push_back(bbs);

    // FullBitShuffleHash * fbs = new FullBitShuffleHash();
    // hash_functions.push_back(fbs);
    // XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    // hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("thesaurus lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {

        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("thesaurus lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;
    }

    delete cache;
}


void try_bs(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    // vector<HashFunction *> hash_functions;
    // ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    // hash_functions.push_back(the);

    vector<HashFunction *> hash_functions;
    BitSamplingLSHash * bs = new BitSamplingLSHash(fp_size_in_bits);
    hash_functions.push_back(bs);

    // ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
    // hash_functions.push_back(bbs);

    // FullBitShuffleHash * fbs = new FullBitShuffleHash();
    // hash_functions.push_back(fbs);
    // XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    // hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("bs lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("bs lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {

        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;
    }

    delete cache;
}


void sparsebytemap(bool shuffle, int everyNByte, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    int true_hash = 0;
    vector<HashFunction *> hash_functions;
    SparseByteMapHash * bm = new SparseByteMapHash(everyNByte);
    hash_functions.push_back(bm);
    true_hash += 1;
    if (shuffle) {
        FullBitShuffleHash * fbs = new FullBitShuffleHash();
        hash_functions.push_back(fbs);
        true_hash += 1;
    }
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, true_hash);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("sbm lsh (every %d byte) %d-bit bit entropy: %f, cr:%f\n", 
            everyNByte,fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("sbm lsh (every %d byte) %d-bit bit entropy: %f, cr:%f\n", 
            everyNByte,fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}

void try_sparsebytemap_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(false, 2, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}
void try_sparseshuffledbytemap_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(true, 2, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_sparsebytemap_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(false, 4, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}
void try_sparseshuffledbytemap_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(true, 4, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}
void try_sparsebytemap_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(false, 8, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}
void try_sparseshuffledbytemap_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    sparsebytemap(true, 8, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_2bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    NByteMapHash * nbm = new NByteMapHash(2);
    hash_functions.push_back(nbm);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("2bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("2bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_4bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    NByteMapHash * nbm = new NByteMapHash(4);
    hash_functions.push_back(nbm);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("4bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("4bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_tbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    TernaryByteMapHash * tbm = new TernaryByteMapHash();
    hash_functions.push_back(tbm);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("tbm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("tbm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_shuffledtbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ShuffledTernaryByteMapHash * tbm = new ShuffledTernaryByteMapHash();
    hash_functions.push_back(tbm);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("stbm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("stbm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_shuffledbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,2);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();


    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("shuffled bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("shuffled bm lsh %d-bit bit entropy: %f, cr:%f\n", fp_size_in_bits, bit_entropy, cr);
        // hxorCache->print();
        delete hxorCache;

    }

    delete cache;
}


void try_lowentropy_2_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(2, 8, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_2_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(2, 4, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}
void try_lowentropy_2_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(2, 2, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_4_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(4, 4, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_4_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(4, 8, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_8_16(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(8, 16, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_8_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(8, 8, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void try_lowentropy_8_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    lowentropy(8, 4, num_banks, KB_per_bank, dir, fp_size_in_bits, cr, entropy_reduction, false_rate, use_xorcache);
}

void lowentropy(int seg_size, int bits_to_take, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;

    // int seg_size = 4; // take per every seg_size bytes
    // int bits_to_take = 4; // total 64 bit
    int bits_to_take_to_byte = int(ceil(bits_to_take/8.0));
    int funct_to_concact = 0;
    int funct_true_hash = 0;
    for (int i = 0; i < line_size/seg_size; i++){
        int byte_ind = seg_size*i+seg_size-bits_to_take_to_byte;
        for (int j = 0; j < bits_to_take; j++){
            int bit_ind = byte_ind*8+j;
            // printf("i: %d, j: %d, byte_ind:%d, bit_ind: %d\n", i, j, byte_ind, bit_ind);
            BitSelectionHash * bs = new BitSelectionHash(bit_ind);
            // bs->print();
            hash_functions.push_back(bs);
            funct_to_concact++;
            funct_true_hash++;
        }
    }
    hash_functions.push_back(new FullBitShuffleHash()); // with shuffling
    funct_true_hash++;
    hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, false, funct_to_concact, funct_true_hash);

    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();
    // printf("vanilla bit entropy: %f\n", bit_entropy);
    // cache->print();
    unsigned num_unempty_cluster = cache->m_clusters.size();
    double percent_unempty_cluster = (double)num_unempty_cluster/(double)num_clusters*100.0;



    if (use_xorcache) {
        HashXORCache * hxorCache;
        hxorCache = new HashXORCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("lowentropy (%d,%d) %d-bit bit entropy: %f, cr:%f, unempty: %f\n", 
            seg_size, bits_to_take, fp_size_in_bits, bit_entropy, cr, percent_unempty_cluster);
        // hxorCache->print();
        delete hxorCache;
    } else {
        HashDeltaCache * hxorCache;
        hxorCache = new HashDeltaCache(*cache);
        double bit_entropy = hxorCache->get_bit_entropy();

        entropy_reduction = vanilla_bit_entropy - bit_entropy;
        cr = hxorCache->get_compression_ratio();
        false_rate = hxorCache->get_false_positive_rate();

        printf("lowentropy (%d,%d) %d-bit bit entropy: %f, cr:%f, unempty: %f\n", 
            seg_size, bits_to_take, fp_size_in_bits, bit_entropy, cr, percent_unempty_cluster);
        // hxorCache->print();
        delete hxorCache;
    }

    delete cache;
}

void all(int num_banks, int KB_per_bank, string dir, vector <double> &crs, vector <double> &ers, vector <double> &frs, vector<double> fbs, bool use_xorcache,
    void (*func)(int, int, string, int, double &, double &, double &, bool))
{
    double cr, entropy_reduction;
    double fr;

    for (unsigned i = 0; i < fbs.size(); i++){
        func(num_banks, KB_per_bank, dir, fbs[i], cr, entropy_reduction, fr, use_xorcache);
        crs.push_back(cr);
        ers.push_back(entropy_reduction);
        frs.push_back(fr);
    }
}