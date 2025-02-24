#include "function/clustercache.hh"
#include "cache/xorCache.hh"
#include "common/file/file_read.hh"
#include "common/vector/vector.hh"
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
BankedClusteredCache * create_bankedclustered_cache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set,
    int fp_size_in_bits,
    bool cascade, int funct_to_concact, int funct_true_hash,
    string dir, vector<HashFunction *> &hash_functions)
{
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    BankedClusteredCache * cache = new BankedClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, cascade, funct_to_concact, funct_true_hash);

    cache->populate_lines(filenames_data, filenames_addr);
    return cache;
}

HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(ClusteredCache * cache, unsigned defined_seed)
{
    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache, defined_seed);
    return hxorCache;
}
HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(BankedClusteredCache * cache, unsigned defined_seed)
{
    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache, defined_seed);
    return hxorCache;
}
HashDeltaCache * create_hashed_inter_cache_from_clustered_cache_delta(ClusteredCache * cache, unsigned defined_seed)
{
    HashDeltaCache * hxorCache;
    hxorCache = new HashDeltaCache(*cache, defined_seed);
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
ThesaurusCompressedXORCache * create_thesauruscompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache, unsigned defined_seed)
{
    ThesaurusCompressedXORCache * theXorCache;
    theXorCache = new ThesaurusCompressedXORCache(*hxorCache, defined_seed);
    return theXorCache;
}
////////////////////////////////////////////////////////////////

void create_hashfunctions_bbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ByteBitShuffleHash * bbs = new ByteBitShuffleHash(true, defined_seed);
    hash_functions.push_back(bbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_fbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    FullBitShuffleHash * fbs = new FullBitShuffleHash(defined_seed);
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_thesaurus(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size, defined_seed);
    hash_functions.push_back(the);
    true_hash += 1;
}

void create_hashfunctions_bs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    BitSamplingLSHash * bs = new BitSamplingLSHash(fp_size_in_bits, defined_seed);
    hash_functions.push_back(bs);
    true_hash += 1;
}

void maskedbs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size,
    int everyNbyte, int bits_to_take, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    MaskedBitSamplingLSHash * bs = new MaskedBitSamplingLSHash(fp_size_in_bits, everyNbyte, bits_to_take, defined_seed);
    hash_functions.push_back(bs);
    true_hash += 1;
}
void create_hashfunctions_maskedbs_8_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 32, defined_seed);
}
void create_hashfunctions_maskedbs_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 16, defined_seed);
}
void create_hashfunctions_maskedbs_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 8, defined_seed);
}
void create_hashfunctions_maskedbs_4_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 16, defined_seed);
}
void create_hashfunctions_maskedbs_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    maskedbs(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 8, defined_seed);
}

void create_hashfunctions_bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) defined_seed;
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
    bool shuffle, int everyNByte, int bytes_to_take, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    SparseByteMapHash * bm = new SparseByteMapHash(everyNByte, bytes_to_take);
    hash_functions.push_back(bm);
    true_hash += 1;
    if (shuffle) {
        FullBitShuffleHash * fbs = new FullBitShuffleHash(defined_seed);
        hash_functions.push_back(fbs);
        true_hash += 1;
    }
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);

}
void create_hashfunctions_sparsebytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 2, 1, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 1, defined_seed);
}
void create_hashfunctions_sparsebytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 4, 1, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 1, defined_seed);
}
void create_hashfunctions_sparsebytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, false, 8, 1, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 1, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_8_7(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 7, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_8_6(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 6, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 8, 4, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_4_3(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 3, defined_seed);
}
void create_hashfunctions_sparseshuffledbytemap_4_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    sparsebytemap(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, true, 4, 2, defined_seed);
}

void create_hashfunctions_2bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) defined_seed;
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
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) defined_seed;
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
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) defined_seed;
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
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ShuffledTernaryByteMapHash * tbm = new ShuffledTernaryByteMapHash(defined_seed);
    hash_functions.push_back(tbm);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);

}


