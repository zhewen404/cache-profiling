#include "common/file/file_read.hh"

u_int8_t * read_bytes_from_file(unsigned *size, string filename) //Don't forget to free retval after use
{
   FILE * file = fopen(filename.c_str(), "r");
   if (file == NULL) {
      perror("fopen Failed: ");
      return NULL;
   }
   u_int8_t * retval = read_bytes_from_filepointer(size, file);
   fclose(file);
   return retval;
}

u_int8_t * read_bytes_from_filepointer(unsigned *size, FILE * file) //Don't forget to free retval after use
{
   *size = 0;
   unsigned int val;
   int startpos = ftell(file);
   while (fscanf(file, "%d ", &val) == 1)
   {
      *size += 1;
   }
   if ((*size)/64 % 2 == 1) {
      *size -= 64;
   }
   // printf("%d\n", *size);
   u_int8_t * retval = (unsigned char *) malloc(*size);
   fseek(file, startpos, SEEK_SET); //if the file was not on the beginning when we started
   int pos = 0;
   while (fscanf(file, "%d ", &val) == 1)
   {
      retval[pos] = (unsigned char) val;
      pos++;
      if ((unsigned)pos == *size) {
         break;
      }
   }
   return retval;
}

uint64_t * read_addr_from_file(unsigned *size, string filename) //Don't forget to free retval after use
{
   FILE * file = fopen(filename.c_str(), "r");
   if (file == NULL) {
      perror("fopen Failed: ");
      return NULL;
   }
   uint64_t * retval = read_addr_from_filepointer(size, file);
   fclose(file);
   return retval;
}

uint64_t * read_addr_from_filepointer(unsigned *size, FILE * file) //Don't forget to free retval after use
{
   *size = 0;
   uint64_t val;
   uint64_t val2;
   int startpos = ftell(file);
   // while (fscanf(file, "[%lu" PRIx64 ", line %lu" PRIx64 "]", &val, &val2) == 2)
   while (fscanf(file, "[%lx, line %lx]\n", &val, &val2) == 2)
   {
      *size += 1;
   }
   if (*size % 2 == 1) {
      *size -= 1;
   }
   // printf("%d\n", *size);
   uint64_t * retval = (uint64_t *) malloc(sizeof(uint64_t)*(*size));
   fseek(file, startpos, SEEK_SET); //if the file was not on the beginning when we started
   int pos = 0;
   while (fscanf(file, "[%lx, line %lx]\n", &val, &val2) == 2)
   {
      retval[pos] = (uint64_t) val;
      pos++;
      if ((unsigned)pos == *size) {
         break;
      }
   }
   return retval;
}

void fill_string_arrays_data_addr(
   vector<string> &filenames_data, vector<string> &filenames_addr, string dir, int num_banks)
{
   char filename_base[256];
   snprintf(filename_base, sizeof(filename_base), "%s%s", dir.c_str(), "/system.cache_hierarchy.ruby_system.l2_controllers");


    for (int i=0; i < num_banks; i++) {
        char filename_data[512];
        char filename_addr[512];
        if (num_banks >= 10) {
            snprintf(filename_data, sizeof(filename_data), "%s%02d", filename_base, i);
            snprintf(filename_addr, sizeof(filename_addr), "%s%02d%s", filename_base, i,"_addr");
        }
        else if (num_banks == 1) {
            snprintf(filename_data, sizeof(filename_data), "%s", filename_base);
            snprintf(filename_addr, sizeof(filename_addr), "%s%s", filename_base,"_addr");
        }
        else {
            snprintf(filename_data, sizeof(filename_data), "%s%d", filename_base, i);
            snprintf(filename_addr, sizeof(filename_addr), "%s%d%s", filename_base, i,"_addr");
        }

        filenames_data.push_back(string(filename_data));
        filenames_addr.push_back(string(filename_addr));
    }
}
