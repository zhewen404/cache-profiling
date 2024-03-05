#ifndef CACHE_XORCACHE_HH
#define CACHE_XORCACHE_HH
#include "cache/cache.hh"
#include "cache/superLine.hh"
#include "compression/intraCompressor.hh"
#include <random>
#include <algorithm>

using namespace std;

class RandBankXORCache : public BaseCache
{
    public:
    vector<vector<XORedLine*>> m_lines;
    int m_uncompressed_size;

    RandBankXORCache(const Cache& cache, unsigned seed) : 
    BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_uncompressed_size = cache.get_size();
        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<XORedLine*>());

            // aggregate all line pointers in this bank
            // printf("aggregating line pointers in bank %d\n", i);
            vector<Line*> lines_in_this_bank;
            for (int j = 0; j < m_num_sets; j++) {
                for (Line* line : cache.m_lines[i][j]) {
                    lines_in_this_bank.push_back(line);
                }
            }

            default_random_engine rng(seed);
            shuffle(lines_in_this_bank.begin(), lines_in_this_bank.end(), rng);

            // split lines_in_this_bank into a vector of vectors of size 2
            // printf("spliting into line pairs in bank %d\n", i);
            vector<vector<Line*>> lines_in_this_bank_split;
            int number_of_line_pairs = ceil(lines_in_this_bank.size() / 2.0);
            for (unsigned j = 0; j < (unsigned)number_of_line_pairs; j++) {
                vector<Line*> line_pair;
                if (j * 2 + 1 < lines_in_this_bank.size()) {
                    line_pair.push_back(lines_in_this_bank[j * 2]);
                    line_pair.push_back(lines_in_this_bank[j * 2 + 1]);
                } else {
                    line_pair.push_back(lines_in_this_bank[j * 2]);
                }
                lines_in_this_bank_split.push_back(line_pair);
            }


            // for each pair of lines, create a XORedLine object
            for (vector<Line*> line_pair : lines_in_this_bank_split) {
                XORedLine* xor_line = new XORedLine(line_pair);
                // printf("created XORedLine object, adding to bank %d, size=%ld\n", i, m_lines[i].size());
                m_lines[i].push_back(xor_line);
            }
        }
    }

    RandBankXORCache(const Cache& cache) :
    RandBankXORCache(cache, 12345)
    {
    }

    ~RandBankXORCache()
    {
        for (int i = 0; i < m_num_banks; i++) {
            vector<XORedLine*> lines = m_lines[i];
            for (XORedLine* line : lines) {
                delete line;
            }
        }
    }

    double get_bit_entropy() const;

    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;
    vector<double> * get_per_byte_entropy() const;
    vector<double> * get_per_byte_entropy_only_thoses_xored() const;


    void print() const;

};

class RandSetXORCache : public BaseCache
{
    public:
    vector<vector<XORedLine*>> m_lines;
    int m_num_xored_lines;
    int m_uncompressed_size;

    RandSetXORCache(const Cache& cache, unsigned seed) : 
    BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_uncompressed_size = cache.get_size();
        int count = 0;
        int set_ind = 0;
        for (int i = 0; i < m_num_banks; i++) {

            // aggregate all line pointers in this bank
            // printf("aggregating line pointers in bank %d\n", i);
            for (int j = 0; j < m_num_sets; j++) {
                vector<Line*> lines_in_this_set;
                m_lines.push_back(vector<XORedLine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    lines_in_this_set.push_back(line);
                }
                default_random_engine rng(seed);
                shuffle(lines_in_this_set.begin(), lines_in_this_set.end(), rng);

                // split lines_in_this_set into a vector of vectors of size 2
                // printf("spliting into line pairs in bank %d\n", i);
                vector<vector<Line*>> lines_in_this_set_split;
                int number_of_line_pairs = ceil(lines_in_this_set.size() / 2.0);
                for (unsigned j = 0; j < (unsigned)number_of_line_pairs; j++) {
                    vector<Line*> line_pair;
                    if (j * 2 + 1 < lines_in_this_set.size()) {
                        line_pair.push_back(lines_in_this_set[j * 2]);
                        line_pair.push_back(lines_in_this_set[j * 2 + 1]);
                    } else {
                        line_pair.push_back(lines_in_this_set[j * 2]);
                    }
                    lines_in_this_set_split.push_back(line_pair);
                }

                // for each pair of lines, create a XORedLine object
                for (vector<Line*> line_pair : lines_in_this_set_split) {
                    XORedLine* xor_line = new XORedLine(line_pair);
                    // printf("created XORedLine object, adding to bank %d, size=%ld\n", i, m_lines[i].size());
                    m_lines[set_ind].push_back(xor_line);
                    count++;
                }
                set_ind++;
            }
            m_num_xored_lines = count;

        }
    }

    RandSetXORCache(const Cache& cache) :
    RandSetXORCache(cache, 12345)
    {
    }

    ~RandSetXORCache()
    {
        for (unsigned i = 0; i < m_lines.size(); i++) {
            vector<XORedLine*> lines = m_lines[i];
            for (XORedLine* line : lines) {
                delete line;
            }
        }
    }

    double get_bit_entropy() const;


    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;

    void print() const;

};

