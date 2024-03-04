#ifndef CACHE_CACHE_HH
#define CACHE_CACHE_HH

#include "common/type/type.hh"
#include "cache/line.hh"
#include "compression/hashFunction.hh"
#include <vector>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <unordered_map>

// BaseCache is a class that contains the basic information of the organization of a cache but no lines are stored
class BaseCache
{
    public:
        BaseCache(int num_banks, int KB_per_bank, int assoc, int line_size, int shift_bank, int shift_set)
        {
            m_num_banks = num_banks;
            m_size_per_bank_KB = KB_per_bank;
            m_assoc = assoc;
            m_line_size = line_size;
            m_shift_bank = shift_bank;
            m_shift_set = shift_set;
            m_num_sets = (m_num_banks * KB_per_bank * 1024) / (assoc * line_size);

            int line_size_bits = int(floor(log2(m_line_size)));
            m_bank_start = line_size_bits + m_shift_bank;
            m_bank_end = m_bank_start + int(floor(log2(m_num_banks)))-1;
            // printf("m_bank_start: %d, m_bank_end: %d\n", m_bank_start, m_bank_end);
            m_set_start = line_size_bits + m_shift_set;
            m_set_end = m_set_start + int(floor(log2(m_num_sets)))-1;
            // printf("m_set_start: %d, m_set_end: %d\n", m_set_start, m_set_end);

            // assert(m_bank_end <= m_set_end);
            // assert(m_bank_start >= m_set_start);

        }

        ~BaseCache() {
        }

        int get_set_from_addr(addr_t addr) const;
        int get_bank_from_addr(addr_t addr) const;
        void print();

        int m_num_banks;
        int m_line_size; // in byte
        int m_assoc;
        int m_size_per_bank_KB; // in KB
        int m_num_sets;

        int m_bank_start;
        int m_bank_end;
        int m_set_start;
        int m_set_end;

        int m_shift_bank;
        int m_shift_set;
};

// Cache is a class extends BaseCache and contains the lines of the cache, organized by bank and set
class Cache : public BaseCache
{
    public:
        // should be able to construct a cache from basecache
        Cache(int num_banks, 
            int KB_per_bank, 
            int assoc, 
            int line_size, 
            int shift_bank, 
            int shift_set) : BaseCache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set)
        {
            m_lines_populated = false;
            for (int i = 0; i < m_num_banks; i++) {
                m_lines.push_back(vector<vector<Line*>>());
                for (int j = 0; j < m_num_sets; j++) {
                    m_lines[i].push_back(vector<Line*>());
                }
            }
        }

        void populate_lines(vector<string> filenames_data, vector<string> filenames_addr);

        double get_bit_entropy() const;
        vector<double> * get_per_byte_entropy() const;
        
        ~Cache()
        {
            for (int i=0; i < m_num_banks; i++) {
                for (int j=0; j < m_num_sets; j++) {
                    // delete lines in each set
                    for (long unsigned int k=0; k < m_lines[i][j].size(); k++) {
                        delete m_lines[i][j][k];
                    }
                    m_lines[i][j].clear();
                }
                m_lines[i].clear();
            }
            m_lines.clear();
        }

        Cache& operator=(const Cache& obj);

        int get_size() const;
        int get_num_lines() const;
        
        void print();

        vector<vector<vector<Line*>>> m_lines;
        bool m_lines_populated;
};

class ClusteringManager
{
    public:
        bool m_cascade_hash_functions; // by default, cascade hash functions, otherwise concactenate hash functions
        int m_num_hash_functions;
        vector<HashFunction*> m_hash_functions;
        int m_num_funct_true_fingerprint; // number of hash functions to use for true fingerprint
        int m_funct_to_concact; // if concact, how many hash functions to concact

        ClusteringManager(vector<HashFunction*> hash_functions,int num_funct_true_fingerprint)
        {
            m_num_hash_functions = hash_functions.size();
            m_hash_functions = hash_functions;
            m_cascade_hash_functions = true;
            m_num_funct_true_fingerprint = num_funct_true_fingerprint;
        }
        ClusteringManager(vector<HashFunction*> hash_functions, 
            bool cascade_hash_functions, int funct_to_concact, 
            int num_funct_true_fingerprint)
        {
            m_num_hash_functions = hash_functions.size();
            m_hash_functions = hash_functions;
            m_cascade_hash_functions = cascade_hash_functions;
            m_funct_to_concact = funct_to_concact;
            m_num_funct_true_fingerprint = num_funct_true_fingerprint;
        }
        ~ClusteringManager()
        {
            for (unsigned i = 0; i < m_hash_functions.size(); i++) {
                delete m_hash_functions[i];
            }
        }
        u_int8_t * get_line_true_fingerprint(Line * line, int & true_fingerprint_size)const;
        u_int8_t * compute_cascaded_fingerprint_for_a_line(Line * line, int & fingerprint_size);
        u_int8_t * compute_cascaded_fingerprint_for_a_line_true(Line * line, int & fingerprint_size) const;
        u_int8_t * compute_concact_fingerprint_for_a_line(Line * line, int & fingerprint_size);
        u_int8_t * compute_concact_fingerprint_for_a_line_true(Line * line, int & fingerprint_size) const;
        u_int8_t * get_fingerprint_of_a_line(Line * line, int & fingerprint_size);
        void print() const;
};