void create_hashfunctions_shuffledbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    true_hash += 1;
    FullBitShuffleHash * fbs = new FullBitShuffleHash(defined_seed);
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void create_hashfunctions_shuffledmaxbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    MaxByteMapHash * bm = new MaxByteMapHash();
    hash_functions.push_back(bm);
    true_hash += 1;
    FullBitShuffleHash * fbs = new FullBitShuffleHash(defined_seed);
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);
}

void lowentropy(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size,
    int seg_size, int bits_to_take, unsigned defined_seed)
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
    hash_functions.push_back(new FullBitShuffleHash(defined_seed)); // with shuffling
    true_hash++;
    hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));
}

void create_hashfunctions_lowentropy_2_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 8, defined_seed);
}
void create_hashfunctions_lowentropy_2_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 4, defined_seed);
}
void create_hashfunctions_lowentropy_2_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 2, 2, defined_seed);
}

void create_hashfunctions_lowentropy_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 8, defined_seed);
}
void create_hashfunctions_lowentropy_4_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 4, 4, defined_seed);
}


void create_hashfunctions_lowentropy_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 16, defined_seed);
}
void create_hashfunctions_lowentropy_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 8, defined_seed);
}
void create_hashfunctions_lowentropy_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    lowentropy(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, 8, 4, defined_seed);
}

void create_hashfunctions_epc_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    EPCWordLabelingHash * epc = new EPCWordLabelingHash();
    hash_functions.push_back(epc);
    true_hash += 1;
}
void create_hashfunctions_strong_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    StrongWordLabelingHash * st = new StrongWordLabelingHash();
    hash_functions.push_back(st);
    true_hash += 1;
}
void create_hashfunctions_hycomp_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    HyCompWordLabelingHash * hycomp = new HyCompWordLabelingHash();
    hash_functions.push_back(hycomp);
    true_hash += 1;
}
void create_hashfunctions_semantic_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    SemanticWordLabelingHash * se = new SemanticWordLabelingHash();
    hash_functions.push_back(se);
    true_hash += 1;
}
void create_hashfunctions_density_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    DensityWordLabelingHash * se = new DensityWordLabelingHash();
    hash_functions.push_back(se);
    true_hash += 1;
}

void create_hashfunctions_averagebytemsb_word_labeling_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    AverageByteMSBWordLabelingHash * se = new AverageByteMSBWordLabelingHash(4);
    hash_functions.push_back(se);
    true_hash += 1;
}
void create_hashfunctions_averagebytemsb_word_labeling_24(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    AverageByteMSBWordLabelingHash * se = new AverageByteMSBWordLabelingHash(3);
    hash_functions.push_back(se);
    true_hash += 1;
}
void create_hashfunctions_averagebytemsb_word_labeling_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{
    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    AverageByteMSBWordLabelingHash * se = new AverageByteMSBWordLabelingHash(2);
    hash_functions.push_back(se);
    true_hash += 1;
}

