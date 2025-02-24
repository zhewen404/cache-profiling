#ifndef HEATMAP_AXIS_HH
#define HEATMAP_AXIS_HH

#include "cache/cache.hh"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string>
#include "common/plot/gnup.hh"
#include "cache/superLine.hh"
#include <cmath>  // For NAN

using namespace std;
// ThreeHierarchyLines is a class that contains a three dimensional array of lines
// dim=3, organized by bank, group/encoding, address

class ThreeHierarchyLines
{
    public:
        // if the constructor takes a Cache object, the second dimention can be ignored.
        ThreeHierarchyLines(Cache * cache) 
        {
            assert(cache->m_lines_populated);
            m_num_lines = 0;
            m_organized_lines = vector<vector<vector<Line*>>>();
            for (int i = 0; i < cache->m_num_banks; i++) {
                vector<Line*> bank_lines;
                vector<int> bank_boundary;
                int index = 0;
                bank_boundary.push_back(index);
                for (int j = 0; j < cache->m_num_sets; j++) {
                    for (long unsigned int k = 0; k < cache->m_lines[i][j].size(); k++) {
                        bank_lines.push_back(cache->m_lines[i][j][k]);
                        index += 1;
                    }
                }

                // in place sort bank_lines by address by using lambda function
                sort(bank_lines.begin(), bank_lines.end(), 
                    [](Line* a, Line* b) -> bool
                    {
                        return a->m_addr < b->m_addr;
                    });
                // for (long unsigned int j = 0; j < bank_lines.size(); j++) {
                //     if (i == 0)
                //     printf("bank %d line %ld addr %lx\n", i, j, bank_lines[j]->m_addr);
                // }
                bank_boundary.push_back(index);
                
                m_organized_lines.push_back({bank_lines});
                m_boundary.push_back(bank_boundary);
                m_num_lines += index;
                m_heatmap_dims.push_back(index);
                // m_heatmap_dims.push_back(2000);
            }

            // for (unsigned i = 0; i < m_organized_lines.size(); i++) {
            //     for (unsigned j = 0; j < m_organized_lines[i].size(); j++) {
            //         for (unsigned k = 0; k < m_organized_lines[i][j].size(); k++) {
            //             printf("i %d j %d k %d addr %lx\n", i, j, k, m_organized_lines[i][j][k]->m_addr);
            //         }
            //     }
            // }
        }

        // if the constructor takes a BankedClusteredCache object, the second dimention is the cluster index
        ThreeHierarchyLines(BankedClusteredCache * cache) 
        {
            assert(cache->m_clusters_populated);
            m_num_lines = 0;
            m_organized_lines = vector<vector<vector<Line*>>>();
            for (unsigned i = 0; i < cache->m_bank_clusters.size(); i++) { // bank loop
                vector<vector<Line*>> bank_lines;
                vector<int> bank_boundary;
                int index = 0;
                bank_boundary.push_back(index); // beginning of the bank
                for (unsigned j = 0; j < cache->m_bank_clusters[i].size(); j++) {
                    bank_boundary.push_back(index); // beginning of the group
                    vector<Line*> group_lines;
                    for (long unsigned int k = 0; k < cache->m_bank_clusters[i][j].size(); k++) {
                        group_lines.push_back(cache->m_bank_clusters[i][j][k]);
                        index += 1;
                    }
                    // in place sort group_lines by address by using lambda function
                    sort(group_lines.begin(), group_lines.end(), 
                        [](Line* a, Line* b) -> bool
                        {
                            return a->m_addr < b->m_addr;
                        });
                    bank_lines.push_back(group_lines);
                    bank_boundary.push_back(index); // end of the group
                }

                bank_boundary.push_back(index); // end of the bank
                
                m_organized_lines.push_back(bank_lines);
                m_boundary.push_back(bank_boundary);
                m_num_lines += index;
                m_heatmap_dims.push_back(index);
                // m_heatmap_dims.push_back(2000);
            }
        }