class HashXORCache : public BaseCache
{
    public:
    vector<XORedLine*> m_lines; // one d vector
    int m_num_xored_lines;
    int m_num_false_positive;
    int m_uncompressed_size;

    HashXORCache(const ClusteredCache& cc, unsigned seed) :
    BaseCache(cc.m_num_banks, cc.m_size_per_bank_KB, cc.m_assoc, cc.m_line_size, cc.m_shift_bank, cc.m_shift_set)
    {
        m_uncompressed_size = cc.get_size();
        int count = 0;
        int false_positive_count = 0;
        // vector<Line*> singular_lines;
        for (unsigned i = 0; i < cc.m_clusters.size(); i++) {
            unsigned num_lines_this_bin = cc.m_clusters[i].size();
            if (num_lines_this_bin == 0) continue;

            vector<Line*> lines_in_this_bin;
            for (unsigned j = 0; j < cc.m_clusters[i].size(); j++) {
                lines_in_this_bin.push_back(cc.m_clusters[i][j]);
            }

            default_random_engine rng(seed);
            shuffle(lines_in_this_bin.begin(), lines_in_this_bin.end(), rng);

            // split lines_in_this_bin into a vector of vectors of size 2
            // printf("spliting into line pairs in cluster %d, num lines %ld\n", i, cc.m_clusters[i].size());
            vector<vector<Line*>> lines_in_this_bin_split;
            int number_of_line_pairs = ceil(lines_in_this_bin.size() / 2.0);
            for (unsigned j = 0; j < (unsigned)number_of_line_pairs; j++) {
                vector<Line*> line_pair;
                if (j * 2 + 1 < lines_in_this_bin.size()) {
                    line_pair.push_back(lines_in_this_bin[j * 2]);
                    line_pair.push_back(lines_in_this_bin[j * 2 + 1]);
                } else {
                    line_pair.push_back(lines_in_this_bin[j * 2]);
                }
                lines_in_this_bin_split.push_back(line_pair);
            }

            // for each pair of lines, create a XORedLine object
            for (vector<Line*> line_pair : lines_in_this_bin_split) {
                // printf("number of lines in pair: %ld\n", line_pair.size());
                // for (Line* line : line_pair) {
                //     line->print();
                // }
                
                // check if the pair is a false positive
                if (line_pair.size() == 2) {
                    int line1_true_fingerprint_size_in_bit;
                    int line2_true_fingerprint_size_in_bit;
                    u_int8_t * line1_true_fingerprint = cc.m_clustering_manager->get_line_true_fingerprint(line_pair[0], line1_true_fingerprint_size_in_bit);
                    u_int8_t * line2_true_fingerprint = cc.m_clustering_manager->get_line_true_fingerprint(line_pair[1], line2_true_fingerprint_size_in_bit);
                    

                    // test if they are equal
                    assert(line1_true_fingerprint_size_in_bit == line2_true_fingerprint_size_in_bit);
                    bool mismatch = (memcmp(line1_true_fingerprint, line2_true_fingerprint, int(ceil(line1_true_fingerprint_size_in_bit/8.0))) != 0);
                    if (mismatch){
                        false_positive_count++;
                    }

                    // if (!mismatch) {
                    //     // print these two fingerprints
                    //     printf("line1_true_fingerprint_size_in_bit: %d\n", line1_true_fingerprint_size_in_bit);
                        
                    //     // print line1 original data
                    //     printf("line1 original data: ");
                    //     for (int i = 0; i < m_line_size; i++) {
                    //         printf("%02x ", line_pair[0]->m_segs[i]);
                    //     }
                    //     printf("\n");

                    //     // print line2 original data
                    //     printf("line2 original data: ");
                    //     for (int i = 0; i < m_line_size; i++) {
                    //         printf("%02x ", line_pair[1]->m_segs[i]);
                    //     }
                    //     printf("\n");

                    //     printf("line1_true_fingerprint: ");
                    //     for (int i = 0; i < int(ceil(line1_true_fingerprint_size_in_bit/8.0)); i++) {
                    //         printf("%02x ", line1_true_fingerprint[i]);
                    //     }
                    //     printf("\n");
                    //     printf("line2_true_fingerprint: ");
                    //     for (int i = 0; i < int(ceil(line2_true_fingerprint_size_in_bit/8.0)); i++) {
                    //         printf("%02x ", line2_true_fingerprint[i]);
                    //     }
                    //     printf("\n\n\n");
                    // }

                    delete line1_true_fingerprint;
                    delete line2_true_fingerprint;
                }
                
                XORedLine* xor_line = new XORedLine(line_pair);
                // printf("created XORedLine object, adding to cache, size=%ld\n", m_lines.size());
                // xor_line->print();
                m_lines.push_back(xor_line);
                count++;
            }
            
        }
        m_num_xored_lines = count;
        m_num_false_positive = false_positive_count;
    }
    HashXORCache(const BankedClusteredCache& cc, unsigned seed) :
    BaseCache(cc.m_num_banks, cc.m_size_per_bank_KB, cc.m_assoc, cc.m_line_size, cc.m_shift_bank, cc.m_shift_set)
    {
        m_uncompressed_size = cc.get_size();
        int count = 0;
        int false_positive_count = 0;
        // vector<Line*> singular_lines;
        for (unsigned i = 0; i < cc.m_bank_clusters.size(); i++) {
            for (unsigned j = 0; j < cc.m_bank_clusters[i].size(); j++) {
                unsigned num_lines_this_bin = cc.m_bank_clusters[i][j].size();
                if (num_lines_this_bin == 0) continue;

                vector<Line*> lines_in_this_bin;
                for (unsigned k = 0; k < cc.m_bank_clusters[i][j].size(); k++) {
                    lines_in_this_bin.push_back(cc.m_bank_clusters[i][j][k]);
                }

                default_random_engine rng(seed);
                shuffle(lines_in_this_bin.begin(), lines_in_this_bin.end(), rng);

                // split lines_in_this_bin into a vector of vectors of size 2
                // printf("spliting into line pairs in cluster %d, num lines %ld\n", i, cc.m_clusters[i].size());
                vector<vector<Line*>> lines_in_this_bin_split;
                int number_of_line_pairs = ceil(lines_in_this_bin.size() / 2.0);
                for (unsigned j = 0; j < (unsigned)number_of_line_pairs; j++) {
                    vector<Line*> line_pair;
                    if (j * 2 + 1 < lines_in_this_bin.size()) {
                        line_pair.push_back(lines_in_this_bin[j * 2]);
                        line_pair.push_back(lines_in_this_bin[j * 2 + 1]);
                    } else {
                        line_pair.push_back(lines_in_this_bin[j * 2]);
                    }
                    lines_in_this_bin_split.push_back(line_pair);
                }

                // for each pair of lines, create a XORedLine object
                for (vector<Line*> line_pair : lines_in_this_bin_split) {
                    // printf("number of lines in pair: %ld\n", line_pair.size());
                    // for (Line* line : line_pair) {
                    //     line->print();
                    // }
                    
                    // check if the pair is a false positive
                    if (line_pair.size() == 2) {
                        int line1_true_fingerprint_size_in_bit;
                        int line2_true_fingerprint_size_in_bit;
                        u_int8_t * line1_true_fingerprint = cc.m_clustering_manager->get_line_true_fingerprint(line_pair[0], line1_true_fingerprint_size_in_bit);
                        u_int8_t * line2_true_fingerprint = cc.m_clustering_manager->get_line_true_fingerprint(line_pair[1], line2_true_fingerprint_size_in_bit);
                        

                        // test if they are equal
                        assert(line1_true_fingerprint_size_in_bit == line2_true_fingerprint_size_in_bit);
                        bool mismatch = (memcmp(line1_true_fingerprint, line2_true_fingerprint, int(ceil(line1_true_fingerprint_size_in_bit/8.0))) != 0);
                        if (mismatch){
                            false_positive_count++;
                        }

                        // if (!mismatch) {
                        //     // print these two fingerprints
                        //     printf("line1_true_fingerprint_size_in_bit: %d\n", line1_true_fingerprint_size_in_bit);
                            
                        //     // print line1 original data
                        //     printf("line1 original data: ");
                        //     for (int i = 0; i < m_line_size; i++) {
                        //         printf("%02x ", line_pair[0]->m_segs[i]);
                        //     }
                        //     printf("\n");

                        //     // print line2 original data
                        //     printf("line2 original data: ");
                        //     for (int i = 0; i < m_line_size; i++) {
                        //         printf("%02x ", line_pair[1]->m_segs[i]);
                        //     }
                        //     printf("\n");

                        //     printf("line1_true_fingerprint: ");
                        //     for (int i = 0; i < int(ceil(line1_true_fingerprint_size_in_bit/8.0)); i++) {
                        //         printf("%02x ", line1_true_fingerprint[i]);
                        //     }
                        //     printf("\n");
                        //     printf("line2_true_fingerprint: ");
                        //     for (int i = 0; i < int(ceil(line2_true_fingerprint_size_in_bit/8.0)); i++) {
                        //         printf("%02x ", line2_true_fingerprint[i]);
                        //     }
                        //     printf("\n\n\n");
                        // }

                        delete line1_true_fingerprint;
                        delete line2_true_fingerprint;
                    }
                    
                    XORedLine* xor_line = new XORedLine(line_pair);
                    // printf("created XORedLine object, adding to cache, size=%ld\n", m_lines.size());
                    // xor_line->print();
                    m_lines.push_back(xor_line);
                    count++;
                }
            
            }
                
        }
        m_num_xored_lines = count;
        m_num_false_positive = false_positive_count;
    }
    HashXORCache(const ClusteredCache& cc) :
    HashXORCache(cc, 12345)
    {
    }


