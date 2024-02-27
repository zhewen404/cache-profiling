#ifndef FUNCTION_CLUSTERCACHE_HH
#define FUNCTION_CLUSTERCACHE_HH
#include "cache/cache.hh"
#include "cache/compressedCache.hh"
#include <vector>
#include <string>
using namespace std;
typedef enum {
    BDI,
    BPC,
}intracomp_t;

ClusteredCache * create_clustered_cache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set,
    int fp_size_in_bits, bool cascade, int funct_to_concact, int funct_true_hash, string dir, vector<HashFunction *> &hash_functions);

HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(ClusteredCache * cache);
HashDeltaCache * create_hashed_inter_cache_from_clustered_cache_delta(ClusteredCache * cache);
BDICompressedXORCache * create_bdicompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache, bool use_little_e, bool allow_immo);
BPCCompressedXORCache * create_bpccompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache);

void create_hashfunctions_bbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_fbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_thesaurus(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_bs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void maskedbs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, int everyNbyte, int bits_to_take);
void create_hashfunctions_maskedbs_8_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_maskedbs_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_maskedbs_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_maskedbs_4_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_maskedbs_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void create_hashfunctions_bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void sparsebytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, bool shuffle, int everyNByte, int bytes_to_take);
void create_hashfunctions_sparsebytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparsebytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparsebytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_8_7(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_8_6(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_4_3(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_sparseshuffledbytemap_4_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_2bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_4bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_tbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void create_hashfunctions_shuffledtbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_shuffledbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_shuffledmaxbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void lowentropy(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, int seg_size, int bits_to_take);
void create_hashfunctions_lowentropy_2_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_2_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_2_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_4_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_lowentropy_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void create_hashfunctions_epc_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);
void create_hashfunctions_strong_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size);

void create_vanila_bdi(int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);
void create_vanila_bpc(int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);

void map_all(int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int));
void map_x(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int));

void vanila_x(int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hamming,
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_vanila_x)(int, int, string, double &, double &, double &, double &, double &, bool, bool));

#endif