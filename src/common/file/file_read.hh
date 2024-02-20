#ifndef COMMON_FILE_FILE_READ_HH
#define COMMON_FILE_FILE_READ_HH

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string>
#include <vector>
using namespace std;

u_int8_t * read_bytes_from_file(unsigned *size, string filename);
u_int8_t * read_bytes_from_filepointer(unsigned *size, FILE * file);
u_int64_t * read_addr_from_file(unsigned *size, string filename);
u_int64_t * read_addr_from_filepointer(unsigned *size, FILE * file);

void fill_string_arrays_data_addr(vector<string> &filenames_data, vector<string> &filenames_addr, string dir, int num_banks);

#endif // COMMON_FILE_FILE_READ_HH