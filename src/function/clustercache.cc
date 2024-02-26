#include "function/clustercache.hh"
#include "cache/xorCache.hh"
#include "common/file/file_read.hh"
#include <stdio.h>
#include <assert.h>

ClusteredCache * create_clustered_cache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set,
    int fp_size_in_bits,
    bool cascade, int funct_to_concact, int funct_true_hash,
    string dir, vector<HashFunction *> &hash_functions)
{
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, cascade, funct_to_concact, funct_true_hash);

    cache->populate_lines(filenames_data, filenames_addr);
    return cache;
}

HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(ClusteredCache * cache)
{
    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);
    return hxorCache;
}
HashDeltaCache * create_hashed_inter_cache_from_clustered_cache_delta(ClusteredCache * cache)
{
    HashDeltaCache * hxorCache;
    hxorCache = new HashDeltaCache(*cache);
    return hxorCache;
}

BDICompressedXORCache * create_bdicompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache, bool use_little_e, bool allow_immo)
{
    BDICompressedXORCache * bdiXorCache;
    bdiXorCache = new BDICompressedXORCache(*hxorCache, use_little_e, allow_immo);
    return bdiXorCache;
}
BPCCompressedXORCache * create_bpccompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache)
{
    BPCCompressedXORCache * bpcXorCache;
    bpcXorCache = new BPCCompressedXORCache(*hxorCache);
    return bpcXorCache;
}
////////////////////////////////////////////////////////////////

void create_hashfunctions_bbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true);
    hash_functions.push_back(bbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_fbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_thesaurus(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size);
    hash_functions.push_back(the);
    true_hash += 1;
}

void create_hashfunctions_bs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    BitSamplingLSHash * bs = new BitSamplingLSHash(fp_size_in_bits);
    hash_functions.push_back(bs);
    true_hash += 1;
}

void maskedbs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size,
    int everyNbyte, int bits_to_take)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    MaskedBitSamplingLSHash * bs = new MaskedBitSamplingLSHash(fp_size_in_bits, everyNbyte, bits_to_take);
    hash_functions.push_back(bs);
    true_hash += 1;
}
void create_hashfunctions_maskedbs_8_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 32);
}
void create_hashfunctions_maskedbs_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 16);
}
void create_hashfunctions_maskedbs_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 8);
}
void create_hashfunctions_maskedbs_4_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 16);
}
void create_hashfunctions_maskedbs_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 8);
}

void create_hashfunctions_bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void sparsebytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size,
    bool shuffle, int everyNByte, int bytes_to_take)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    SparseByteMapHash * bm = new SparseByteMapHash(everyNByte, bytes_to_take);
    hash_functions.push_back(bm);
    true_hash += 1;
    if (shuffle) {
        FullBitShuffleHash * fbs = new FullBitShuffleHash();
        hash_functions.push_back(fbs);
        true_hash += 1;
    }
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);

}
void create_hashfunctions_sparsebytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 2, 1);
}
void create_hashfunctions_sparseshuffledbytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 1);
}
void create_hashfunctions_sparsebytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 4, 1);
}
void create_hashfunctions_sparseshuffledbytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 1);
}
void create_hashfunctions_sparsebytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 8, 1);
}
void create_hashfunctions_sparseshuffledbytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 1);
}
void create_hashfunctions_sparseshuffledbytemap_8_7(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 7);
}
void create_hashfunctions_sparseshuffledbytemap_8_6(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 6);
}
void create_hashfunctions_sparseshuffledbytemap_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 4);
}
void create_hashfunctions_sparseshuffledbytemap_4_3(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 3);
}
void create_hashfunctions_sparseshuffledbytemap_4_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 2);
}

void create_hashfunctions_2bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    NByteMapHash * nbm = new NByteMapHash(2);
    hash_functions.push_back(nbm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}
void create_hashfunctions_4bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    NByteMapHash * nbm = new NByteMapHash(4);
    hash_functions.push_back(nbm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_tbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    TernaryByteMapHash * tbm = new TernaryByteMapHash();
    hash_functions.push_back(tbm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}
void create_hashfunctions_shuffledtbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ShuffledTernaryByteMapHash * tbm = new ShuffledTernaryByteMapHash();
    hash_functions.push_back(tbm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);

}


