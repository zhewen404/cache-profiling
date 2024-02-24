#include "cache/cache.hh"
#include <stdio.h>
#include <assert.h>
#include <unordered_map>
#include "common/file/file_read.hh"
#include "common/bit/bitvec.hh"
#include "common/vector/vector.hh"

int 
BaseCache::get_set_from_addr(addr_t addr) const
{
    return (int)bitExtractedInclusive(addr, m_set_end, m_set_start);
}

int 
BaseCache::get_bank_from_addr(addr_t addr) const
{
    return (int)bitExtractedInclusive(addr, m_bank_end, m_bank_start);
}

void
BaseCache::print()
{
    printf("BaseCache [ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
}

void 
Cache::populate_lines(vector<string> filenames_data, vector<string> filenames_addr)
{
    assert (filenames_data.size() == filenames_addr.size());
    for (long unsigned int i=0; i < filenames_addr.size(); i++) {
        string filename_data = filenames_data[i];
        string filename_addr = filenames_addr[i];

        // printf("filename_data: %s\n", filename_data.c_str());
        // printf("filename_addr: %s\n", filename_addr.c_str());
        unsigned data_size;
        unsigned addr_size;
        u_int8_t * bank_bytes = read_bytes_from_file(&data_size, filename_data);
        u_int64_t * bank_addrs = read_addr_from_file(&addr_size, filename_addr);
        int num_lines_per_file = data_size / m_line_size;
        assert((unsigned)num_lines_per_file == addr_size);
        for (int j=0; j < num_lines_per_file; j++) {
            addr_t line_addr = bank_addrs[j];
            int line_set = get_set_from_addr(line_addr);
            int line_bank = get_bank_from_addr(line_addr);
            u_int8_t * bytes = &bank_bytes[j*m_line_size];
            Line * line;
            line = new Line(m_line_size, line_set, line_bank, line_addr, bytes);

            // line.copyData(bytes);
            // m_lines[line_bank][line_set].emplace_back(m_line_size, line_set, line_bank, line_addr, bytes);
            m_lines[line_bank][line_set].push_back(line);
        }
        free(bank_bytes);
        free(bank_addrs);
    }
    m_lines_populated = true;
}

double
Cache::get_bit_entropy() const
{
    unsigned num1s = 0;
    unsigned num0s = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                unsigned num1 = countSetBits(m_lines[i][j][k]->m_segs, m_lines[i][j][k]->m_size);
                unsigned num0 = m_lines[i][j][k]->m_size * 8 - num1;
                num1s += num1;
                num0s += num0;
            }
        }
    }
    vector<int> vec = {(int)num1s, (int)num0s};
    double entropy = calculateEntropy(vec);
    return entropy;
}

vector<double> * 
Cache::get_per_byte_entropy() const
{
    vector<double> * entropies = new vector<double>();
    for (int b=0; b < m_line_size; b++) { // for every byte
        unordered_map<u_int8_t, int> byte_count;
        for (int i = 0; i < m_num_banks; i++) {
            for (int j = 0; j < m_num_sets; j++) {
                for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                    u_int8_t byte = m_lines[i][j][k]->m_segs[b];
                    if (byte_count.find(byte) == byte_count.end()) {
                        byte_count[byte] = 1;
                    } else {
                        byte_count[byte]++;
                    }
                }
            }
        }
        vector <int> vec;
        for (auto it = byte_count.begin(); it != byte_count.end(); it++) {
            vec.push_back(it->second);
        }
        double entropy = calculateEntropy(vec);
        entropies->push_back(entropy);
    }
    return entropies;
}

int 
Cache::get_size() const
{
    int size = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                size += m_lines[i][j][k]->m_size;
            }
        }
    }
    return size;
}
int 
Cache::get_num_lines() const
{
    int num = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            num += m_lines[i][j].size();
        }
    }
    return num;
}

