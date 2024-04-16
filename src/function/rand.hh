#ifndef FUNCTION_RAND_HH
#define FUNCTION_RAND_HH
#include <string>
#include <vector>
using namespace std;
void create_rand_bank_bdi(unsigned seed, int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);


void create_rand_set_bdi(unsigned seed, int num_banks, int KB_per_bank, string dir, double &cr, double &entropy_reduction, double &false_rate, double &intra_compression_ratio, double &hamming,
    bool use_little_endian, bool allow_immo);


void rand_x(vector<unsigned> defined_seeds, int num_banks, int KB_per_bank, string dir, 
    vector <double> &crs, vector <double> &ers, vector <double> &frs, vector <double> &intras, vector <double> &hamming,
    vector <double> &crs_max, vector <double> &ers_max, vector <double> &frs_max, vector <double> &intras_max, vector <double> &hammings_max, 
    vector <double> &crs_min, vector <double> &ers_min, vector <double> &frs_min, vector <double> &intras_min, vector <double> &hammings_min, 
    vector<double> fbs, bool use_xorcache, bool use_little_e, bool allow_immo,
    void (*create_ideal_x)(unsigned, int, int, string, double &, double &, double &, double &, double &, bool, bool));
#endif
