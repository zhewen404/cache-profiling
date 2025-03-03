#include "cache/cache.hh"
#include "cache/encodedCache.hh"
#include "common/file/file_read.hh"
#include <stdio.h>
#include <string>

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;
    
    int num_banks = 4;
    int line_size = 64;
    int assoc = 16;
    int KB_per_bank = 256;
    int shift_bank = 0;
    int shift_set = 0;

    vector<string> filenames_data;
    vector<string> filenames_addr;
    string dir = "test_input";
    fill_string_arrays_data_addr(filenames_data, filenames_addr, dir, num_banks);

    Cache * cache;
    cache = new Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set);
    cache->populate_lines(filenames_data, filenames_addr);

    int granularity_in_bit = 8;

    DBIEncodedCache * dbiCache;
    dbiCache = new DBIEncodedCache(*cache, granularity_in_bit);
    delete cache;


    dbiCache->print();

    delete dbiCache;

    return 0;
}