// ClusteredCache is a class that extends BaseCache and contains the lines of the cache, organized by fingerprint generated by
// a custom hash function
class ClusteredCache : public Cache
{
    public:
        u_int64_t m_num_clusters;
        vector<vector<Line*>> m_clusters;
        int m_clusters_populated;
        unordered_map<u_int64_t, int> m_fingerprint_to_cluster;
        ClusteringManager * m_clustering_manager;
        
        ClusteredCache(int num_banks, 
            int KB_per_bank, 
            int assoc, 
            int line_size, 
            int shift_bank, 
            int shift_set,
            u_int64_t numclusters,
            vector<HashFunction*> hash_functions,
            int num_funct_true_fingerprint) : 
            Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set),
            m_clustering_manager(new ClusteringManager(hash_functions, num_funct_true_fingerprint))
        {
            m_num_clusters = numclusters;
            m_clusters_populated = false;
        }
        ClusteredCache(int num_banks, 
            int KB_per_bank, 
            int assoc, 
            int line_size, 
            int shift_bank, 
            int shift_set,
            u_int64_t numclusters,
            vector<HashFunction*> hash_functions, 
            bool cascade_hash_functions, int funct_to_concact, 
            int num_funct_true_fingerprint) : 
            Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set),
            m_clustering_manager(new ClusteringManager(hash_functions, cascade_hash_functions, funct_to_concact, num_funct_true_fingerprint))
        {
            m_num_clusters = numclusters;
            m_clusters_populated = false;
        }
        ~ClusteredCache()
        {
            for (unsigned i = 0; i < m_clusters.size(); i++) {
                m_clusters[i].clear();
            }
            m_clusters.clear();
            m_fingerprint_to_cluster.clear();
            delete m_clustering_manager;
        }
        void populate_lines(vector<string> filenames_data, vector<string> filenames_addr);
        void populate_clusters();
        void print();
};

class BankedClusteredCache : public Cache
{
    public:
        u_int64_t m_num_clusters;
        vector<vector<vector<Line*>>> m_bank_clusters;
        int m_clusters_populated;
        vector<unordered_map<u_int64_t, int>> m_bank_fingerprint_to_cluster;
        ClusteringManager * m_clustering_manager;
        
        BankedClusteredCache(int num_banks, 
            int KB_per_bank, 
            int assoc, 
            int line_size, 
            int shift_bank, 
            int shift_set,
            u_int64_t numclusters,
            vector<HashFunction*> hash_functions,
            int num_funct_true_fingerprint) : 
            Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set),
            m_clustering_manager(new ClusteringManager(hash_functions, num_funct_true_fingerprint))
        {
            m_num_clusters = numclusters;
            m_clusters_populated = false;
        }
        BankedClusteredCache(int num_banks, 
            int KB_per_bank, 
            int assoc, 
            int line_size, 
            int shift_bank, 
            int shift_set,
            u_int64_t numclusters,
            vector<HashFunction*> hash_functions, 
            bool cascade_hash_functions, int funct_to_concact, 
            int num_funct_true_fingerprint) : 
            Cache(num_banks, KB_per_bank, assoc, line_size, shift_bank, shift_set),
            m_clustering_manager(new ClusteringManager(hash_functions, cascade_hash_functions, funct_to_concact, num_funct_true_fingerprint))
        {
            m_num_clusters = numclusters;
            m_clusters_populated = false;
        }
        ~BankedClusteredCache()
        {
            for (unsigned i = 0; i < m_bank_clusters.size(); i++) {
                for (unsigned j = 0; j < m_bank_clusters[i].size(); j++) {
                    m_bank_clusters[i][j].clear();
                }
                m_bank_clusters[i].clear();
            }
            m_bank_clusters.clear();

            for (unsigned i = 0; i < m_bank_fingerprint_to_cluster.size(); i++) {
                m_bank_fingerprint_to_cluster[i].clear();
            }
            m_bank_fingerprint_to_cluster.clear();
            
            delete m_clustering_manager;
        }
        void populate_lines(vector<string> filenames_data, vector<string> filenames_addr);
        void populate_clusters();
        void print();
};



#endif // CACHE_CACHE_HH