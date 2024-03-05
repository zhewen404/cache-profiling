#include "function/vanila.hh"
#include "cache/cache.hh"
#include "cache/compressedCache.hh"
#include "common/file/file_read.hh"

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