//////////////////// line labeling
void create_hashfunctions_bdi_line_labeling_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{

    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    BDILineLabelingHash * bdi_ll = new BDILineLabelingHash(true, false);
    hash_functions.push_back(bdi_ll);
    true_hash += 1;
}
void create_hashfunctions_bdi_immo_line_labeling_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{

    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    BDILineLabelingHash * bdi_ll = new BDILineLabelingHash(true, true);
    hash_functions.push_back(bdi_ll);
    true_hash += 1;
}
void create_hashfunctions_bpc_line_labeling_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, unsigned defined_seed)
{

    (void) line_size;
    (void) fp_size_in_bits;
    (void) defined_seed;
    true_hash = 0;
    funct_to_concact = 0;
    cascade = true;

    BPCLineLabelingHash * bpc_ll = new BPCLineLabelingHash();
    hash_functions.push_back(bpc_ll);
    true_hash += 1;
}
////////////////////////////////////////////////////////////////
// for hash functions
void map_all(bool banked, vector<unsigned> defined_seeds, 
    int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, vector <double> &centroid, vector <double> &noncentroid_success, vector <double> &noncentroid_fail, 
    vector <double> &crs_max, vector <double> &ers_max, vector <double> &frs_max, vector <double> &intras_max, vector <double> &hammings_max, vector <double> &centroid_max, vector <double> &noncentroid_success_max, vector <double> &noncentroid_fail_max,
    vector <double> &crs_min, vector <double> &ers_min, vector <double> &frs_min, vector <double> &intras_min, vector <double> &hammings_min, vector <double> &centroid_min, vector <double> &noncentroid_success_min, vector <double> &noncentroid_fail_min,
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned))
{
    vector<double> crs_temp, ers_temp, frs_temp, intras_temp, hammings_temp, centroid_temp, noncentroid_success_temp, noncentroid_fail_temp;

    for (unsigned i = 0; i < fbs.size(); i++) {
        double cr, entropy_reduction;
        double fr;
        double intra;
        double hamming;
        double centroid_; 
        double noncentroid_success_;
        double noncentroid_fail_;
        for (unsigned j = 0; j < defined_seeds.size(); j++) {
            unsigned defined_seed = defined_seeds[j];
            if (banked) {
                map_x_banked(defined_seed, shift_bank, shift_set, num_banks, KB_per_bank, dir, fbs[i], cr, entropy_reduction, fr, intra, hamming, centroid_, noncentroid_success_, noncentroid_fail_, use_xorcache, use_little_e, allow_immo, type, create_hash_functions_x);
            } else {
                map_x_flat(defined_seed, shift_bank, shift_set, num_banks, KB_per_bank, dir, fbs[i], cr, entropy_reduction, fr, intra, hamming, centroid_, noncentroid_success_, noncentroid_fail_, use_xorcache, use_little_e, allow_immo, type, create_hash_functions_x);
            }
            
            assert(cr > 0); // gmean
            // assert(entropy_reduction >= 0); // amean
            assert(fr >= 0); // amean
            assert(intra > 0); // gmean
            assert(hamming >= 0); // amean

            crs_temp.push_back(cr);
            ers_temp.push_back(entropy_reduction);
            frs_temp.push_back(fr);
            intras_temp.push_back(intra);
            hammings_temp.push_back(hamming);
            centroid_temp.push_back(centroid_);
            noncentroid_success_temp.push_back(noncentroid_success_);
            noncentroid_fail_temp.push_back(noncentroid_fail_);
        }
        // printf("fb: %f, crs_temp:", fbs[i]);
        // for (unsigned j = 0; j < crs_temp.size(); j++) {
        //     printf(" %f", crs_temp[j]);
        // }
        // printf("\n");
        
        crs.push_back(calculateGeoMean(crs_temp));
        ers.push_back(calculateMean(ers_temp));
        frs.push_back(calculateMean(frs_temp));
        intras.push_back(calculateGeoMean(intras_temp));
        hammings.push_back(calculateMean(hammings_temp));
        centroid.push_back(calculateMean(centroid_temp));
        noncentroid_success.push_back(calculateMean(noncentroid_success_temp));
        noncentroid_fail.push_back(calculateMean(noncentroid_fail_temp));

        crs_max.push_back(*max_element(crs_temp.begin(), crs_temp.end()));
        ers_max.push_back(*max_element(ers_temp.begin(), ers_temp.end()));
        frs_max.push_back(*max_element(frs_temp.begin(), frs_temp.end()));
        intras_max.push_back(*max_element(intras_temp.begin(), intras_temp.end()));
        hammings_max.push_back(*max_element(hammings_temp.begin(), hammings_temp.end()));
        centroid_max.push_back(*max_element(centroid_temp.begin(), centroid_temp.end()));
        noncentroid_success_max.push_back(*max_element(noncentroid_success_temp.begin(), noncentroid_success_temp.end()));
        noncentroid_fail_max.push_back(*max_element(noncentroid_fail_temp.begin(), noncentroid_fail_temp.end()));

        crs_min.push_back(*min_element(crs_temp.begin(), crs_temp.end()));
        ers_min.push_back(*min_element(ers_temp.begin(), ers_temp.end()));
        frs_min.push_back(*min_element(frs_temp.begin(), frs_temp.end()));
        intras_min.push_back(*min_element(intras_temp.begin(), intras_temp.end()));
        hammings_min.push_back(*min_element(hammings_temp.begin(), hammings_temp.end()));
        centroid_min.push_back(*min_element(centroid_temp.begin(), centroid_temp.end()));
        noncentroid_success_min.push_back(*min_element(noncentroid_success_temp.begin(), noncentroid_success_temp.end()));
        noncentroid_fail_min.push_back(*min_element(noncentroid_fail_temp.begin(), noncentroid_fail_temp.end()));
        
        crs_temp.clear();
        ers_temp.clear();
        frs_temp.clear();
        intras_temp.clear();
        hammings_temp.clear();
        centroid_temp.clear();
        noncentroid_success_temp.clear();
        noncentroid_fail_temp.clear();

    }
}

