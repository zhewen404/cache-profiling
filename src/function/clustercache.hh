#ifndef FUNCTION_CLUSTERCACHE_HH
#define FUNCTION_CLUSTERCACHE_HH

#include <vector>
#include <string>
using namespace std;

void try_bbsxf(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_fbsxf(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_thesaurus(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_bs(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);

void sparsebytemap(int everyNByte, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparsebytemap_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparseshuffledbytemap_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparsebytemap_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparseshuffledbytemap_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparsebytemap_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_sparseshuffledbytemap_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);

void try_2bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_4bytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_tbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_shuffledtbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_shuffledbytemap(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);

void try_lowentropy_2_2(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_2_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_2_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_4_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_4_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_8_16(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_8_8(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void try_lowentropy_8_4(int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);
void lowentropy(int seg_size, int bits_to_take, int num_banks, int KB_per_bank, string dir, int fp_size_in_bits, double &cr, double &entropy_reduction, double &false_rate, bool use_xorcache);

void all(int num_banks, int KB_per_bank, string dir, vector <double> &crs, vector <double> &ers, vector <double> &frs, vector<double> fbs, bool use_xorcache,
    void (*func)(int, int, string, int, double &, double &, double &, bool));

#endif