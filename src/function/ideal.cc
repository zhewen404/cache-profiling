#include "function/ideal.hh"
#include "cache/cache.hh"
#include "cache/compressedCache.hh"
#include "common/file/file_read.hh"
#include "cache/xorCache.hh"

void create_ideal_bank_bdi_shift(int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    false_rate = 0;

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();

    IdealBankXORCache<BDICompressor> * xorIdealBankCache;
    xorIdealBankCache = new IdealBankXORCache<BDICompressor>(*cache, new BDICompressor(use_little_endian, allow_immo));

    BDICompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BDICompressedXORCache(*xorIdealBankCache, use_little_endian, allow_immo);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorIdealBankCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorIdealBankCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorIdealBankCache;
    delete xorCompressedCache;
}

void create_ideal_bank_bpc_shift(int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    (void) use_little_endian;
    (void) allow_immo;
    
    false_rate = 0;

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();

    IdealBankXORCache<BitPlaneCompressor> * xorIdealBankCache;
    xorIdealBankCache = new IdealBankXORCache<BitPlaneCompressor>(*cache, new BitPlaneCompressor());

    BPCCompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BPCCompressedXORCache(*xorIdealBankCache);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorIdealBankCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorIdealBankCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorIdealBankCache;
    delete xorCompressedCache;
}


void create_ideal_set_bdi_shift(int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    false_rate = 0;

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();

    IdealSetXORCache<BDICompressor> * xorIdealSetCache;
    xorIdealSetCache = new IdealSetXORCache<BDICompressor>(*cache, new BDICompressor(use_little_endian, allow_immo));

    BDICompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BDICompressedXORCache(*xorIdealSetCache, use_little_endian, allow_immo);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorIdealSetCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorIdealSetCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorIdealSetCache;
    delete xorCompressedCache;
}


void create_ideal_set_bpc_shift(int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo)
{
    (void) use_little_endian;
    (void) allow_immo;
    
    false_rate = 0;

    int line_size = 64;
    int assoc = 16;
    // int shift_bank = 0;
    // int shift_set = 0;
    // int fp_size_in_bits = 32;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);
    double vanilla_bit_entropy = cache->get_bit_entropy();

    IdealSetXORCache<BitPlaneCompressor> * xorIdealSetCache;
    xorIdealSetCache = new IdealSetXORCache<BitPlaneCompressor>(*cache, new BitPlaneCompressor());

    BPCCompressedXORCache * xorCompressedCache;
    xorCompressedCache = new BPCCompressedXORCache(*xorIdealSetCache);

    double intra_cr = xorCompressedCache->get_intra_compression_ratio();
    intra_compression_ratio = intra_cr;

    double inter = xorCompressedCache->get_inter_compression_ratio();
    cr = inter;

    double hamming_ = xorIdealSetCache->get_hamming_distance();
    hamming = hamming_;


    double bit_entropy = xorIdealSetCache->get_bit_entropy();
    entropy_reduction = vanilla_bit_entropy - bit_entropy;

    delete cache;
    delete xorIdealSetCache;
    delete xorCompressedCache;
}


// for vanila intra compressed cache
void ideal_x(int shift_bank, int shift_set, 
    int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hammings, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_ideal_x)(int, int, int, int, string, double &, double &, double &, double &, double &, bool, bool))
{
    (void) fbs;
    (void) use_xorcache;
    double cr, entropy_reduction;
    double fr;
    double intra;
    double hamming;

    create_ideal_x(shift_bank, shift_set, num_banks, KB_per_bank, dir, cr, entropy_reduction, fr, intra, hamming, use_little_e, allow_immo);
    crs.push_back(cr);
    ers.push_back(entropy_reduction);
    frs.push_back(fr);
    intras.push_back(intra);
    hammings.push_back(hamming);
}