void
Cache::print()
{
    printf("Cache [ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    printf("bit entropy: %f\n", get_bit_entropy());
    vector<int> num_lines_per_bank;
    vector<int> num_lines_per_set;
    for (int i = 0; i < m_num_banks; i++) {
        int num_lines_this_bank = 0;
        for (int j = 0; j < m_num_sets; j++) {
            num_lines_this_bank += m_lines[i][j].size();
            if (m_lines[i][j].size() != 0) {
                num_lines_per_set.push_back(m_lines[i][j].size());
            }
            // for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
            // }
        }
        if (num_lines_this_bank != 0) {
            num_lines_per_bank.push_back(num_lines_this_bank);
        }
    }
    // // print the two vectors
    // for (long unsigned int i=0; i < num_lines_per_bank.size(); i++) {
    //     printf("num_lines_per_bank[%lu]: %d\n", i, num_lines_per_bank[i]);
    // }
    // for (long unsigned int i=0; i < num_lines_per_set.size(); i++) {
    //     printf("num_lines_per_set[%lu]: %d\n", i, num_lines_per_set[i]);
    // }
    double mean_lines_per_bank = calculateMean(num_lines_per_bank);
    double mean_lines_per_set = calculateMean(num_lines_per_set);
    double std_dev_lines_per_bank = calculateStandardDeviation(num_lines_per_bank);
    double std_dev_lines_per_set = calculateStandardDeviation(num_lines_per_set);
    printf("num_unempty_bank: %ld, mean_lines_per_bank: %f, std_dev_lines_per_bank: %f\n", 
        num_lines_per_bank.size(), mean_lines_per_bank, std_dev_lines_per_bank);
    printf("num_unempty_set: %ld, mean_lines_per_set: %f, std_dev_lines_per_set: %f\n", 
        num_lines_per_set.size(), mean_lines_per_set, std_dev_lines_per_set);

    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                m_lines[i][j][k]->print();
            }
        }
    }
}


void
ClusteredCache::populate_clusters()
{
    if (!m_lines_populated) {
        printf("Error: lines not populated yet\n");
        return;
    }
    if (m_num_hash_functions == 0 || m_hash_functions.empty() || m_hash_functions[0] == nullptr) {
        printf("Error: hash functions has not been initialized correctly\n");
        return;
    }
    // resize vector size based on number of cluster
    // m_clusters.resize();
    // create a map to map fingerprint to cluster index

    // iterate through all lines
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                Line * line = m_lines[i][j][k];
                u_int8_t * fingerprint;
                int fingerprint_size;

                if (m_cascade_hash_functions) {
                    fingerprint = compute_cascaded_fingerprint_for_a_line(line, fingerprint_size);
                } else {
                    fingerprint = compute_concact_fingerprint_for_a_line(line, fingerprint_size);
                }


                // // print the fingerprint
                // printf("fingerprint %d,%d,%ld: ", i, j, k);
                // for (int i = 0; i < ceil(fingerprint_size/8.0); i++) {
                //     // print hex
                //     printf("%02x ", fingerprint[i]);
                // }
                // printf("\n");

                // convert fingerprint to cluster index
                // add line to cluster
                if (fingerprint_size >= 64) {
                    printf("Error: fingerprint size %d is greater than or equal to 64\n", fingerprint_size);
                }
                assert(fingerprint_size < 64);
                
                u_int64_t fp64 = 0;
                memcpy(&fp64, fingerprint, ceil(fingerprint_size/8.0));
                // u_int64_t cluster_index = fp64;
                // printf("fp64: %lu, m_num_cluster: %lu\n", fp64, m_num_clusters);
                assert(fp64 < m_num_clusters);

                // translate fingerprint to cluster index
                int id = -1;
                if (m_fingerprint_to_cluster.find(fp64) == m_fingerprint_to_cluster.end()) {
                    // did not find match
                    id = m_clusters.size();
                    m_fingerprint_to_cluster[fp64] = id; // add to map
                    m_clusters.push_back(vector<Line*>());
                } else{
                    id = m_fingerprint_to_cluster[fp64];
                }

                m_clusters[id].push_back(line);
                delete [] fingerprint;
            }
        }
    }
    m_clusters_populated = true;
}

u_int8_t *
ClusteredCache::get_line_true_fingerprint(Line * line, int& true_fingerprint_size) const
{
    u_int8_t * fingerprint;
    if (m_cascade_hash_functions) {
        fingerprint = compute_cascaded_fingerprint_for_a_line_true(line, true_fingerprint_size);
    } else {
        fingerprint = compute_concact_fingerprint_for_a_line_true(line, true_fingerprint_size);
    }
    return fingerprint;
}

