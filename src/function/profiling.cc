#include "function/profiling.hh"
#include "cache/cache.hh"
#include "common/file/file_read.hh"

void profiling_entropy_byte_position(int num_banks, int KB_per_bank, string dir, vector <double> &entropies)
{
    int line_size = 64;
    int assoc = 16;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, line_size, assoc, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    vector<double> * ent_vec = cache->get_per_byte_entropy();
    for (unsigned i = 0; i < ent_vec->size(); i++){
        entropies.push_back((*ent_vec)[i]);
    }
    delete ent_vec;
    delete cache;
}

void profiling_x(int num_banks, int KB_per_bank, string dir, vector <double> &results, 
    void(*profiling_function)(int, int, string, vector <double> &))
{
    profiling_function(num_banks, KB_per_bank, dir, results);
}