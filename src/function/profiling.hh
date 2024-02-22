#ifndef FUNCTION_PROFILING_HH
#define FUNCTION_PROFILING_HH
#include <vector>
#include <string>
using namespace std;

void profiling_entropy_byte_position(int num_banks, int KB_per_bank, string dir, vector <double> &entropies);

void profiling_x(int num_banks, int KB_per_bank, string dir, vector <double> &results, 
    void(*profiling_function)(int, int, string, vector <double> &));

#endif