        ~ThreeHierarchyLines()
        {
            for (unsigned i = 0; i < m_organized_lines.size(); i++) {
                for (unsigned j = 0; j < m_organized_lines[i].size(); j++) {
                    m_organized_lines[i][j].clear();
                }
                m_organized_lines[i].clear();
            }
            m_organized_lines.clear();
            for (unsigned i = 0; i < m_boundary.size(); i++) {
                m_boundary[i].clear();
            }
            m_boundary.clear();

            m_heatmap_dims.clear();

            // clean up m_heatmaps
            for (unsigned i = 0; i < m_heatmaps.size(); i++) {
                for (unsigned j = 0; j < m_heatmaps[i].size(); j++) {
                    m_heatmaps[i][j].clear();
                }
                m_heatmaps[i].clear();
            }
            m_heatmaps.clear();

            // clean up m_xoredline_2ds
            for (unsigned i = 0; i < m_xoredline_2ds.size(); i++) {
                for (unsigned j = 0; j < m_xoredline_2ds[i].size(); j++) {
                    for (unsigned k = 0; k < m_xoredline_2ds[i][j].size(); k++) {
                        if (m_xoredline_2ds[i][j][k] != nullptr) {
                            delete m_xoredline_2ds[i][j][k];
                        }
                    }
                    m_xoredline_2ds[i][j].clear();
                }
                m_xoredline_2ds[i].clear();
            }
            m_xoredline_2ds.clear();
        }

        void generate_empty_heatmap()
        {
            for (unsigned i = 0; i < m_heatmap_dims.size(); i++) { // bank loop
                int dim = m_heatmap_dims[i];
                vector<vector<double>> bank_heatmap;
                for (int j = 0; j < dim; j++) {
                    vector<double> row(dim, NAN);
                    bank_heatmap.push_back(row);
                }
                m_heatmaps.push_back(bank_heatmap);
            }
        };

        void fill_XORedLines()
        {
            // flatten m_organized_lines into a 2d array, the first dimension remains the same
            // the second and third dimensions are flattened
            vector<vector<Line*>> flat_2d_lines;
            vector<unordered_map<int, int>> group_maps;
            group_maps.clear();
            for (unsigned i = 0; i < m_organized_lines.size(); i++) { // bank loop
                int index = 0;
                vector<Line*> flat_lines;
                unordered_map<int, int> group_map;
                group_map.clear();
                for (unsigned j = 0; j < m_organized_lines[i].size(); j++) { // group loop
                    for (unsigned k = 0; k < m_organized_lines[i][j].size(); k++) { // line loop
                        flat_lines.push_back(m_organized_lines[i][j][k]);
                        group_map[index] = j;
                        index += 1;
                    }
                }
                flat_2d_lines.push_back(flat_lines);
                group_maps.push_back(group_map);
            }
            // for (unsigned i = 0; i < flat_2d_lines.size(); i++) {
            //     for (unsigned j = 0; j < flat_2d_lines[i].size(); j++) {
            //         if (i == 0)
            //         printf("bank %d j %d addr %lx ", i, j, flat_2d_lines[i][j]->m_addr); 
            //         flat_2d_lines[i][j]->print();
            //     }
            // }

            // iterate through the flat_2d_lines and fill m_xoredline_2ds
            for (unsigned i = 0; i < m_heatmap_dims.size(); i++) { // bank loop
                int dim = m_heatmap_dims[i];
                vector<vector<XORedLine*>> bank_xoredlines;
                for (int j = 0; j < dim; j++) { // row dimension
                    vector<XORedLine*> row_xoredlines;
                    for (int k = 0; k < dim; k++) { // col dimension
                        if (k >= j) {
                            row_xoredlines.push_back(nullptr);
                        } else if (group_maps[i][j] != group_maps[i][k]) {
                            row_xoredlines.push_back(nullptr);
                        } else {
                            // printf("bank %d j %d xorwith k %d ", i, j, k);
                            vector<Line*> line_pair = vector<Line*>();
                            line_pair.push_back(flat_2d_lines[i][j]);
                            line_pair.push_back(flat_2d_lines[i][k]);
                            XORedLine * xored_line = new XORedLine(line_pair);
                            // xored_line->print();
                            row_xoredlines.push_back(xored_line);
                        }          
                    }
                    bank_xoredlines.push_back(row_xoredlines);
                }
                m_xoredline_2ds.push_back(bank_xoredlines);
            }

            // clear group_maps
            for (unsigned i = 0; i < group_maps.size(); i++) {
                group_maps[i].clear();
            }
        }

