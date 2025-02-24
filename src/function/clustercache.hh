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
    THESAURUS,
}intracomp_t;

ClusteredCache * create_clustered_cache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set,
    int fp_size_in_bits, bool cascade, int funct_to_concact, int funct_true_hash, string dir, vector<HashFunction *> &hash_functions);

BankedClusteredCache * create_bankedclustered_cache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set,
    int fp_size_in_bits, bool cascade, int funct_to_concact, int funct_true_hash, string dir, vector<HashFunction *> &hash_functions);

HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(ClusteredCache * cache, unsigned defined_seed);
HashXORCache * create_hashed_inter_cache_from_clustered_cache_xor(BankedClusteredCache * cache, unsigned defined_seed);
HashDeltaCache * create_hashed_inter_cache_from_clustered_cache_delta(ClusteredCache * cache, unsigned defined_seed);
BDICompressedXORCache * create_bdicompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache, bool use_little_e, bool allow_immo);
BPCCompressedXORCache * create_bpccompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache);
ThesaurusCompressedXORCache * create_thesauruscompressedxorcache_from_hashedxorcache(HashXORCache * hxorCache, unsigned defined_seed);

void create_hashfunctions_bbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_fbsxf(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_thesaurus(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_bs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void maskedbs(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact,
    int fp_size_in_bits, int line_size, int everyNbyte, int bits_to_take, unsigned defined_seed);
void create_hashfunctions_maskedbs_8_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_maskedbs_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_maskedbs_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_maskedbs_4_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_maskedbs_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void create_hashfunctions_bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void sparsebytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, bool shuffle, int everyNByte, int bytes_to_take, unsigned defined_seed);
void create_hashfunctions_sparsebytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_2_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparsebytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_4_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparsebytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_8_1(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_8_7(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_8_6(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_4_3(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_sparseshuffledbytemap_4_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_2bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_4bytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_tbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void create_hashfunctions_shuffledtbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_shuffledbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_shuffledmaxbytemap(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void lowentropy(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, int seg_size, int bits_to_take, unsigned defined_seed);
void create_hashfunctions_lowentropy_2_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_2_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_2_2(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_4_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_4_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_8_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_8_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_lowentropy_8_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void create_hashfunctions_epc_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_strong_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_hycomp_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_semantic_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_density_word_labeling(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_averagebytemsb_word_labeling_32(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_averagebytemsb_word_labeling_24(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_averagebytemsb_word_labeling_16(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void create_hashfunctions_bdi_line_labeling_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_bdi_immo_line_labeling_4(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);
void create_hashfunctions_bpc_line_labeling_8(vector<HashFunction *>& hash_functions, int& true_hash, bool& cascade, int& funct_to_concact, int fp_size_in_bits, int line_size, unsigned defined_seed);

void map_all(bool banked, vector<unsigned> defined_seeds, int shift_bank, int shift_set, int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, vector <double> &centroid, vector <double> &noncentroid_success, vector <double> &noncentroid_fail, 
    vector <double> &crs_max, vector <double> &ers_max, vector <double> &frs_max, vector <double> &intras_max, vector <double> &hammings_max, vector <double> &centroid_max, vector <double> &noncentroid_success_max, vector <double> &noncentroid_fail_max,
    vector <double> &crs_min, vector <double> &ers_min, vector <double> &frs_min, vector <double> &intras_min, vector <double> &hammings_min, vector <double> &centroid_min, vector <double> &noncentroid_success_min, vector <double> &noncentroid_fail_min,
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned));
void map_x_flat(unsigned defined_seed, int shift_bank, int shift_set, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    double &centroid, double &noncentroid_success, double &noncentroid_fail, 
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned));
void map_x_banked(unsigned defined_seed, int shift_bank, int shift_set, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    double &centroid, double &noncentroid_success, double &noncentroid_fail, 
    bool use_xorcache, bool use_little_e, bool allow_immo, intracomp_t type,
    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned));

#endif