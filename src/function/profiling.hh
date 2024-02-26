#ifndef FUNCTION_PROFILING_HH
#define FUNCTION_PROFILING_HH
#include <vector>
#include <string>
using namespace std;

void profiling_entropy_byte_position(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);
void profiling_entropy_byte_position_afterxor_randbank(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);
void profiling_entropy_byte_position_afterxor12_bytemap(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);
void profiling_entropy_byte_position_afterxor12_sparseshuffledbytemap_4_3(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);
void profiling_entropy_byte_position_afterxor12_thesaurus(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);
void profiling_entropy_byte_position_afterxor12_lowentropy_8_16(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies);

void profiling_x(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &results, 
    void(*profiling_function)(int, int, string, bool, vector <double> &));

#endif