        // heatmap is filled with bdi compressed sizes
        // void fill_heatmap(BDICompressor* bdicomp)
        // {
        //     // iterate through m_xoredline_2ds and fill m_heatmaps
        //     for (unsigned i = 0; i < m_heatmap_dims.size(); i++) { // bank loop
        //         int dim = m_heatmap_dims[i];
        //         for (int j = 0; j < dim; j++) { // row dimension
        //             for (int k = 0; k < dim; k++) { // col dimension
        //                 if (m_xoredline_2ds[i][j][k] != nullptr) {
        //                     XORedLine * xored_line = m_xoredline_2ds[i][j][k];
        //                     BDILine * bdiline = bdicomp->compress_a_line(xored_line);
        //                     m_heatmaps[i][j][k] = bdiline->m_compressed_size;
        //                 }
        //             }
        //         }
        //     }
        // }
        template <typename T>
        void fill_heatmap(BaseIntraCompressor<T>* bdicomp)
        {
            // iterate through m_xoredline_2ds and fill m_heatmaps
            for (unsigned i = 0; i < m_heatmap_dims.size(); i++) { // bank loop
                int dim = m_heatmap_dims[i];
                for (int j = 0; j < dim; j++) { // row dimension
                    for (int k = 0; k < dim; k++) { // col dimension
                        if (m_xoredline_2ds[i][j][k] != nullptr) {
                            XORedLine * xored_line = m_xoredline_2ds[i][j][k];
                            T * bdiline = bdicomp->compress_a_line(xored_line);
                            m_heatmaps[i][j][k] = bdiline->m_compressed_size;
                        }
                    }
                }
            }
        }

        // heatmap is filled with line's bit hamming weight
        void fill_heatmap(string weight_bit_or_byte)
        {
            // iterate through m_xoredline_2ds and fill m_heatmaps
            for (unsigned i = 0; i < m_heatmap_dims.size(); i++) { // bank loop
                int dim = m_heatmap_dims[i];
                for (int j = 0; j < dim; j++) { // row dimension
                    for (int k = 0; k < dim; k++) { // col dimension
                        if (m_xoredline_2ds[i][j][k] != nullptr) {
                            XORedLine * xored_line = m_xoredline_2ds[i][j][k];
                            if (weight_bit_or_byte == "bit") {
                                m_heatmaps[i][j][k] = xored_line->get_bit_hamming_weight();
                            } else if (weight_bit_or_byte == "byte") {
                                m_heatmaps[i][j][k] = xored_line->get_byte_hamming_weight();
                            } else {
                                assert(false);
                            }
                        }
                    }
                }
            }
        }

        // plot elements of m_heatmaps as subfigures
        // each element is a 2d heatmap
        // store the subfigures in a pdf file
        void plot(string filename)
        {
            plot_heatmaps<double>(m_heatmaps, m_boundary, m_heatmap_dims, filename);
        };

        vector<vector<vector<double>>> m_heatmaps; // bank, 2dmap
        vector<vector<vector<XORedLine*>>> m_xoredline_2ds; // bank, 2dmap
        vector<vector<vector<Line*>>> m_organized_lines;
        vector<vector<int>> m_boundary; // per bank is a vec<int>, denoting the vertical and horizontal lines location
        // to put on the heatmap
        int m_num_lines;
        vector<int> m_heatmap_dims;

};

#endif