// for hash functions
void map_x_flat(unsigned defined_seed, 
    int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming, 
    double &centroid, double &noncentroid_success, double &noncentroid_fail, 
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned)
    )
{
    assert(use_xorcache);

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    
    int true_hash = 0;
    int funct_to_concact = 0;
    bool cascade = true;
    vector<HashFunction *> hash_functions;
    create_hash_functions_x(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, defined_seed);

    ClusteredCache *cache = create_clustered_cache(
        num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set, fp_size_in_bits, 
        cascade, funct_to_concact, true_hash, dir, hash_functions);

    double vanilla_bit_entropy = cache->get_bit_entropy();


    HashXORCache * hxorCache = create_hashed_inter_cache_from_clustered_cache_xor(cache, defined_seed);
    
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
    
    centroid = 0;
    noncentroid_success = 0;
    noncentroid_fail = 0;

    delete hxorCache;
    delete cache;
}

void map_x_banked(unsigned defined_seed, 
    int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming, 
    double &centroid, double &noncentroid_success, double &noncentroid_fail, 
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned)
    )
{
    assert(use_xorcache);

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    
    int true_hash = 0;
    int funct_to_concact = 0;
    bool cascade = true;
    vector<HashFunction *> hash_functions;
    create_hash_functions_x(hash_functions, true_hash, cascade, funct_to_concact, fp_size_in_bits, line_size, defined_seed);

    BankedClusteredCache *cache = create_bankedclustered_cache(
        num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set, fp_size_in_bits, 
        cascade, funct_to_concact, true_hash, dir, hash_functions);

    double vanilla_bit_entropy = cache->get_bit_entropy();


    HashXORCache * hxorCache = create_hashed_inter_cache_from_clustered_cache_xor(cache, defined_seed);
    
    double bit_entropy = hxorCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;
    cr = hxorCache->get_compression_ratio();
    false_rate = hxorCache->get_false_positive_rate();
    hamming = hxorCache->get_hamming_distance();

    if (type == BDI) {
        BDICompressedXORCache * bdiXorCache = create_bdicompressedxorcache_from_hashedxorcache(hxorCache, use_little_e, allow_immo);
        intra_compression_ratio = bdiXorCache->get_intra_compression_ratio();
        delete bdiXorCache;
        centroid = 0;
        noncentroid_success = 0;
        noncentroid_fail = 0;
    } else if (type == BPC) {
        BPCCompressedXORCache * bpcXorCache = create_bpccompressedxorcache_from_hashedxorcache(hxorCache);
        intra_compression_ratio = bpcXorCache->get_intra_compression_ratio();
        delete bpcXorCache;
        centroid = 0;
        noncentroid_success = 0;
        noncentroid_fail = 0;
    } else if (type == THESAURUS) {
        ThesaurusCompressedXORCache * thesaurusXorCache = create_thesauruscompressedxorcache_from_hashedxorcache(hxorCache, defined_seed);
        intra_compression_ratio = thesaurusXorCache->get_intra_compression_ratio();
        centroid = thesaurusXorCache->get_centroid_size();
        noncentroid_success = thesaurusXorCache->get_noncentroid_success_size();
        noncentroid_fail = thesaurusXorCache->get_noncentroid_fail_size();
        delete thesaurusXorCache;
    } else {
        assert(false);
    }
    delete hxorCache;
    delete cache;
}