    ~HashXORCache()
    {
        for (XORedLine* line : m_lines) {
            delete line;
        }
    }

    // stats
    double get_bit_entropy() const;
    double get_hamming_distance() const; // for only those xored
    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;
    double get_false_positive_rate() const;
    vector<double> * get_per_byte_entropy() const;
    vector<double> * get_per_byte_entropy_only_thoses_xored() const;

    void print() const;

};

class HashDeltaCache : public BaseCache
{
    public:
    vector<DeltaLine*> m_lines; // one d vector
    int m_num_delta_lines;
    int m_num_false_positive;
    int m_uncompressed_size;

    HashDeltaCache(const ClusteredCache& cc, unsigned seed) :
    BaseCache(cc.m_num_banks, cc.m_size_per_bank_KB, cc.m_assoc, cc.m_line_size, cc.m_shift_bank, cc.m_shift_set)
    {
        m_uncompressed_size = cc.get_size();
        int count = 0;
        // vector<Line*> singular_lines;
        for (unsigned i = 0; i < cc.m_clusters.size(); i++) {
            unsigned num_lines_this_bin = cc.m_clusters[i].size();
            if (num_lines_this_bin == 0) continue;

            vector<Line*> lines_in_this_bin;
            for (unsigned j = 0; j < cc.m_clusters[i].size(); j++) {
                lines_in_this_bin.push_back(cc.m_clusters[i][j]);
            }

            default_random_engine rng(seed);
            shuffle(lines_in_this_bin.begin(), lines_in_this_bin.end(), rng);

            // split lines_in_this_bin into a vector of vectors of size 2
            // printf("spliting into line pairs in cluster %d, num lines %ld\n", i, cc.m_clusters[i].size());
            vector<vector<Line*>> lines_in_this_bin_split;
            int number_of_line_pairs = ceil(lines_in_this_bin.size() / 2.0);
            for (unsigned j = 0; j < (unsigned)number_of_line_pairs; j++) {
                vector<Line*> line_pair;
                if (j * 2 + 1 < lines_in_this_bin.size()) {
                    line_pair.push_back(lines_in_this_bin[j * 2]);
                    line_pair.push_back(lines_in_this_bin[j * 2 + 1]);
                } else {
                    line_pair.push_back(lines_in_this_bin[j * 2]);
                }
                lines_in_this_bin_split.push_back(line_pair);
            }

            // for each pair of lines, create a XORedLine object
            for (vector<Line*> line_pair : lines_in_this_bin_split) {
                // printf("number of lines in pair: %ld\n", line_pair.size());
                // for (Line* line : line_pair) {
                //     line->print();
                // }
                DeltaLine* delta_line = new DeltaLine(line_pair);
                // printf("created XORedLine object, adding to cache, size=%ld\n", m_lines.size());
                // xor_line->print();
                m_lines.push_back(delta_line);
                count++;
            }
            
        }
        m_num_delta_lines = count;
    }
    HashDeltaCache(const ClusteredCache& cc) :
    HashDeltaCache(cc, 12345)
    {
    }