u_int8_t * 
ClusteredCache::compute_cascaded_fingerprint_for_a_line(Line * line, int& fingerprint_size)
{
    u_int8_t * fingerprint;
    // int fingerprint_size;
    // m_hash_functions[0]->print();
    fingerprint = m_hash_functions[0]->hash(line->m_segs, line->m_size*8, fingerprint_size);

    for (int h=1; h < m_num_hash_functions; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(fingerprint, fingerprint_size, fingerprint_size_new);
        
        delete [] fingerprint;
        fingerprint = fingerprint_new;
        fingerprint_size = fingerprint_size_new;
    }
    return fingerprint;
}

u_int8_t * 
ClusteredCache::compute_cascaded_fingerprint_for_a_line_true(Line * line, int& fingerprint_size) const
{
    int num_funct_executed = 0;
    u_int8_t * fingerprint;
    // int fingerprint_size;
    // m_hash_functions[0]->print();
    fingerprint = m_hash_functions[0]->hash(line->m_segs, line->m_size*8, fingerprint_size);
    num_funct_executed++;
    if (num_funct_executed == m_num_funct_true_fingerprint) {
        return fingerprint;
    }

    for (int h=1; h < m_num_hash_functions; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(fingerprint, fingerprint_size, fingerprint_size_new);
        
        delete [] fingerprint;
        fingerprint = fingerprint_new;
        fingerprint_size = fingerprint_size_new;
        num_funct_executed++;
        if (num_funct_executed == m_num_funct_true_fingerprint) {
            return fingerprint;
        }
    }
    return fingerprint;
}


// assume that all functions are concactenated but the last one is cascaded on the whole thing
u_int8_t * 
ClusteredCache::compute_concact_fingerprint_for_a_line(Line * line, int& fingerprint_size)
{
    u_int8_t * fingerprint;
    fingerprint_size = 0;
    // m_hash_functions[0]->print();
    int fingerprint_size_i;
    fingerprint = m_hash_functions[0]->hash(line->m_segs, line->m_size*8, fingerprint_size_i);
    fingerprint_size += fingerprint_size_i;

    // // print the line data
    // printf("line data: ");
    // for (int i = 0; i < line->m_size; i++) {
    //     // print hex
    //     printf("%02x ", line->m_segs[i]);
    // }
    // printf("\n");


    for (int h=1; h < m_funct_to_concact; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_old = fingerprint_size;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(line->m_segs, line->m_size*8, fingerprint_size_new);
        fingerprint_size += fingerprint_size_new;

        // concatenate fingerprint_new to fingerprint
        u_int8_t * fingerprint_concat = new u_int8_t[int(ceil(fingerprint_size/8.0))];
        memset(fingerprint_concat, 0, int(ceil(fingerprint_size/8.0)));
        
        memcpy(fingerprint_concat, fingerprint, int(ceil(fingerprint_size_old/8.0)));
        
        // for every bit in fingerprint_new, set the corresponding bit in fingerprint_concat
        for (int i = 0; i < fingerprint_size_new; i++) {
            int byte_ind = (fingerprint_size_old + i) / 8;
            int bit_ind = (fingerprint_size_old + i) % 8;
            int byte_ind_new = i / 8;
            int bit_ind_new = i % 8;
            fingerprint_concat[byte_ind] |= ((fingerprint_new[byte_ind_new] >> bit_ind_new) & 1) << bit_ind;
        }
        
        delete [] fingerprint;
        fingerprint = fingerprint_concat;
        delete [] fingerprint_new;
    }
    // // print the fingerprint
    // printf("fingerprint after concact: ");
    // for (int i = 0; i < ceil(fingerprint_size/8.0); i++) {
    //     // print hex
    //     printf("%02x ", fingerprint[i]);
    // }
    // printf("\n");

    // cascade the last few hash functions
    for (int h=m_funct_to_concact; h < m_num_hash_functions; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(fingerprint, fingerprint_size, fingerprint_size_new);
        
        delete [] fingerprint;
        fingerprint = fingerprint_new;
        fingerprint_size = fingerprint_size_new;
    }
    // u_int8_t * fingerprint_final = m_hash_functions[m_num_hash_functions-1]->hash(fingerprint, fingerprint_size, fingerprint_size);
    // delete [] fingerprint;

    // print the fingerprint
    // printf("fingerprint after cascade: ");
    // for (int i = 0; i < ceil(fingerprint_size/8.0); i++) {
    //     // print hex
    //     printf("%02x ", fingerprint[i]);
    // }
    // printf("\n");

    return fingerprint;
}

