#include "function/profiling.hh"
#include "cache/xorCache.hh"
#include "common/file/file_read.hh"

void profiling_entropy_byte_position(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    (void)only_those_xored;
    (void) seed;
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    vector<double> * ent_vec;
    ent_vec = cache->get_per_byte_entropy();
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }
    delete ent_vec;
    delete cache;
}

void profiling_entropy_byte_position_afterxor12_bytemap(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    int fp_size_in_bits = 12;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ByteMapHash * bm = new ByteMapHash();
    hash_functions.push_back(bm);
    FullBitShuffleHash * fbs = new FullBitShuffleHash(seed);
    hash_functions.push_back(fbs);
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,2);

    cache->populate_lines(filenames_data, filenames_addr);

    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);

    vector<double> * ent_vec;
    if (only_those_xored) {
        ent_vec = hxorCache->get_per_byte_entropy_only_thoses_xored();
    } else {
        ent_vec = hxorCache->get_per_byte_entropy();
    }
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }

    delete ent_vec;
    delete hxorCache;
    delete cache;
}

void profiling_entropy_byte_position_afterxor12_sparseshuffledbytemap_4_3(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    int fp_size_in_bits = 12;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;

    
    int true_hash = 0;
    int funct_to_concact = 0;
    bool cascade = true;
    int everyNByte = 4;
    int bytes_to_take = 3;

    SparseByteMapHash * bm = new SparseByteMapHash(everyNByte, bytes_to_take);
    hash_functions.push_back(bm);
    true_hash += 1;
    FullBitShuffleHash * fbs = new FullBitShuffleHash(seed);
    hash_functions.push_back(fbs);
    true_hash += 1;
    XORFoldingHash * x = new XORFoldingHash(fp_size_in_bits);
    hash_functions.push_back(x);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, cascade, funct_to_concact, true_hash);


    cache->populate_lines(filenames_data, filenames_addr);

    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);

    vector<double> * ent_vec;
    if (only_those_xored) {
        ent_vec = hxorCache->get_per_byte_entropy_only_thoses_xored();
    } else {
        ent_vec = hxorCache->get_per_byte_entropy();
    }
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }

    delete ent_vec;
    delete hxorCache;
    delete cache;
}

void profiling_entropy_byte_position_afterxor_randbank(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    (void)seed; //TODO: use seed
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    // int fp_size_in_bits = 12;
    // u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    RandBankXORCache * xorRandBankCache;
    xorRandBankCache = new RandBankXORCache(*cache);

    vector<double> * ent_vec;
    if (only_those_xored) {
        ent_vec = xorRandBankCache->get_per_byte_entropy_only_thoses_xored();
    } else {
        ent_vec = xorRandBankCache->get_per_byte_entropy();
    }
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }

    delete ent_vec;
    delete xorRandBankCache;
    delete cache;
}

void profiling_entropy_byte_position_afterxor12_thesaurus(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    int fp_size_in_bits = 12;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;
    ThesaurusLSHash * the = new ThesaurusLSHash(fp_size_in_bits, line_size, seed);
    hash_functions.push_back(the);



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions,1);

    cache->populate_lines(filenames_data, filenames_addr);

    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);

    vector<double> * ent_vec;
    if (only_those_xored) {
        ent_vec = hxorCache->get_per_byte_entropy_only_thoses_xored();
    } else {
        ent_vec = hxorCache->get_per_byte_entropy();
    }

    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }

    delete ent_vec;
    delete hxorCache;
    delete cache;
}

void profiling_entropy_byte_position_afterxor12_lowentropy_8_16(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;
    int fp_size_in_bits = 12;
    u_int64_t num_clusters = (u_int64_t)pow((u_int64_t)2,(u_int64_t)fp_size_in_bits);

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    ClusteredCache * cache;

    vector<HashFunction *> hash_functions;

    int seg_size = 8; // take per every seg_size bytes
    int bits_to_take = 16; // total 64 bit
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
    hash_functions.push_back(new FullBitShuffleHash(seed)); // with shuffling
    funct_true_hash++;
    hash_functions.push_back(new XORFoldingHash(fp_size_in_bits));



    cache = new ClusteredCache(
        num_banks, KB_per_bank, assoc, line_size, 
        shift_bank, shift_set, 
        num_clusters, hash_functions, false, funct_to_concact, funct_true_hash);


    cache->populate_lines(filenames_data, filenames_addr);

    HashXORCache * hxorCache;
    hxorCache = new HashXORCache(*cache);

    vector<double> * ent_vec;
    if (only_those_xored) {
        ent_vec = hxorCache->get_per_byte_entropy_only_thoses_xored();
    } else {
        ent_vec = hxorCache->get_per_byte_entropy();
    }
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }

    delete ent_vec;
    delete hxorCache;
    delete cache;
}

void profiling_histogram_word_pattern_epc(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    EPCWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new EPCWordPatternCacheProfiler(cache);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}
void profiling_histogram_word_pattern_strong(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    StrongWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new StrongWordPatternCacheProfiler(cache);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}
void profiling_histogram_word_pattern_hycomp(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    HyCompWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new HyCompWordPatternCacheProfiler(cache);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}
void profiling_histogram_word_pattern_semantic(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    SemanticWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new SemanticWordPatternCacheProfiler(cache);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}


void profiling_histogram_word_pattern_density(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    DensityWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new DensityWordPatternCacheProfiler(cache);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}

void profiling_histogram_word_pattern_averagebytemsb_32(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    AverageByteMSBWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new AverageByteMSBWordPatternCacheProfiler(cache, 4);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}
void profiling_histogram_word_pattern_averagebytemsb_24(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    AverageByteMSBWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new AverageByteMSBWordPatternCacheProfiler(cache, 3);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}
void profiling_histogram_word_pattern_averagebytemsb_16(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed)
{
    (void)only_those_xored;
    (void)seed;

    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    AverageByteMSBWordPatternCacheProfiler * cache_profiler;
    cache_profiler = new AverageByteMSBWordPatternCacheProfiler(cache, 2);
    vector<int> hist = cache_profiler->profile_cache_word_pattern();

    for (unsigned i = 0; i < hist.size(); i++){
        histogram.push_back(hist[i]);
    }
    delete cache_profiler;
    hist.clear();
    delete cache;
}



void profiling_x(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &results, unsigned seed,
    void(*profiling_function)(int, int, string, bool, vector <double> &, unsigned))
{
    profiling_function(num_banks, KB_per_bank, dir, only_those_xored, results, seed);
}