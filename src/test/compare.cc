// #include "cache/cache.hh"
// #include "cache/xorCache.hh"
// #include "common/plot/plot.hh"
// #include <stdio.h>
// #include <string>
// #include <vector>
// #include "function/clustercache.hh"



// int main(int argc, char *argv[]){
//     if (argc < 2){
//         printf("Usage: %s <dir>\n", argv[0]);
//         return 1;
//     }
//     string dir = argv[1];
    
//     int num_banks = 4;
//     int KB_per_bank = 256;
//     if (argc >= 3) {
//         num_banks = atoi(argv[2]);
//     }
//     if (argc >= 4) {
//         KB_per_bank = atoi(argv[3]);
//     }

//     vector<double> fbs = {
//         1,2,
//         3,4,5, 
//         6,7, 8, 9, 10, 
//         11, 12, 13,14,15,16,17,18,19,20,
//         21,22,23,24,25,26,27,28,29,30,
//         31,32,33,34,35,36,37,38,39,40,
//         41,42,43,44,45,46,47,48,49,50,
//         51,52,53,53,55,56,57,58,59,60
//     };

//     vector<vector<vector<double>>> datas;
//     vector<vector<double>> crss, erss;
//     vector<vector<double>> frss;
//     vector<string> legends;
//     // string dir;
//     bool use_xorcache = true;


//     // vector<double> crs1, ers1, frs1;
//     // all(num_banks, KB_per_bank, dir, crs1, ers1, frs1, fbs, use_xorcache, &try_bbsxf);
//     // crss.push_back(crs1);
//     // erss.push_back(ers1);
//     // frss.push_back(frs1);
//     // legends.push_back("bbsxf");

//     // vector<double> crs2, ers2;
//     // all(num_banks, KB_per_bank, dir, crs2, ers2, fbs, use_xorcache, &try_fbsxf);
//     // crss.push_back(crs2);
//     // erss.push_back(ers2);
//     // legends.push_back("fbsxf");

//     // vector<double> crs3, ers3, frs3;
//     // all(num_banks, KB_per_bank, dir, crs3, ers3, frs3, fbs, use_xorcache, &try_thesaurus);
//     // crss.push_back(crs3);
//     // erss.push_back(ers3);
//     // frss.push_back(frs3);
//     // legends.push_back("thesaurus");

//     // vector<double> crs4, ers4, frs4;
//     // all(num_banks, KB_per_bank, dir, crs4, ers4, frs4, fbs, use_xorcache, &try_bs);
//     // crss.push_back(crs4);
//     // erss.push_back(ers4);
//     // frss.push_back(frs4);
//     // legends.push_back("bs");

//     // vector<double> crs5, ers5, frs5;
//     // all(num_banks, KB_per_bank, dir, crs5, ers5, frs5, fbs, use_xorcache, &try_bytemap);
//     // crss.push_back(crs5);
//     // erss.push_back(ers5);
//     // frss.push_back(frs5);
//     // legends.push_back("bytemap");


//     // vector<double> crs6, ers6, frs6;
//     // all(num_banks, KB_per_bank, dir, crs6, ers6, frs6, fbs, use_xorcache, &try_tbytemap);
//     // crss.push_back(crs6);
//     // erss.push_back(ers6);
//     // frss.push_back(frs6);
//     // legends.push_back("tenarybytemap");


//     // vector<double> crs7, ers7;
//     // all(num_banks, KB_per_bank, dir, crs7, ers7, fbs, use_xorcache, &try_2bytemap);
//     // crss.push_back(crs7);
//     // erss.push_back(ers7);
//     // legends.push_back("twobytemap");


//     // vector<double> crs8, ers8;
//     // all(num_banks, KB_per_bank, dir, crs8, ers8, fbs, use_xorcache, &try_4bytemap);
//     // crss.push_back(crs8);
//     // erss.push_back(ers8);
//     // legends.push_back("fourbytemap");


//     // vector<double> crs9, ers9, frs9;
//     // all(num_banks, KB_per_bank, dir, crs9, ers9, frs9, fbs, use_xorcache, &try_shuffledbytemap);
//     // crss.push_back(crs9);
//     // erss.push_back(ers9);
//     // frss.push_back(frs9);
//     // legends.push_back("shuffledbytemap");


