#include "cache/cache.hh"
#include "common/plot/plot.hh"
#include <stdio.h>
#include <string>
#include <vector>
#include "function/profiling.hh"

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Usage: %s <dir>\n", argv[0]);
        return 1;
    }
    string dir = argv[1];
    
    int num_banks = 4;
    int KB_per_bank = 256;
    if (argc >= 3) {
        num_banks = atoi(argv[2]);
    }
    if (argc >= 4) {
        KB_per_bank = atoi(argv[3]);
    }


    vector<string> legends;
    bool only_those_xored = false;
    string name_extra = "";
    if (only_those_xored) {
        name_extra = "-onlyxored";
    }


    string name = "entropy_byte_position_oracle";

    vector<double> results;
    unsigned seed = 12345;
    profiling_x(num_banks, KB_per_bank, dir, only_those_xored, results, seed, &profiling_entropy_byte_position_oracle);

    

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