    ~HashDeltaCache()
    {
        for (DeltaLine* line : m_lines) {
            delete line;
        }
    }

    double get_bit_entropy() const;
    double get_hamming_distance() const; // for only those xored
    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;
    double get_false_positive_rate() const;
    vector<double> * get_per_byte_entropy() const;

    void print() const;

};

class IdealBankXORCache : public BaseCache
{
    public:
    vector<XORedLine*> m_lines; // one d vector
    int m_uncompressed_size;
    BDICompressor * m_intra_compressor;
    int m_num_xored_lines;


    IdealBankXORCache(const Cache& cache, BDICompressor * compressor) : 
    BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set),
    m_intra_compressor(compressor)
    {
        m_uncompressed_size = cache.get_size();
        int max_rank = m_intra_compressor->get_max_rank();
        for (int i = 0; i < m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add all line, iterate through all lines in this bank
            vector<Line*> uncompressed_lines_in_this_bank;
            for (int j = 0; j < m_num_sets; j++) {
                for (Line* line : cache.m_lines[i][j]) {
                    uncompressed_lines_in_this_bank.push_back(line);
                }
            }

            // iterate through all uncompressed lines in this bank
            while (uncompressed_lines_in_this_bank.size() > 1) {
                if (uncompressed_lines_in_this_bank.size() % 100 == 0) {
                    // printf("    remaining size = %ld\n", uncompressed_lines_in_this_bank.size());
                }
                Line* line1 = uncompressed_lines_in_this_bank[0];
                Line* line2;

                bool found_min_rank_pair = false;
                for (int k = 0; k < max_rank; k++) {
                    // printf("        checking rank %d\n", k);
                    if (found_min_rank_pair) {
                        break;
                    }
                    for (unsigned j = 1; j < uncompressed_lines_in_this_bank.size(); j++) {
                        line2 = uncompressed_lines_in_this_bank[j];
                        vector<Line*> line_pair;
                        line_pair.push_back(line1);
                        line_pair.push_back(line2);
                        XORedLine* xor_line = new XORedLine(line_pair);
                        int rank = m_intra_compressor->get_rank(xor_line);

                        // find a line pair with minimum rank
                        if (rank == k) {
                            m_lines.push_back(xor_line);
                            m_num_xored_lines += 1;
                            // remove line1 and line2 from uncompressed_lines_in_this_bank
                            uncompressed_lines_in_this_bank.erase(uncompressed_lines_in_this_bank.begin() + j);
                            uncompressed_lines_in_this_bank.erase(uncompressed_lines_in_this_bank.begin());
                            found_min_rank_pair = true;
                            break;

                        } else {
                            delete xor_line;
                        }
                    }
                }
            }

            if (uncompressed_lines_in_this_bank.size() == 1) {
                // add the last line to m_lines
                vector<Line*> line_pair;
                line_pair.push_back(uncompressed_lines_in_this_bank[0]);
                m_lines.push_back(new XORedLine(line_pair));
            }
        }
    }


    ~IdealBankXORCache()
    {
        delete m_intra_compressor;
        for (XORedLine* line : m_lines) {
            delete line;
        }
    }

    double get_bit_entropy() const;

    double get_hamming_distance() const;
    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;
    vector<double> * get_per_byte_entropy() const;
    vector<double> * get_per_byte_entropy_only_thoses_xored() const;


    void print() const;

};

#endif // CACHE_XORCACHE_HH