//     // vector<double> crs10, ers10, frs10;
//     // all(num_banks, KB_per_bank, dir, crs10, ers10, frs10, fbs, use_xorcache, &try_shuffledtbytemap);
//     // crss.push_back(crs10);
//     // erss.push_back(ers10);
//     // frss.push_back(frs10);
//     // legends.push_back("shuffledtbytemap");


//     // vector<double> crs11, ers11, frs11;
//     // all(num_banks, KB_per_bank, dir, crs11, ers11, frs11, fbs, use_xorcache, &try_lowentropy_4_4);
//     // crss.push_back(crs11);
//     // erss.push_back(ers11);
//     // frss.push_back(frs11);
//     // legends.push_back("lowentropy4-4");


//     // vector<double> crs12, ers12, frs12;
//     // all(num_banks, KB_per_bank, dir, crs12, ers12, frs12, fbs, use_xorcache, &try_lowentropy_4_8);
//     // crss.push_back(crs12);
//     // erss.push_back(ers12);
//     // frss.push_back(frs12);
//     // legends.push_back("lowentropy4-8");


//     // vector<double> crs13, ers13, frs13;
//     // all(num_banks, KB_per_bank, dir, crs13, ers13, frs13, fbs, use_xorcache, &try_lowentropy_8_8);
//     // crss.push_back(crs13);
//     // erss.push_back(ers13);
//     // frss.push_back(frs13);
//     // legends.push_back("lowentropy8-8");


//     // vector<double> crs14, ers14, frs14;
//     // all(num_banks, KB_per_bank, dir, crs14, ers14, frs14, fbs, use_xorcache, &try_lowentropy_8_4);
//     // crss.push_back(crs14);
//     // erss.push_back(ers14);
//     // frss.push_back(frs14);
//     // legends.push_back("lowentropy8-4");

//     // vector<double> crs15, ers15, frs15;
//     // all(num_banks, KB_per_bank, dir, crs15, ers15, frs15, fbs, use_xorcache, &try_lowentropy_2_2);
//     // crss.push_back(crs15);
//     // erss.push_back(ers15);
//     // frss.push_back(frs15);
//     // legends.push_back("lowentropy2-2");


//     // vector<double> crs16, ers16, frs16;
//     // all(num_banks, KB_per_bank, dir, crs16, ers16, frs16, fbs, use_xorcache, &try_lowentropy_2_4);
//     // crss.push_back(crs16);
//     // erss.push_back(ers16);
//     // frss.push_back(frs16);
//     // legends.push_back("lowentropy2-4");


//     // vector<double> crs17, ers17, frs17;
//     // all(num_banks, KB_per_bank, dir, crs17, ers17, frs17, fbs, use_xorcache, &try_lowentropy_2_8);
//     // crss.push_back(crs17);
//     // erss.push_back(ers17);
//     // frss.push_back(frs17);
//     // legends.push_back("lowentropy2-8");

//     // vector<double> crs18, ers18, frs18;
//     // all(num_banks, KB_per_bank, dir, crs18, ers18, frs18, fbs, use_xorcache, &try_sparsebytemap_2);
//     // crss.push_back(crs18);
//     // erss.push_back(ers18);
//     // frss.push_back(frs18);
//     // legends.push_back("sbytemap-2");

//     // vector<double> crs19, ers19, frs19;
//     // all(num_banks, KB_per_bank, dir, crs19, ers19, frs19, fbs, use_xorcache, &try_sparsebytemap_4);
//     // crss.push_back(crs19);
//     // erss.push_back(ers19);
//     // frss.push_back(frs19);
//     // legends.push_back("sbytemap-4");

//     // vector<double> crs20, ers20, frs20;
//     // all(num_banks, KB_per_bank, dir, crs20, ers20, frs20, fbs, use_xorcache, &try_sparsebytemap_8);
//     // crss.push_back(crs20);
//     // erss.push_back(ers20);
//     // frss.push_back(frs20);
//     // legends.push_back("sbytemap-8");

//     vector<double> crs21, ers21, frs21;
//     all(num_banks, KB_per_bank, dir, crs21, ers21, frs21, fbs, use_xorcache, &try_sparseshuffledbytemap_2);
//     crss.push_back(crs21);
//     erss.push_back(ers21);
//     frss.push_back(frs21);
//     legends.push_back("sshuffledbytemap-2");