void create_hashfunctions_shuffledbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    true_hash += 1;
    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_shuffledmaxbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    MaxByteMapHash * bm = new MaxByteMapHash();
    hash_functions.push_back(bm);
    true_hash += 1;
    FullBitShuffleHash * fbs = new FullBitShuffleHash();
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void lowentropy(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size,
    int seg_size, int bits_to_take)
{
    true_hash = 0;
    funct_to_concact = 0;
    cascade = false;

    int bits_to_take_to_byte = int(ceil(bits_to_take/8.0));
    for (int i = 0; i < line_size/seg_size; i++){
        int byte_ind = seg_size*i+seg_size-bits_to_take_to_byte;
        for (int j = 0; j < bits_to_take; j++){
            int bit_ind = byte_ind*8+j;
            // printf("i: %d, j: %d, byte_ind:%d, bit_ind: %d\n", i, j, byte_ind, bit_ind);
            BitSelectionHash * bs = new BitSelectionHash(bit_ind);
            // bs->print();
            hash_functions.push_back(bs);
            funct_to_concact++;
            true_hash++;
        }
    }
    hash_functions.push_back(new FullBitShuffleHash()); // with shuffling
    true_hash++;
    hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));
}

void create_hashfunctions_lowentropy_2_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 8);
}
void create_hashfunctions_lowentropy_2_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 4);
}
void create_hashfunctions_lowentropy_2_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 2);
}

void create_hashfunctions_lowentropy_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 8);
}
void create_hashfunctions_lowentropy_4_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 4);
}


void create_hashfunctions_lowentropy_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 16);
}
void create_hashfunctions_lowentropy_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 8);
}
void create_hashfunctions_lowentropy_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 4);
}

////////////////////////////////////////////////////////////////
void create_vanila_bdi(int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    cr = 0;
    entropy_reduction = 0;
    false_rate = 0;
    hamming = 0;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    BDICompressedCache * bdiCache;
    bdiCache = new BDICompressedCache(*cache, use_little_endian, allow_immo);

    double intra_cr = bdiCache->get_compression_ratio();
    intra_compression_ratio = intra_cr;

    delete cache;
    delete bdiCache;
}


void create_vanila_bpc(int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    (void)use_little_endian;
    (void)allow_immo;

    
    cr = 0;
    entropy_reduction = 0;
    false_rate = 0;
    hamming = 0;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    BPCCompressedCache * bpcCache;
    bpcCache = new BPCCompressedCache(*cache);

    double intra_cr = bpcCache->get_compression_ratio();
    intra_compression_ratio = intra_cr;

    delete cache;
    delete bpcCache;
}



////////////////////////////////////////////////////////////////
// for hash functions
void map_all(int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int))
{
    double cr, entropy_reduction;
    double fr;
    double intra;
    double hamming;

    for (unsigned i = 0; i < fbs.size(); i++){
        map_x(num_banks, KB_per_bank, dir, fbs[i], cr, entropy_reduction, fr, intra, hamming, use_xorcache, use_little_e, allow_immo, type, create_hash_functions_x);
        crs.push_back(cr);
        ers.push_back(entropy_reduction);
        frs.push_back(fr);
        intras.push_back(intra);
        hammings.push_back(hamming);
    }
}

// for hash functions
void map_x(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming, 
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int)
    )
{
    assert(use_xorcache);

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    
    int true_hash = 0;
    int funct_to_concact = 0;
    bool cascade = true;
    vector<HashFunction *> hash_functions;
    create_hash_functions_x(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size);

    ClusteredCache *cache = create_clustered_cache(
        num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set, fp_size_in_bits, 
        cascade, funct_to_concact, true_hash, dir, hash_functions);

    double vanilla_bit_entropy = cache->get_bit_entropy();


    HashXORCache * hxorCache = create_hashed_inter_cache_from_clustered_cache_xor(cache);
    
    double bit_entropy = hxorCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;
    cr = hxorCache->get_compression_ratio();
    false_rate = hxorCache->get_false_positive_rate();
    hamming = hxorCache->get_hamming_distance();

    if (type == BDI) {
        BDICompressedXORCache * bdiXorCache = create_bdicompressedxorcache_from_hashedxorcache(hxorCache, use_little_e, allow_immo);
        intra_compression_ratio = bdiXorCache->get_intra_compression_ratio();
        delete bdiXorCache;
    } else if (type == BPC) {
        BPCCompressedXORCache * bpcXorCache = create_bpccompressedxorcache_from_hashedxorcache(hxorCache);
        intra_compression_ratio = bpcXorCache->get_intra_compression_ratio();
        delete bpcXorCache;
    } else {
        assert(false);
    }
    delete hxorCache;
    delete cache;
}

////////////////////////////////////////////////////////////////
// for vanila intra compressed cache
void vanila_x(int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_vanila_x)(int, int, string, double &, double &, double &, double &, double &, bool, bool))
{
    (void) fbs;
    (void) use_xorcache;
    double cr, entropy_reduction;
    double fr;
    double intra;
    double hamming;

    create_vanila_x(num_banks, KB_per_bank, dir, cr, entropy_reduction, fr, intra, hamming, use_little_e, allow_immo);
    crs.push_back(cr);
    ers.push_back(entropy_reduction);
    frs.push_back(fr);
    intras.push_back(intra);
    hammings.push_back(hamming);
}