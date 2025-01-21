#include "cache/compressedCache.hh"
#include "common/vector/vector.hh"

//////////////////////////////////// BPC ////////////////////////////////////
void
BPCCompressedCache::print() const
{
    printf("BPCCompressedCache ");
    
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    printf("coverage: %.2f, compression_ratio: %f (%d/%d)\n", 
        get_coverage_rate(),compression_ratio, uncompressed_size, compressed_size);

    printf("[ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    
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
    double mean_lines_per_bank = calculateMeanLinesPerBank(num_lines_per_bank);
    double mean_lines_per_set = calculateMeanLinesPerBank(num_lines_per_set);
    double std_dev_lines_per_bank = calculateStandardDeviation(num_lines_per_bank);
    double std_dev_lines_per_set = calculateStandardDeviation(num_lines_per_set);
    printf("num_unempty_bank: %ld, mean_lines_per_bank: %f, std_dev_lines_per_bank: %f\n", 
        num_lines_per_bank.size(), mean_lines_per_bank, std_dev_lines_per_bank);
    printf("num_unempty_set: %ld, mean_lines_per_set: %f, std_dev_lines_per_set: %f\n", 
        num_lines_per_set.size(), mean_lines_per_set, std_dev_lines_per_set);

    m_intraCompressor->print();

    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                m_lines[i][j][k]->print();
            }
        }
    }
}

int 
BPCCompressedCache::get_compressed_size() const
{
    int size = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                size += m_lines[i][j][k]->get_compressed_size();
            }
        }
    }
    return size;
}

int
BPCCompressedCache::get_uncompressed_size() const
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

double 
BPCCompressedCache::get_compression_ratio() const
{
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

int 
BPCCompressedCache::get_num_lines() const
{
    int num_lines = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            num_lines += m_lines[i][j].size();
        }
    }
    return num_lines;
}

int 
BPCCompressedCache::get_num_compressed_lines() const
{
    int num_lines = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                if (m_lines[i][j][k]->is_compressed()) num_lines++;
            }
        }
    }
    return num_lines;
}

double 
BPCCompressedCache::get_coverage_rate() const
{
    double rate = (double)get_num_compressed_lines() / (double)get_num_lines();
    return rate;
}

//////////////////////////////////// BDI ////////////////////////////////////
void
BDICompressedCache::print() const
{
    printf("BDICompressedCache ");
    
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    printf("coverage: %.2f, compression_ratio: %f (%d/%d)\n", 
        get_coverage_rate(),compression_ratio, uncompressed_size, compressed_size);

    printf("[ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    
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
    double mean_lines_per_bank = calculateMeanLinesPerBank(num_lines_per_bank);
    double mean_lines_per_set = calculateMeanLinesPerBank(num_lines_per_set);
    double std_dev_lines_per_bank = calculateStandardDeviation(num_lines_per_bank);
    double std_dev_lines_per_set = calculateStandardDeviation(num_lines_per_set);
    printf("num_unempty_bank: %ld, mean_lines_per_bank: %f, std_dev_lines_per_bank: %f\n", 
        num_lines_per_bank.size(), mean_lines_per_bank, std_dev_lines_per_bank);
    printf("num_unempty_set: %ld, mean_lines_per_set: %f, std_dev_lines_per_set: %f\n", 
        num_lines_per_set.size(), mean_lines_per_set, std_dev_lines_per_set);

    m_intraCompressor->print();

    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                m_lines[i][j][k]->print();
            }
        }
    }
}


int 
BDICompressedCache::get_compressed_size() const
{
    int size = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                size += m_lines[i][j][k]->get_compressed_size();
            }
        }
    }
    return size;
}

int
BDICompressedCache::get_uncompressed_size() const
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

double 
BDICompressedCache::get_compression_ratio() const
{
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

int 
BDICompressedCache::get_num_lines() const
{
    int num_lines = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            num_lines += m_lines[i][j].size();
        }
    }
    return num_lines;
}

int 
BDICompressedCache::get_num_compressed_lines() const
{
    int num_lines = 0;
    for (int i = 0; i < m_num_banks; i++) {
        for (int j = 0; j < m_num_sets; j++) {
            for (long unsigned int k = 0; k < m_lines[i][j].size(); k++) {
                if (m_lines[i][j][k]->is_compressed()) num_lines++;
            }
        }
    }
    return num_lines;
}

