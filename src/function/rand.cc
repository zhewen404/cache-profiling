#include "function/rand.hh"
#include "cache/cache.hh"
#include "cache/xorCache.hh"
#include "cache/compressedCache.hh"
#include "common/file/file_read.hh"
#include "common/vector/vector.hh"
#include <assert.h>

void create_rand_bank_bdi(unsigned seed, int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    false_rate = 0;

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
    double vanilla_bit_entropy = cache->get_bit_entropy();

    RandBankXORCache * xorRandBankCache;
    xorRandBankCache = new RandBankXORCache(*cache, seed);

    BDICompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BDICompressedXORCache(*xorRandBankCache, use_little_endian, allow_immo);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorRandBankCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorRandBankCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorRandBankCache;
    delete xorCompressedCache;
}


void create_rand_set_bdi(unsigned seed, int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    false_rate = 0;

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
    double vanilla_bit_entropy = cache->get_bit_entropy();

    RandSetXORCache * xorRandSetCache;
    xorRandSetCache = new RandSetXORCache(*cache, seed);

    BDICompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BDICompressedXORCache(*xorRandSetCache, use_little_endian, allow_immo);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorRandSetCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorRandSetCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorRandSetCache;
    delete xorCompressedCache;
}


// for vanila intra compressed cache
void rand_x(vector<unsigned> defined_seeds, int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, 
    vector <double> &crs_max, vector <double> &ers_max, vector <double> &frs_max, vector <double> &intras_max, vector <double> &hammings_max, 
    vector <double> &crs_min, vector <double> &ers_min, vector <double> &frs_min, vector <double> &intras_min, vector <double> &hammings_min, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_rand_x)(unsigned, int, int, string, double &, double &, double &, double &, double &, bool, bool))
{
    (void) fbs;
    (void) use_xorcache;
    double cr, entropy_reduction;
    double fr;
    double intra;
    double hamming;
    vector<double> crs_temp, ers_temp, frs_temp, intras_temp, hammings_temp;

    for (unsigned j = 0; j < defined_seeds.size(); j++) {
        unsigned defined_seed = defined_seeds[j];
        create_rand_x(defined_seed, num_banks, KB_per_bank, dir, cr, entropy_reduction, fr, intra, hamming, use_little_e, allow_immo);
        
        assert(cr > 0); //gmean
        assert(fr >= 0); //amean
        assert(intra > 0); //gmean
        assert(hamming >= 0); //amean

        crs_temp.push_back(cr);
        ers_temp.push_back(entropy_reduction);
        frs_temp.push_back(fr);
        intras_temp.push_back(intra);
        hammings_temp.push_back(hamming);
    }

    crs.push_back(calculateGeoMean(crs_temp));
    ers.push_back(calculateMean(ers_temp));
    frs.push_back(calculateMean(frs_temp));
    intras.push_back(calculateGeoMean(intras_temp));
    hammings.push_back(calculateMean(hammings_temp));

    crs_max.push_back(*max_element(crs_temp.begin(), crs_temp.end()));
    ers_max.push_back(*max_element(ers_temp.begin(), ers_temp.end()));
    frs_max.push_back(*max_element(frs_temp.begin(), frs_temp.end()));
    intras_max.push_back(*max_element(intras_temp.begin(), intras_temp.end()));
    hammings_max.push_back(*max_element(hammings_temp.begin(), hammings_temp.end()));

    crs_min.push_back(*min_element(crs_temp.begin(), crs_temp.end()));
    ers_min.push_back(*min_element(ers_temp.begin(), ers_temp.end()));
    frs_min.push_back(*min_element(frs_temp.begin(), frs_temp.end()));
    intras_min.push_back(*min_element(intras_temp.begin(), intras_temp.end()));
    hammings_min.push_back(*min_element(hammings_temp.begin(), hammings_temp.end()));
    
    crs_temp.clear();
    ers_temp.clear();
    frs_temp.clear();
    intras_temp.clear();
    hammings_temp.clear();
}
