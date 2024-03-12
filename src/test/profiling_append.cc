#include "cache/cache.hh"
#include "common/plot/plot.hh"
#include <stdio.h>
#include <string>
#include <vector>
#include "function/profiling.hh"

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Usage: %s <dir> <scheme>\n", argv[0]);
        return 1;
    }
    string dir = argv[1];
    string name = argv[2];
    
    int num_banks = 4;
    int KB_per_bank = 256;
    if (argc >= 4) {
        num_banks = atoi(argv[3]);
    }
    if (argc >= 5) {
        KB_per_bank = atoi(argv[4]);
    }


    vector<string> legends;
    bool only_those_xored = false;
    string name_extra = "";
    if (only_those_xored) {
        name_extra = "-onlyxored";
    }

    void(*profiling_function)(int, int, string, bool, vector <double> &, unsigned);
    if (name == "entropy_byte_position_oracle") {
        profiling_function = &profiling_entropy_byte_position_oracle;
    } else if (name == "entropy_byte_position_afterxor12_bytemap") {
        profiling_function = &profiling_entropy_byte_position_afterxor12_bytemap;
    } else if (name == "hamming_byte_position_oracle") {
        profiling_function = &profiling_hamming_byte_position_oracle;
    } else if (name == "hamming_byte_position_afterxor12_bytemap") {
        profiling_function = &profiling_hamming_byte_position_afterxor12_bytemap;
    } else if (name == "heatcube_bdi_sizechange_oracle") {
        profiling_function = &profiling_heatcube_bdi_sizechange_oracle;
    } else {
        assert(false);
    }

    vector<double> results;
    unsigned seed = 12345;
    profiling_x(num_banks, KB_per_bank, dir, only_those_xored, results, seed, profiling_function);

    

    string rst_filename = dir + "/profiling-" + name +name_extra+".txt";
    FILE * rst_file = fopen(rst_filename.c_str(), "w");
    for (unsigned j = 0; j < results.size(); j++){
        fprintf(rst_file, "%f ", results[j]);
    }
    fprintf(rst_file, "\n");
    fclose(rst_file);



    // string plot_name = dir + "/" + name + ".pdf";
    // vector<vector<double>> datas = {crs6, ers6};
    // plot2dx2<double>(fbs, datas, name, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
    //     plot_name);
    // datas.clear();

    legends.clear();
    results.clear();

    return 0;


}