//     vector<double> crs22, ers22, frs22;
//     all(num_banks, KB_per_bank, dir, crs22, ers22, frs22, fbs, use_xorcache, &try_sparseshuffledbytemap_4);
//     crss.push_back(crs22);
//     erss.push_back(ers22);
//     frss.push_back(frs22);
//     legends.push_back("sshuffledbytemap-4");

//     vector<double> crs23, ers23, frs23;
//     all(num_banks, KB_per_bank, dir, crs23, ers23, frs23, fbs, use_xorcache, &try_sparseshuffledbytemap_8);
//     crss.push_back(crs23);
//     erss.push_back(ers23);
//     frss.push_back(frs23);
//     legends.push_back("sshuffledbytemap-8");


//     vector<double> crs24, ers24, frs24;
//     all(num_banks, KB_per_bank, dir, crs24, ers24, frs24, fbs, use_xorcache, &try_lowentropy_8_16);
//     crss.push_back(crs24);
//     erss.push_back(ers24);
//     frss.push_back(frs24);
//     legends.push_back("lowentropy8-16(BCD)");

//     datas.push_back(crss);
//     datas.push_back(erss);
//     datas.push_back(frss);

//     // print crss to file dir/crss.txt
//     string extra = "";
//     if (!use_xorcache){
//          extra = "-deltacache";
//     }
//     string crss_filename = dir + "/crss" + extra + ".txt";
//     FILE * crss_file = fopen(crss_filename.c_str(), "w");
//     for (unsigned i = 0; i < crss.size(); i++){
//         for (unsigned j = 0; j < crss[i].size(); j++){
//             fprintf(crss_file, "%f ", crss[i][j]);
//         }
//         fprintf(crss_file, "\n");
//     }
//     fclose(crss_file);

//     // print erss to file dir/erss.txt
//     string erss_filename = dir + "/erss" + extra + ".txt";
//     FILE * erss_file = fopen(erss_filename.c_str(), "w");
//     for (unsigned i = 0; i < erss.size(); i++){
//         for (unsigned j = 0; j < erss[i].size(); j++){
//             fprintf(erss_file, "%f ", erss[i][j]);
//         }
//         fprintf(erss_file, "\n");
//     }
//     fclose(erss_file);

//     // print erss to file dir/erss.txt
//     string frss_filename = dir + "/frss" + extra + ".txt";
//     FILE * frss_file = fopen(frss_filename.c_str(), "w");
//     for (unsigned i = 0; i < frss.size(); i++){
//         for (unsigned j = 0; j < erss[i].size(); j++){
//             fprintf(frss_file, "%f ", frss[i][j]);
//         }
//         fprintf(frss_file, "\n");
//     }
//     fclose(frss_file);

//     string plot_name = dir + "/all" + extra + ".pdf";
//     plot2dx3xn<double>(fbs, datas, legends, 
//         {"Fingerprint size (bits)","Fingerprint size (bits)","Fingerprint size (bits)"}, 
//         {"inter comp. ratio", "entropy reduction", "false positive rate"}, 
//         plot_name);

//     fbs.clear();
//     datas.clear();
//     legends.clear();
//     crss.clear();
//     erss.clear();
//     frss.clear();
//     // crs1.clear();
//     // ers1.clear();
//     // crs2.clear();
//     // ers2.clear();
//     // crs3.clear();
//     // ers3.clear();
//     // crs4.clear();
//     // ers4.clear();
//     // crs5.clear();
//     // ers5.clear();
//     // crs6.clear();
//     // ers6.clear();
//     // crs7.clear();
//     // ers7.clear();
//     // crs8.clear();
//     // ers8.clear();
//     // crs9.clear();
//     // ers9.clear();
//     // frs9.clear();
//     // crs10.clear();
//     // ers10.clear();
//     // crs11.clear();
//     // ers11.clear();
//     // crs12.clear();
//     // ers12.clear();
//     // crs13.clear();
//     // ers13.clear();
//     // crs14.clear();
//     // ers14.clear();
//     // crs15.clear();
//     // ers15.clear();
//     // crs16.clear();
//     // ers16.clear();
//     // crs17.clear();
//     // ers17.clear();
//     // crs18.clear();
//     // ers18.clear();
//     // crs19.clear();
//     // ers19.clear();
//     // crs20.clear();
//     // ers20.clear();
//     // crs21.clear();
//     // ers21.clear();
//     // crs22.clear();
//     // ers22.clear();
//     // crs23.clear();
//     // ers23.clear();
//     // crs24.clear();
//     // ers24.clear();

//     return 0;


// }