double 
BDICompressedCache::get_coverage_rate() const
{
    double rate = (double)get_num_compressed_lines() / (double)get_num_lines();
    return rate;
}

//////////////////////////////////// Thesaurus ////////////////////////////////////
void
ThesaurusCompressedCache::print() const
{
    printf("ThesaurusCompressedCache ");
    
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    printf("coverage: %.2f, compression_ratio: %f (%d/%d)\n", 
        get_coverage_rate(),compression_ratio, uncompressed_size, compressed_size);

    printf("[ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    
    
    m_interCompressor->print();

    printf("compressed bank lines: \n");
    for (unsigned i = 0; i < m_lines.size(); i++) {
        for (unsigned j = 0; j < m_lines[i].size(); j++) {
            for (unsigned k = 0; k < m_lines[i][j].size(); k++) {
                m_lines[i][j][k]->print();
            }
        }
    }
    printf("bank centroids: \n");
    for (unsigned i = 0; i < m_banked_centroids.size(); i++) {
        for (unsigned j = 0; j < m_banked_centroids[i].size(); j++) {
            if (m_banked_centroids[i][j] == nullptr) continue;
            m_banked_centroids[i][j]->print();
            printf("cntr=%d\n", m_banked_cntrs[i][j]);
        }
    }
}

int 
ThesaurusCompressedCache::get_compressed_size() const
{
    int size = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        for (unsigned j = 0; j <  m_lines[i].size(); j++) {
            for (unsigned k = 0; k < m_lines[i][j].size(); k++) {
                size += m_lines[i][j][k]->get_compressed_size();
            }
        }
    }
    for (unsigned i = 0; i < m_banked_centroids.size(); i++) {
        for (unsigned j = 0; j < m_banked_centroids[i].size(); j++) {
            if (m_banked_centroids[i][j] == nullptr) continue;
            size += m_banked_centroids[i][j]->m_size;
            size += 2;// for counter
        }
    }
    return size;
}

int
ThesaurusCompressedCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}

