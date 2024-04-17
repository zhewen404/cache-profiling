#ifndef FUNCTION_IDEAL_HH
#define FUNCTION_IDEAL_HH
#include <string>
#include <vector>
using namespace std;

void create_ideal_bank_bdi(int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);

void create_ideal_set_bdi(int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);

void ideal_x(int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hamming,
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_ideal_x)(int, int, string, double &, double &, double &, double &, double &, bool, bool));

#endif