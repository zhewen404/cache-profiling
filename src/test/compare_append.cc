#include "cache/cache.hh"
#include "cache/xorCache.hh"
#include "common/plot/plot.hh"
#include <stdio.h>
#include <string>
#include <vector>
#include "function/clustercache.hh"

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

    vector<double> fbs = {
        1,
        2,3,4,5, 
        6,7, 8, 9, 10, 
        11, 12, 13,14,15,16,17,18,19,20,
        21,22,23,24,25,26,27,28,29,30,
        31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,
        51,52,53,53,55,56,57,58,59,60
    };
    // vector<double> fbs = {
    //     2,4,6,8,10,12,14,16,18,20,
    //     22,24,26,28,30,32,34,36,38,40,
    //     42,44,46,48,50,52,54,58,60
    // };

    vector<string> legends;


    string name = "shuffledbytemap";
    bool use_xorcache = true;
    bool use_little_e = true;
    bool allow_immo = false;

    vector<double> crs6, ers6, frs6, intras6;

    // print crss to file dir/crss.txt
    if (name.find("vanila") == string::npos) {
        intracomp_t type = BPC;
        // no vanila in name
        if (!use_xorcache){
            name += "-deltacache";
        }
        if (!use_little_e){
            name += "-bigE";
        }
        if (allow_immo){
            name += "-immo";
        }
        if (type == BDI){
        } else if (type == BPC) {
            name += "-bpc";
        } else {
            assert(false);
        }
        map_all(num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, fbs, use_xorcache, use_little_e, allow_immo, type,
            &create_hashfunctions_shuffledbytemap);
    } else {
        vanila_x(num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, fbs, use_xorcache, use_little_e, allow_immo, 
            &create_vanila_bdi);
    }

    string crss_filename = dir + "/crss-" + name +".txt";
    FILE * crss_file = fopen(crss_filename.c_str(), "w");
    for (unsigned j = 0; j < crs6.size(); j++){
        fprintf(crss_file, "%f ", crs6[j]);
    }
    fprintf(crss_file, "\n");
    fclose(crss_file);


    // print erss to file dir/erss.txt
    string erss_filename = dir + "/erss-" + name + ".txt";
    FILE * erss_file = fopen(erss_filename.c_str(), "w");
    for (unsigned j = 0; j < ers6.size(); j++){
        fprintf(erss_file, "%f ", ers6[j]);
    }
    fprintf(erss_file, "\n");
    fclose(erss_file);


    // print frss to file dir/erss.txt
    string frss_filename = dir + "/frss-" + name + ".txt";
    FILE * frss_file = fopen(frss_filename.c_str(), "w");
    for (unsigned j = 0; j < frs6.size(); j++){
        fprintf(frss_file, "%f ", frs6[j]);
    }
    fprintf(frss_file, "\n");
    fclose(frss_file);


    // print frss to file dir/intras.txt
    string intras_filename = dir + "/intras-" + name + ".txt";
    FILE * instras_file = fopen(intras_filename.c_str(), "w");
    for (unsigned j = 0; j < intras6.size(); j++){
        fprintf(instras_file, "%f ", intras6[j]);
    }
    fprintf(instras_file, "\n");
    fclose(instras_file);

    // string plot_name = dir + "/" + name + ".pdf";
    // vector<vector<double>> datas = {crs6, ers6};
    // plot2dx2<double>(fbs, datas, name, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
    //     plot_name);
    // datas.clear();

    fbs.clear();
    legends.clear();
    crs6.clear();
    ers6.clear();
    frs6.clear();
    intras6.clear();

    return 0;


}