double 
ThesaurusCompressedCache::get_compression_ratio() const
{
    int compressed_size = get_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

int 
ThesaurusCompressedCache::get_num_lines() const
{
    return m_num_lines;
}

int 
ThesaurusCompressedCache::get_num_compressed_lines() const
{
    return m_num_compressed_lines;
}

double 
ThesaurusCompressedCache::get_coverage_rate() const
{
    double rate = (double)get_num_compressed_lines() / (double)get_num_lines();
    return rate;
}

//////////////////////////////// BaseCompressedXOR ////////////////////////////////////////
template <typename Tcompressor, typename Tline> void 
BaseCompressedXORCache<Tcompressor, Tline>::print() const
{
    printf("BaseCompressedXORCache\n");
}

template <typename Tcompressor, typename Tline> int 
BaseCompressedXORCache<Tcompressor, Tline>::get_all_compressed_size() const
{
    int size = 0;
    for (long unsigned int k = 0; k < m_intra_lines.size(); k++) {
        size += m_intra_lines[k]->get_compressed_size();
    }
    return size;
}

template <typename Tcompressor, typename Tline> int 
BaseCompressedXORCache<Tcompressor, Tline>::get_xor_compressed_size() const
{
    return m_xor_compress_size;
}

template <typename Tcompressor, typename Tline> int 
BaseCompressedXORCache<Tcompressor, Tline>::get_uncompressed_size() const
{
    return m_uncompressed_size;
}

template <typename Tcompressor, typename Tline> double 
BaseCompressedXORCache<Tcompressor, Tline>::get_total_compression_ratio() const
{
    int compressed_size = get_all_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

template <typename Tcompressor, typename Tline> double 
BaseCompressedXORCache<Tcompressor, Tline>::get_inter_compression_ratio() const
{
    int compressed_size = get_xor_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

template <typename Tcompressor, typename Tline> double 
BaseCompressedXORCache<Tcompressor, Tline>::get_intra_compression_ratio() const
{
    int compressed_size = get_all_compressed_size();
    int uncompressed_size = get_xor_compressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}

template <typename Tcompressor, typename Tline> int 
BaseCompressedXORCache<Tcompressor, Tline>::get_num_lines() const
{
    return m_intra_lines.size();
}

template <typename Tcompressor, typename Tline> int 
BaseCompressedXORCache<Tcompressor, Tline>::get_num_compressed_lines() const
{
    int num_lines = 0;
    for (long unsigned int k = 0; k < m_intra_lines.size(); k++) {
        if (m_intra_lines[k]->is_compressed()) num_lines++;
    }
    return num_lines;
}

template <typename Tcompressor, typename Tline> double 
BaseCompressedXORCache<Tcompressor, Tline>::get_coverage_rate() const
{
    double rate = (double)get_num_compressed_lines() / (double)get_num_lines();
    return rate;
}

void
BDICompressedXORCache::print() const
{
    printf("BDICompressedXORCache ");
    printf("[ coverage %.2f, inter_compression_ratio: %f (%d/%d), intra_compression_ratio: %f (%d/%d), total_compression_ratio: %f (%d/%d) ]\n", 
        get_coverage_rate(),
        get_inter_compression_ratio(), get_xor_compressed_size(), get_uncompressed_size(), 
        get_intra_compression_ratio(), get_all_compressed_size(), get_xor_compressed_size(), 
        get_total_compression_ratio(), get_all_compressed_size(), get_uncompressed_size());

    printf("[ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    
   
    m_intraCompressor->print();

    for (long unsigned int k = 0; k < m_intra_lines.size(); k++) {
        m_intra_lines[k]->print();
    }
}

void
BPCCompressedXORCache::print() const
{
    printf("BPCCompressedXORCache ");
    printf("[ coverage %.2f, inter_compression_ratio: %f (%d/%d), intra_compression_ratio: %f (%d/%d), total_compression_ratio: %f (%d/%d) ]\n", 
        get_coverage_rate(),
        get_inter_compression_ratio(), get_xor_compressed_size(), get_uncompressed_size(), 
        get_intra_compression_ratio(), get_all_compressed_size(), get_xor_compressed_size(), 
        get_total_compression_ratio(), get_all_compressed_size(), get_uncompressed_size());

    printf("[ num_banks: %d, KB_per_bank: %d, assoc: %d, numset: %d, line_size: %d, shift_bank: %d, shift_set: %d, bank bits: [%d,%d], set bits: [%d,%d] ]\n", 
        m_num_banks, m_size_per_bank_KB, m_assoc, m_num_sets, m_line_size, m_shift_bank, m_shift_set, m_bank_start, m_bank_end, m_set_start, m_set_end);
    
   
    m_intraCompressor->print();

    for (long unsigned int k = 0; k < m_intra_lines.size(); k++) {
        m_intra_lines[k]->print();
    }
}

void 
ThesaurusCompressedXORCache::print() const
{
    printf("ThesaurusCompressedXORCache\n");
}

int
ThesaurusCompressedXORCache::get_all_compressed_size() const
{
    return m_all_compressed_size;
}
int
ThesaurusCompressedXORCache::get_xor_compressed_size() const
{
    return m_xor_compress_size;
}
int
ThesaurusCompressedXORCache::get_uncompressed_size() const
{
    return m_uncompressed_size;
}
double
ThesaurusCompressedXORCache::get_total_compression_ratio() const
{
    int compressed_size = get_all_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}
double
ThesaurusCompressedXORCache::get_inter_compression_ratio() const
{
    int compressed_size = get_xor_compressed_size();
    int uncompressed_size = get_uncompressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}
double
ThesaurusCompressedXORCache::get_intra_compression_ratio() const
{
    int compressed_size = get_all_compressed_size();
    int uncompressed_size = get_xor_compressed_size();
    double compression_ratio =  (double)uncompressed_size / (double)compressed_size;
    return compression_ratio;
}
int
ThesaurusCompressedXORCache::get_num_lines() const
{
    int num_lines = 0;
    for (long unsigned int i = 0; i < m_banked_intra_lines.size(); i++) {
        for (long unsigned int j = 0; j < m_banked_intra_lines[i].size(); j++) {
            for (long unsigned int k = 0; k < m_banked_intra_lines[i][j].size(); k++) {
                num_lines++;
            }
        }
    }
    return num_lines;
}
int
ThesaurusCompressedXORCache::get_num_compressed_lines() const
{
    return m_num_intra_compressed_lines;
}
double
ThesaurusCompressedXORCache::get_coverage_rate() const
{
    double rate = (double)get_num_compressed_lines() / (double)get_num_lines();
    return rate;
}
