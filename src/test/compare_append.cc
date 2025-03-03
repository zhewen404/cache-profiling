// this is now deprecated, see compare_append_thesaurus.cc
#include "cache/cache.hh"
#include "cache/xorCache.hh"
#include "common/plot/plot.hh"
#include <stdio.h>
#include <string>
#include <vector>
#include "function/clustercache.hh"
#include "function/vanila.hh"
#include "function/ideal.hh"
#include "function/rand.hh"

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

    vector<double> fbs = {
        1,
        2,3,4,5, 
        6,7, 8, 9, 10, 
        11, 12, 13,14,15,16,17,18,19,20,
        21,22,23,24,25,26,27,28,29,30,
        31,32,
        // 33,34,35,36,37,38,39,40,
        // 41,42,43,44,45,46,47,48,49,50,
        // 51,52,53,53,55,56,57,58,59,60
    };
    // vector<double> fbs = {
    //     2,4,6,8,10,12,14,16,18,20,
    //     22,24,26,28,30,32,34,36,38,40,
    //     42,44,46,48,50,52,54,58,60
    // };
    vector<unsigned> defined_seeds = {9, 10, 11, 12
    };

    vector<string> legends;


    void (*create_hash_functions_x)(vector<HashFunction *> &, int &, bool &, int &, int, int, unsigned);

    

    bool use_xorcache = true;
    bool use_little_e = true;
    bool allow_immo = false;


    vector<double> crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6;
    vector<double> crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max;
    vector<double> crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min;

    // print crss to file dir/crss.txt
    if (name.find("vanila") != string::npos) {
        if (name.find("bpc") != string::npos) {
            vanila_x(num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,
                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_vanila_bpc);
        } else {
            vanila_x(num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,
                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_vanila_bdi);
        }
    }
    else if (name.find("idealbank") != string::npos) {
        if (name.find("bpc") != string::npos) {
            int shift_bank = 0;
            int shift_set = 0;
            if (name.find("shift1") != string::npos) {
                shift_bank = 1;
            } else if (name.find("shift2") != string::npos) {
                shift_bank = 2;
            } else if (name.find("shift3") != string::npos) {
                shift_bank = 3;
            } else if (name.find("shift4") != string::npos) {
                shift_bank = 4;
            } 
            ideal_x(shift_bank, shift_set, num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, fbs, use_xorcache, use_little_e, allow_immo, 
                &create_ideal_bank_bpc_shift);
        } else {
            int shift_bank = 0;
            int shift_set = 0;
            if (name.find("shift1") != string::npos) {
                shift_bank = 1;
            } else if (name.find("shift2") != string::npos) {
                shift_bank = 2;
            } else if (name.find("shift3") != string::npos) {
                shift_bank = 3;
            } else if (name.find("shift4") != string::npos) {
                shift_bank = 4;
            } 
            ideal_x(shift_bank, shift_set, num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, fbs, use_xorcache, use_little_e, allow_immo, 
                &create_ideal_bank_bdi_shift);
        }
    }
    else if (name.find("idealset") != string::npos) {
        if (name.find("bpc") != string::npos) {
            int shift_bank = 0;
            int shift_set = 0;
            if (name.find("shift1") != string::npos) {
                shift_bank = 1;
                shift_set = 1;
            } else if (name.find("shift2") != string::npos) {
                shift_bank = 2;
                shift_set = 2;
            } else if (name.find("shift3") != string::npos) {
                shift_bank = 3;
                shift_set = 3;
            } else if (name.find("shift4") != string::npos) {
                shift_bank = 4;
                shift_set = 4;
            } 
            ideal_x(shift_bank, shift_set, num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, fbs, use_xorcache, use_little_e, allow_immo, 
                &create_ideal_set_bpc_shift);
        } else {
            int shift_bank = 0;
            int shift_set = 0;
            if (name.find("shift1") != string::npos) {
                shift_bank = 1;
                shift_set = 1;
            } else if (name.find("shift2") != string::npos) {
                shift_bank = 2;
                shift_set = 2;
            } else if (name.find("shift3") != string::npos) {
                shift_bank = 3;
                shift_set = 3;
            } else if (name.find("shift4") != string::npos) {
                shift_bank = 4;
                shift_set = 4;
            } 
            ideal_x(shift_bank, shift_set, num_banks, KB_per_bank, dir, crs6, ers6, frs6, intras6, hammings6, fbs, use_xorcache, use_little_e, allow_immo, 
                &create_ideal_set_bdi_shift);
        }
    }
    else if (name.find("randbank") != string::npos) {
        if (name.find("bpc") != string::npos) {
            rand_x(defined_seeds, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,

                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, 
                centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,

                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, 
                centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,

                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_rand_bank_bpc);
        } else {
            rand_x(defined_seeds, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,

                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, 
                centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,

                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, 
                centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,

                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_rand_bank_bdi);
        }
    }
    else if (name.find("randset") != string::npos) {
        if (name.find("bpc") != string::npos) {
            rand_x(defined_seeds, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,

                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, 
                centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,

                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, 
                centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,

                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_rand_set_bpc);
        } else {
            rand_x(defined_seeds, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, 
                centroid6, noncentroid_success6, noncentroid_fail6,

                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, 
                centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,

                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, 
                centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,

                fbs, use_xorcache, use_little_e, allow_immo, 
                &create_rand_set_bdi);
        }
    }
    else {
        // no vanila in name
        
        // intracomp_t type = BDI;
        intracomp_t type = BPC;
        bool banked = true;

        if (banked == true){
            name += "-banked";
        }

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
        int shift_bank = 0;
        int shift_set = 0;

        if (name.find("epc_word_labeling") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {24.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_epc_word_labeling);
        } else if (name.find("strong_word_labeling") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {16.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_strong_word_labeling);
        } else if (name.find("hycomp_word_labeling") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {24.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_hycomp_word_labeling);
        } else if (name.find("semantic_word_labeling") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {24.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_semantic_word_labeling);
        } else if (name.find("density_word_labeling") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {16.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_density_word_labeling);
        } else if (name.find("averagebytemsb_word_labeling_32") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {32.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_averagebytemsb_word_labeling_32);
        } else if (name.find("averagebytemsb_word_labeling_24") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {24.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_averagebytemsb_word_labeling_24);
        } else if (name.find("averagebytemsb_word_labeling_16") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {16.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_averagebytemsb_word_labeling_16);
        } else if (name.find("bdi_immo_line_labeling_4") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {4.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_bdi_immo_line_labeling_4);
        } else if (name.find("bdi_line_labeling_4") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {4.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_bdi_line_labeling_4);
        } else if (name.find("bpc_line_labeling_8") != string::npos) {
            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                {8.0}, use_xorcache, use_little_e, allow_immo, type,
                &create_hashfunctions_bpc_line_labeling_8);
        } else {
            if (name.find("shift1") != string::npos) {
                shift_bank = 1;
            } else if (name.find("shift2") != string::npos) {
                shift_bank = 2;
            } else if (name.find("shift3") != string::npos) {
                shift_bank = 3;
            } else if (name.find("shift4") != string::npos) {
                shift_bank = 4;
            }
            // compare string name with the following
            if (name.find("sparseshuffledbytemap_8_6") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_sparseshuffledbytemap_8_6;
            } else if (name.find("shuffledbytemap") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_shuffledbytemap;
            } else if (name.find("fbsxf") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_fbsxf;
            } else if (name.find("thesaurus") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_thesaurus;
            } else if (name.find("maskedbs_8_32") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_maskedbs_8_32;
            } else if (name.find("maskedbs_8_16") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_maskedbs_8_16;
            } else if (name.find("bs") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_bs;
            } else if (name.find("2bytemap") != string::npos) {
                create_hash_functions_x = &create_hashfunctions_2bytemap;
            } else {
                printf("Unknown scheme %s\n", name.c_str());
                assert(false);
            }

            map_all(banked, defined_seeds, shift_bank, shift_set, num_banks, KB_per_bank, dir, 
                crs6, ers6, frs6, intras6, hammings6, centroid6, noncentroid_success6, noncentroid_fail6,
                crs6_max, ers6_max, frs6_max, intras6_max, hammings6_max, centroid6_max, noncentroid_success6_max, noncentroid_fail6_max,
                crs6_min, ers6_min, frs6_min, intras6_min, hammings6_min, centroid6_min, noncentroid_success6_min, noncentroid_fail6_min,
                fbs, use_xorcache, use_little_e, allow_immo, type,
                create_hash_functions_x);
        }
    } 
    

    string crss_filename = dir + "/crss-" + name +".txt";
    FILE * crss_file = fopen(crss_filename.c_str(), "w");
    for (unsigned j = 0; j < crs6.size(); j++){
        fprintf(crss_file, "%f ", crs6[j]);
    }
    fprintf(crss_file, "\n");
    for (unsigned j = 0; j < crs6_max.size(); j++){
        fprintf(crss_file, "%f ", crs6_max[j]);
    }
    fprintf(crss_file, "\n");
    for (unsigned j = 0; j < crs6_min.size(); j++){
        fprintf(crss_file, "%f ", crs6_min[j]);
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
    for (unsigned j = 0; j < ers6_max.size(); j++){
        fprintf(erss_file, "%f ", ers6_max[j]);
    }
    fprintf(erss_file, "\n");
    for (unsigned j = 0; j < ers6_min.size(); j++){
        fprintf(erss_file, "%f ", ers6_min[j]);
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
    for (unsigned j = 0; j < frs6_max.size(); j++){
        fprintf(frss_file, "%f ", frs6_max[j]);
    }
    fprintf(frss_file, "\n");
    for (unsigned j = 0; j < frs6_min.size(); j++){
        fprintf(frss_file, "%f ", frs6_min[j]);
    }
    fprintf(frss_file, "\n");
    fclose(frss_file);


    // print intras to file dir/intras.txt
    string intras_filename = dir + "/intras-" + name + ".txt";
    FILE * instras_file = fopen(intras_filename.c_str(), "w");
    for (unsigned j = 0; j < intras6.size(); j++){
        fprintf(instras_file, "%f ", intras6[j]);
    }
    fprintf(instras_file, "\n");
    for (unsigned j = 0; j < intras6_max.size(); j++){
        fprintf(instras_file, "%f ", intras6_max[j]);
    }
    fprintf(instras_file, "\n");
    for (unsigned j = 0; j < intras6_min.size(); j++){
        fprintf(instras_file, "%f ", intras6_min[j]);
    }
    fprintf(instras_file, "\n");
    fclose(instras_file);


    // print hammings to file dir/hammings.txt
    string hammings_filename = dir + "/hammings-" + name + ".txt";
    FILE * hammings_file = fopen(hammings_filename.c_str(), "w");
    for (unsigned j = 0; j < hammings6.size(); j++){
        fprintf(hammings_file, "%f ", hammings6[j]);
    }
    fprintf(hammings_file, "\n");
    for (unsigned j = 0; j < hammings6_max.size(); j++){
        fprintf(hammings_file, "%f ", hammings6_max[j]);
    }
    fprintf(hammings_file, "\n");
    for (unsigned j = 0; j < hammings6_min.size(); j++){
        fprintf(hammings_file, "%f ", hammings6_min[j]);
    }
    fprintf(hammings_file, "\n");
    fclose(hammings_file);

    // print centroid to file dir/centroid.txt
    string centroid_filename = dir + "/centroid-" + name + ".txt";
    FILE * centroid_file = fopen(centroid_filename.c_str(), "w");
    for (unsigned j = 0; j < centroid6.size(); j++){
        fprintf(centroid_file, "%f ", centroid6[j]);
    }
    fprintf(centroid_file, "\n");
    for (unsigned j = 0; j < centroid6_max.size(); j++){
        fprintf(centroid_file, "%f ", centroid6_max[j]);
    }
    fprintf(centroid_file, "\n");
    for (unsigned j = 0; j < centroid6_min.size(); j++){
        fprintf(centroid_file, "%f ", centroid6_min[j]);
    }
    fprintf(centroid_file, "\n");
    fclose(centroid_file);

    // print noncentroid_success to file dir/noncentroid_success.txt
    string noncentroid_success_filename = dir + "/noncentroid_success-" + name + ".txt";
    FILE * noncentroid_success_file = fopen(noncentroid_success_filename.c_str(), "w");
    for (unsigned j = 0; j < noncentroid_success6.size(); j++){
        fprintf(noncentroid_success_file, "%f ", noncentroid_success6[j]);
    }
    fprintf(noncentroid_success_file, "\n");
    for (unsigned j = 0; j < noncentroid_success6_max.size(); j++){
        fprintf(noncentroid_success_file, "%f ", noncentroid_success6_max[j]);
    }
    fprintf(noncentroid_success_file, "\n");
    for (unsigned j = 0; j < noncentroid_success6_min.size(); j++){
        fprintf(noncentroid_success_file, "%f ", noncentroid_success6_min[j]);
    }
    fprintf(noncentroid_success_file, "\n");
    fclose(noncentroid_success_file);

    // print noncentroid_fail to file dir/noncentroid_fail.txt
    string noncentroid_fail_filename = dir + "/noncentroid_fail-" + name + ".txt";
    FILE * noncentroid_fail_file = fopen(noncentroid_fail_filename.c_str(), "w");
    for (unsigned j = 0; j < noncentroid_fail6.size(); j++){
        fprintf(noncentroid_fail_file, "%f ", noncentroid_fail6[j]);
    }
    fprintf(noncentroid_fail_file, "\n");
    for (unsigned j = 0; j < noncentroid_fail6_max.size(); j++){
        fprintf(noncentroid_fail_file, "%f ", noncentroid_fail6_max[j]);
    }
    fprintf(noncentroid_fail_file, "\n");
    for (unsigned j = 0; j < noncentroid_fail6_min.size(); j++){
        fprintf(noncentroid_fail_file, "%f ", noncentroid_fail6_min[j]);
    }
    fprintf(noncentroid_fail_file, "\n");
    fclose(noncentroid_fail_file);

    // string plot_name = dir + "/" + name + ".pdf";
    // vector<vector<double>> datas = {crs6, ers6};
    // plot2dx2<double>(fbs, datas, name, {"Fingerprint size (bits)","Fingerprint size (bits)"}, {"inter comp. ratio", "entropy reduction"}, 
    //     plot_name);
    // datas.clear();

    fbs.clear();
    legends.clear();
    defined_seeds.clear();

    crs6.clear();
    ers6.clear();
    frs6.clear();
    intras6.clear();
    hammings6.clear();
    centroid6.clear();
    noncentroid_success6.clear();
    noncentroid_fail6.clear();

    crs6_min.clear();
    ers6_min.clear();
    frs6_min.clear();
    intras6_min.clear();
    hammings6_min.clear();
    centroid6_min.clear();
    noncentroid_success6_min.clear();
    noncentroid_fail6_min.clear();

    crs6_max.clear();
    ers6_max.clear();
    frs6_max.clear();
    intras6_max.clear();
    hammings6_max.clear();
    centroid6_max.clear();
    noncentroid_success6_max.clear();
    noncentroid_fail6_max.clear();

    return 0;


}