u_int8_t * 
ClusteredCache::compute_concact_fingerprint_for_a_line_true(Line * line, int& fingerprint_size) const
{
    int num_funct_executed = 0;
    u_int8_t * fingerprint;
    fingerprint_size = 0;
    // m_hash_functions[0]->print();
    int fingerprint_size_i;
    fingerprint = m_hash_functions[0]->hash(line->m_segs, line->m_size*8, fingerprint_size_i);
    fingerprint_size += fingerprint_size_i;

    // // print the line data
    // printf("line data: ");
    // for (int i = 0; i < line->m_size; i++) {
    //     // print hex
    //     printf("%02x ", line->m_segs[i]);
    // }
    // printf("\n");
    num_funct_executed++;
    if (num_funct_executed == m_num_funct_true_fingerprint) {
        return fingerprint;
    }


    for (int h=1; h < m_funct_to_concact; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_old = fingerprint_size;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(line->m_segs, line->m_size*8, fingerprint_size_new);
        fingerprint_size += fingerprint_size_new;

        // concatenate fingerprint_new to fingerprint
        u_int8_t * fingerprint_concat = new u_int8_t[int(ceil(fingerprint_size/8.0))];
        memset(fingerprint_concat, 0, int(ceil(fingerprint_size/8.0)));
        
        memcpy(fingerprint_concat, fingerprint, int(ceil(fingerprint_size_old/8.0)));
        
        // for every bit in fingerprint_new, set the corresponding bit in fingerprint_concat
        for (int i = 0; i < fingerprint_size_new; i++) {
            int byte_ind = (fingerprint_size_old + i) / 8;
            int bit_ind = (fingerprint_size_old + i) % 8;
            int byte_ind_new = i / 8;
            int bit_ind_new = i % 8;
            fingerprint_concat[byte_ind] |= ((fingerprint_new[byte_ind_new] >> bit_ind_new) & 1) << bit_ind;
        }
        
        delete [] fingerprint;
        fingerprint = fingerprint_concat;
        delete [] fingerprint_new;


        num_funct_executed++;
        if (num_funct_executed == m_num_funct_true_fingerprint) {
            return fingerprint;
        }
    }
    // // print the fingerprint
    // printf("fingerprint after concact: ");
    // for (int i = 0; i < ceil(fingerprint_size/8.0); i++) {
    //     // print hex
    //     printf("%02x ", fingerprint[i]);
    // }
    // printf("\n");

    // cascade the last few hash functions
    for (int h=m_funct_to_concact; h < m_num_hash_functions; h++) {
        u_int8_t * fingerprint_new;
        int fingerprint_size_new;
        fingerprint_new = m_hash_functions[h]->hash(fingerprint, fingerprint_size, fingerprint_size_new);
        
        delete [] fingerprint;
        fingerprint = fingerprint_new;
        fingerprint_size = fingerprint_size_new;

        num_funct_executed++;
        if (num_funct_executed == m_num_funct_true_fingerprint) {
            return fingerprint;
        }
    }
    // u_int8_t * fingerprint_final = m_hash_functions[m_num_hash_functions-1]->hash(fingerprint, fingerprint_size, fingerprint_size);
    // delete [] fingerprint;

    // print the fingerprint
    // printf("fingerprint after cascade: ");
    // for (int i = 0; i < ceil(fingerprint_size/8.0); i++) {
    //     // print hex
    //     printf("%02x ", fingerprint[i]);
    // }
    // printf("\n");

    return fingerprint;
}


void
ClusteredCache::populate_lines(vector<string> filenames_data, vector<string> filenames_addr)
{
    Cache::populate_lines(filenames_data, filenames_addr);
    populate_clusters();
}

void 
ClusteredCache::print()
{
    printf("ClusteredCache [ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d], num_clusters: %lu, num_hash_functions: %d ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end, m_num_clusters, m_num_hash_functions);
    printf("bit entropy: %f\n", get_bit_entropy());
    if (!m_lines_populated || !m_clusters_populated) {
        printf("lines or clusters not populated yet\n");
        return;
    }
    for (unsigned i = 0; i < m_clusters.size(); i++) {
        printf("cluster[%d] size: %ld\n", i, m_clusters[i].size());
    }
}

