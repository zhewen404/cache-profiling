#ifndef CACHE_COMPRESSEDCACHE_HH
#define CACHE_COMPRESSEDCACHE_HH
#include "cache/cache.hh"
#include "cache/compressedLine.hh"
#include "compression/intraCompressor.hh"
#include "compression/interCompressor.hh"
#include "cache/xorCache.hh"
#include <vector>

class BPCCompressedCache : public BaseCache
{
public:
    vector<vector<vector<BPCLine*>>> m_lines;
    BitPlaneCompressor* m_intraCompressor;
    BPCCompressedCache(const Cache& cache) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_intraCompressor = new BitPlaneCompressor();

        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<vector<BPCLine*>>());
            for (int j = 0; j < m_num_sets; j++) {
                m_lines[i].push_back(vector<BPCLine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    // line->print();
                    // Apply your compression algorithm here and create a CompressedLine object
                    BPCLine* compressedLine = m_intraCompressor->compress_a_line(line);
                    // compressedLine->print();
                    m_lines[i][j].push_back(compressedLine);
                }
            }
        }
    }
    ~BPCCompressedCache()
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
        delete m_intraCompressor;
    }
    
    void print() const;

    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;

    int get_num_lines() const;
    int get_num_compressed_lines() const;
    double get_coverage_rate() const;
};

class BDICompressedCache : public BaseCache
{
    public:
    vector<vector<vector<BDILine*>>> m_lines;
    BDICompressor* m_intraCompressor;
    BDICompressedCache(const Cache& cache) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_intraCompressor = new BDICompressor();

        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<vector<BDILine*>>());
            for (int j = 0; j < m_num_sets; j++) {
                m_lines[i].push_back(vector<BDILine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    // Apply your compression algorithm here and create a CompressedLine object
                    BDILine* compressedLine = m_intraCompressor->compress_a_line(line);
                    m_lines[i][j].push_back(compressedLine);
                }
            }
        }
    }
    BDICompressedCache(const Cache& cache, bool use_little_endian) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_intraCompressor = new BDICompressor(use_little_endian);

        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<vector<BDILine*>>());
            for (int j = 0; j < m_num_sets; j++) {
                m_lines[i].push_back(vector<BDILine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    // Apply your compression algorithm here and create a CompressedLine object
                    BDILine* compressedLine = m_intraCompressor->compress_a_line(line);
                    m_lines[i][j].push_back(compressedLine);
                }
            }
        }
    }
    
    BDICompressedCache(const Cache& cache, bool use_little_endian, bool allow_immo) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_intraCompressor = new BDICompressor(use_little_endian, allow_immo);

        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<vector<BDILine*>>());
            for (int j = 0; j < m_num_sets; j++) {
                m_lines[i].push_back(vector<BDILine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    // Apply your compression algorithm here and create a CompressedLine object
                    BDILine* compressedLine = m_intraCompressor->compress_a_line(line);
                    m_lines[i][j].push_back(compressedLine);
                }
            }
        }
    }

    ~BDICompressedCache()
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
        delete m_intraCompressor;
    }

    void print() const;

    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;

    int get_num_lines() const;
    int get_num_compressed_lines() const;
    double get_coverage_rate() const;
};

class ThesaurusCompressedCache : public BaseCache
{
    public:
    vector<vector<Line*>> m_cluster_centroid; // bank, bin
    vector<vector<int>> m_cluster_size; // bank, bin
    vector<vector<int>> m_cluster_noncentroid_size_in_byte; // bank, bin

    vector<vector<vector<ThesaurusLine*>>> m_lines; // bank, bin, lines
    ThesaurusCompressor* m_interCompressor;
    ThesaurusLSHash * m_lsh;
    
    int m_num_lines=0;
    int m_before_compression_size = 0;
    vector<vector<int>> m_cluster_num_comp_successful_lines_noncentroid; // bank, bin
    vector<vector<int>> m_cluster_num_comp_failed_lines_noncentroid; // bank, bin
    vector<int> m_bank_num_cluster_centroid; // per bank
    vector<vector<int>> m_cluster_size_of_comp_successful_lines_noncentroid; // bank, bin

    int m_num_centroid_lines=0;
    int m_size_of_centroid_lines=0;

    int m_num_successful_noncentroid_lines=0;
    int m_num_failed_noncentroid_lines=0;
    int m_num_noncentroid_lines=0;

    int m_size_of_successful_noncentroid_lines=0;
    int m_size_of_failed_noncentroid_lines=0;
    int m_size_of_noncentroid_lines=0;

    double m_avg_size_of_successful_noncentroid_lines=0;
    double m_avg_size_of_noncentroid_lines=0;
    int m_after_compression_size=0;


    ThesaurusCompressedCache(const Cache& cache, unsigned seed) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_before_compression_size = 0;
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, cache.m_line_size, seed);
        // m_lsh->print();
        for (int i = 0; i < m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<Line*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);

            int centroid = 0;

            // iterate through all the lines in this bank
            for (unsigned j = 0; j < cache.m_lines[i].size(); j++) {
                // printf("set %d\n", j);
                for (Line* line : cache.m_lines[i][j]) {
                    m_before_compression_size += line->m_size;
                    m_num_lines++;
                    // apply thesaurus lsh on the line
                    int fingerprint_size_in_bit;
                    u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                    int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                    assert (fingerprint_size_in_byte <= 2);
                    unsigned fingerprint = 0;
                    memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                    delete[] lshfp;

                    assert(fingerprint < 4096);
                    // printf("    fingerprint %u, ", fingerprint);
                    
                    // check if centroid exists
                    if (centroids_this_bank[fingerprint] == NULL) {
                        // printf("new centroid added\n");
                        // create a new centroid
                        centroids_this_bank[fingerprint] = new Line(line->m_size, line->m_set, line->m_bank, 
                            line->m_addr, line->m_segs);
                        cntrs_this_bank[fingerprint] = 1;
                        distance_this_bank[fingerprint] = 0;
                        centroid++;

                        successful_this_bank[fingerprint] = 0;
                        failed_this_bank[fingerprint] = 0;
                        size_successful_this_bank[fingerprint] = 0;
                    } else {
                        // printf("found existing centroid\n");
                        // update the centroid
                        cntrs_this_bank[fingerprint]++;
                        ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                        if (tLine->m_compressed_size < line->m_size) {
                            successful_this_bank[fingerprint]++;
                            size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                        } else {
                            failed_this_bank[fingerprint]++;
                        }
                        distance_this_bank[fingerprint] += tLine->m_compressed_size;
                        lines_this_bank[fingerprint].push_back(tLine);
                    }

                }
            }
            // printf("bank %d done\n", i);
            m_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        // printf("done with thesaurus constructor\n");
        calculate_stats();
    }
    ~ThesaurusCompressedCache()
    {
        // printf("deleting thesaurus compressed cache\n");
        for (unsigned i=0; i < m_lines.size(); i++) { //bank
            for (unsigned j=0; j < m_lines[i].size(); j++) { //bin
                for (unsigned k=0; k < m_lines[i][j].size(); k++) { // lines in this bin
                    delete m_lines[i][j][k];
                }
                m_lines[i][j].clear();
            }
            m_lines[i].clear();
        }
        m_lines.clear();
        // printf("done deleting banked centroids\n");


        for (unsigned i=0; i < m_cluster_centroid.size(); i++) {
            for (unsigned j=0; j < m_cluster_centroid[i].size(); j++) {
                // delete lines in each set
                if (m_cluster_centroid[i][j] != NULL) {
                    delete m_cluster_centroid[i][j];
                }
            }
            m_cluster_centroid[i].clear();
        }
        m_cluster_centroid.clear();
        // printf("done deleting banked centroids\n");


        for (unsigned i=0; i < m_cluster_size.size(); i++) {
            m_cluster_size[i].clear();
        }
        m_cluster_size.clear();

        for (unsigned i=0; i < m_cluster_noncentroid_size_in_byte.size(); i++) {
            m_cluster_noncentroid_size_in_byte[i].clear();
        }
        
        for (unsigned i=0; i < m_cluster_num_comp_successful_lines_noncentroid.size(); i++) {
            m_cluster_num_comp_successful_lines_noncentroid[i].clear();
        }

        for (unsigned i=0; i < m_cluster_num_comp_failed_lines_noncentroid.size(); i++) {
            m_cluster_num_comp_failed_lines_noncentroid[i].clear();
        }

        for (unsigned i=0; i < m_cluster_size_of_comp_successful_lines_noncentroid.size(); i++) {
            m_cluster_size_of_comp_successful_lines_noncentroid[i].clear();
        }

        // printf("done deleting thesaurus compressed cache\n");

        delete m_interCompressor;
        delete m_lsh;

    }
    
    void print() const;
    void calculate_stats() {
        for (unsigned i=0; i < m_bank_num_cluster_centroid.size(); i++) {
            m_num_centroid_lines += m_bank_num_cluster_centroid[i];
        }
        m_size_of_centroid_lines = m_line_size * m_num_centroid_lines;

        for (unsigned i=0; i<m_cluster_num_comp_successful_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_num_comp_successful_lines_noncentroid[i].size(); j++) {
                m_num_successful_noncentroid_lines += m_cluster_num_comp_successful_lines_noncentroid[i][j];
            }
        }

        for (unsigned i=0; i<m_cluster_num_comp_failed_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_num_comp_failed_lines_noncentroid[i].size(); j++) {
                m_num_failed_noncentroid_lines += m_cluster_num_comp_failed_lines_noncentroid[i][j];
            }
        }

        m_num_noncentroid_lines = m_num_successful_noncentroid_lines + m_num_failed_noncentroid_lines;

        for (unsigned i=0; i<m_cluster_size_of_comp_successful_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_size_of_comp_successful_lines_noncentroid[i].size(); j++) {
                m_size_of_successful_noncentroid_lines += m_cluster_size_of_comp_successful_lines_noncentroid[i][j];
            }
        }

        m_size_of_failed_noncentroid_lines = m_line_size * m_num_failed_noncentroid_lines;
        m_size_of_noncentroid_lines = m_size_of_successful_noncentroid_lines + m_size_of_failed_noncentroid_lines;

        m_avg_size_of_successful_noncentroid_lines = (double)m_size_of_successful_noncentroid_lines / (double)m_num_successful_noncentroid_lines;
        m_avg_size_of_noncentroid_lines = (double)m_size_of_noncentroid_lines / (double)m_num_noncentroid_lines;

        m_after_compression_size = m_size_of_centroid_lines + m_size_of_noncentroid_lines;
    }
    
    int get_after_compression_size() const;
    int get_before_compression_size() const;
    double get_compression_ratio() const;

    double get_centroid_size() const {return m_size_of_centroid_lines;}
    double get_noncentroid_success_size() const {return m_size_of_successful_noncentroid_lines;}
    double get_noncentroid_fail_size() const {return m_size_of_failed_noncentroid_lines;}

    int get_num_lines() const;
    int get_num_compressed_lines() const;
    double get_coverage_rate() const;
};

///////////////////////////////////////////////
template<class Tcompressor, class Tline>
class BaseCompressedXORCache : public BaseCache
{
    public:
    vector<Tline*> m_intra_lines;
    Tcompressor* m_intraCompressor;
    int m_uncompressed_size;
    int m_xor_compress_size;
    BaseCompressedXORCache(const HashXORCache& xorcache, Tcompressor* compressor) : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_intraCompressor(compressor),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        for (Line* line : xorcache.m_lines) {
            // Apply your compression algorithm here and create a CompressedLine object
            Tline* compressedLine = m_intraCompressor->compress_a_line(line);
            m_intra_lines.push_back(compressedLine);
        }
    }
    BaseCompressedXORCache(const IdealBankXORCache<Tcompressor>& xorcache, Tcompressor* compressor) : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_intraCompressor(compressor),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        for (Line* line : xorcache.m_lines) {
            // Apply your compression algorithm here and create a CompressedLine object
            Tline* compressedLine = m_intraCompressor->compress_a_line(line);
            m_intra_lines.push_back(compressedLine);
        }
    }
    BaseCompressedXORCache(const IdealSetXORCache<Tcompressor>& xorcache, Tcompressor* compressor) : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_intraCompressor(compressor),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        for (Line* line : xorcache.m_lines) {
            // Apply your compression algorithm here and create a CompressedLine object
            Tline* compressedLine = m_intraCompressor->compress_a_line(line);
            m_intra_lines.push_back(compressedLine);
        }
    }
    BaseCompressedXORCache(const RandBankXORCache& xorcache, Tcompressor* compressor) : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_intraCompressor(compressor),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            vector<XORedLine*> lines = xorcache.m_lines[i];
            for (XORedLine* line : lines) {
            // Apply your compression algorithm here and create a CompressedLine object
                Tline* compressedLine = m_intraCompressor->compress_a_line(line);
                m_intra_lines.push_back(compressedLine);
            }
        }
    }
    BaseCompressedXORCache(const RandSetXORCache& xorcache, Tcompressor* compressor) : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_intraCompressor(compressor),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        for (unsigned i = 0; i < xorcache.m_lines.size(); i++) {
            vector<XORedLine*> lines = xorcache.m_lines[i];
            for (XORedLine* line : lines) {
                // Apply your compression algorithm here and create a CompressedLine object
                Tline* compressedLine = m_intraCompressor->compress_a_line(line);
                m_intra_lines.push_back(compressedLine);
            }
        }
    }

    ~BaseCompressedXORCache()
    {
        for (long unsigned int k=0; k < m_intra_lines.size(); k++) {
            delete m_intra_lines[k];
        }
        m_intra_lines.clear();
        delete m_intraCompressor;
    }

    void print() const;

    int get_all_compressed_size() const;
    int get_xor_compressed_size() const;
    int get_uncompressed_size() const;
    double get_total_compression_ratio() const;
    double get_inter_compression_ratio() const;
    double get_intra_compression_ratio() const;

    int get_num_lines() const;
    int get_num_compressed_lines() const;
    double get_coverage_rate() const;
};

class BDICompressedXORCache : public BaseCompressedXORCache<BDICompressor, BDILine>
{
    public:
    bool m_use_little_endian;
    bool m_allow_immo;
    BDICompressedXORCache(const HashXORCache& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }
    BDICompressedXORCache(const IdealBankXORCache<BDICompressor>& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }
    BDICompressedXORCache(const IdealSetXORCache<BDICompressor>& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }

    BDICompressedXORCache(const RandBankXORCache& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }
    BDICompressedXORCache(const RandSetXORCache& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }
    ~BDICompressedXORCache()
    {
    }
    void print() const;
};

class BPCCompressedXORCache : public BaseCompressedXORCache<BitPlaneCompressor, BPCLine>
{
    public:
    BPCCompressedXORCache(const HashXORCache& xorcache) 
        : BaseCompressedXORCache<BitPlaneCompressor, BPCLine>(xorcache, new BitPlaneCompressor())
    {
    }
    BPCCompressedXORCache(const IdealBankXORCache<BitPlaneCompressor>& xorcache) 
        : BaseCompressedXORCache<BitPlaneCompressor, BPCLine>(xorcache, new BitPlaneCompressor())
    {
    }
    BPCCompressedXORCache(const IdealSetXORCache<BitPlaneCompressor>& xorcache) 
        : BaseCompressedXORCache<BitPlaneCompressor, BPCLine>(xorcache, new BitPlaneCompressor())
    {
    }
    BPCCompressedXORCache(const RandBankXORCache& xorcache) 
        : BaseCompressedXORCache<BitPlaneCompressor, BPCLine>(xorcache, new BitPlaneCompressor())
    {
    }
    BPCCompressedXORCache(const RandSetXORCache& xorcache) 
        : BaseCompressedXORCache<BitPlaneCompressor, BPCLine>(xorcache, new BitPlaneCompressor())
    {
    }
    ~BPCCompressedXORCache()
    {
    }
    void print() const;
};

class ThesaurusCompressedXORCache : public BaseCache
{
    public:
    int m_num_lines=0;
    vector<vector<vector<ThesaurusLine*>>> m_banked_intra_lines;
    vector<vector<XORedLine*>> m_cluster_centroid; // bank, bin
    vector<vector<int>> m_cluster_size;
    vector<vector<int>> m_cluster_noncentroid_size_in_byte; // bank, bin
    ThesaurusCompressor* m_interCompressor;
    ThesaurusLSHash * m_lsh;
    int m_uncompressed_size;
    int m_xor_compress_size;

    vector<vector<int>> m_cluster_num_comp_successful_lines_noncentroid; // bank, bin
    vector<vector<int>> m_cluster_num_comp_failed_lines_noncentroid; // bank, bin
    vector<int> m_bank_num_cluster_centroid; // per bank
    vector<vector<int>> m_cluster_size_of_comp_successful_lines_noncentroid; // bank, bin

    int m_num_centroid_lines=0;
    int m_size_of_centroid_lines=0;

    int m_num_successful_noncentroid_lines=0;
    int m_num_failed_noncentroid_lines=0;
    int m_num_noncentroid_lines=0;

    int m_size_of_successful_noncentroid_lines=0;
    int m_size_of_failed_noncentroid_lines=0;
    int m_size_of_noncentroid_lines=0;

    double m_avg_size_of_successful_noncentroid_lines=0;
    double m_avg_size_of_noncentroid_lines=0;
    int m_after_compression_size=0;

    ThesaurusCompressedXORCache(const HashXORCache& xorcache, unsigned seed)  : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, xorcache.m_line_size, seed);
        vector<vector<XORedLine*>> xored_lines_banked;
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            vector<XORedLine*> lines_in_this_bank;
            for (Line* line : xorcache.m_lines) {
                if (line->m_bank == i) {
                    lines_in_this_bank.push_back((XORedLine*)line);
                }
            }
            xored_lines_banked.push_back(lines_in_this_bank);
        }

        for (int i = 0; i < xorcache.m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<XORedLine*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);
            int centroid = 0;

            // iterate through all the lines in this bank
            for (XORedLine* line : xored_lines_banked[i]) {
                // m_uncompressed_size += line->m_size;
                m_num_lines++;
                // apply thesaurus lsh on the line
                int fingerprint_size_in_bit;
                u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                assert (fingerprint_size_in_byte <= 2);
                unsigned fingerprint = 0;
                memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                delete[] lshfp;

                assert(fingerprint < 4096);
                // printf("    fingerprint %u, ", fingerprint);
                
                // check if centroid exists
                if (centroids_this_bank[fingerprint] == NULL) {
                    // printf("new centroid added\n");
                    // create a new centroid
                    centroids_this_bank[fingerprint] = new XORedLine(line->m_line_ptrs);
                    cntrs_this_bank[fingerprint] = 1;
                    distance_this_bank[fingerprint] = 0;
                    centroid++;

                    successful_this_bank[fingerprint] = 0;
                    failed_this_bank[fingerprint] = 0;
                    size_successful_this_bank[fingerprint] = 0;
                } else {
                    // printf("found existing centroid\n");
                    // update the centroid
                    cntrs_this_bank[fingerprint]++;
                    ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                    if (tLine->m_compressed_size < line->m_size) {
                        successful_this_bank[fingerprint]++;
                        size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                    } else {
                        failed_this_bank[fingerprint]++;
                    }
                    lines_this_bank[fingerprint].push_back(tLine);
                    distance_this_bank[fingerprint] += tLine->m_compressed_size;
                }

            }
            // printf("bank %d done\n", i);
            m_banked_intra_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        // printf("done with thesaurus constructor\n");
        
        calculate_stats();
    }
    ThesaurusCompressedXORCache(const IdealBankXORCacheThesaurus& xorcache, unsigned seed)   : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, xorcache.m_line_size, seed);
        vector<vector<XORedLine*>> xored_lines_banked;
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            vector<XORedLine*> lines_in_this_bank;
            for (Line* line : xorcache.m_lines) {
                if (line->m_bank == i) {
                    lines_in_this_bank.push_back((XORedLine*)line);
                }
            }
            xored_lines_banked.push_back(lines_in_this_bank);
        }

        for (int i = 0; i < xorcache.m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<XORedLine*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);
            int centroid = 0;

            // iterate through all the lines in this bank
            for (XORedLine* line : xored_lines_banked[i]) {
                // m_uncompressed_size += line->m_size;
                m_num_lines++;
                // apply thesaurus lsh on the line
                int fingerprint_size_in_bit;
                u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                assert (fingerprint_size_in_byte <= 2);
                unsigned fingerprint = 0;
                memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                delete[] lshfp;

                assert(fingerprint < 4096);
                // printf("    fingerprint %u, ", fingerprint);
                
                // check if centroid exists
                if (centroids_this_bank[fingerprint] == NULL) {
                    // printf("new centroid added, ");
                    // create a new centroid
                    centroids_this_bank[fingerprint] = new XORedLine(line->m_line_ptrs);
                    cntrs_this_bank[fingerprint] = 1;
                    // printf("+%d\n", line->m_size);
                    distance_this_bank[fingerprint] = 0;
                    centroid++;

                    successful_this_bank[fingerprint] = 0;
                    failed_this_bank[fingerprint] = 0;
                    size_successful_this_bank[fingerprint] = 0;
                } else {
                    // printf("found existing centroid, ");
                    // update the centroid
                    cntrs_this_bank[fingerprint]++;
                    ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                    // printf("+%d\n", tLine->m_compressed_size);
                    if (tLine->m_compressed_size < line->m_size) {
                        successful_this_bank[fingerprint]++;
                        size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                    } else {
                        failed_this_bank[fingerprint]++;
                    }
                    lines_this_bank[fingerprint].push_back(tLine);
                    distance_this_bank[fingerprint] += tLine->m_compressed_size;
                }

            }
            // printf("bank %d done\n", i);
            m_banked_intra_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        // printf("done with thesaurus constructor\n");

        calculate_stats();
    }

    template <typename T>
    ThesaurusCompressedXORCache(const IdealSetXORCache<T>& xorcache, unsigned seed)   : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, xorcache.m_line_size, seed);
        vector<vector<XORedLine*>> xored_lines_banked;
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            vector<XORedLine*> lines_in_this_bank;
            for (Line* line : xorcache.m_lines) {
                if (line->m_bank == i) {
                    lines_in_this_bank.push_back((XORedLine*)line);
                }
            }
            xored_lines_banked.push_back(lines_in_this_bank);
        }

        for (int i = 0; i < xorcache.m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<XORedLine*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);
            int centroid = 0;

            // iterate through all the lines in this bank
            for (XORedLine* line : xored_lines_banked[i]) {
                // m_uncompressed_size += line->m_size;
                m_num_lines++;
                // apply thesaurus lsh on the line
                int fingerprint_size_in_bit;
                u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                assert (fingerprint_size_in_byte <= 2);
                unsigned fingerprint = 0;
                memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                delete[] lshfp;

                assert(fingerprint < 4096);
                // printf("    fingerprint %u, ", fingerprint);
                
                // check if centroid exists
                if (centroids_this_bank[fingerprint] == NULL) {
                    // printf("new centroid added\n");
                    // create a new centroid
                    centroids_this_bank[fingerprint] = new XORedLine(line->m_line_ptrs);
                    cntrs_this_bank[fingerprint] = 1;
                    distance_this_bank[fingerprint] = 0;
                    centroid++;

                    successful_this_bank[fingerprint] = 0;
                    failed_this_bank[fingerprint] = 0;
                    size_successful_this_bank[fingerprint] = 0;
                } else {
                    // printf("found existing centroid\n");
                    // update the centroid
                    cntrs_this_bank[fingerprint]++;
                    ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                    if (tLine->m_compressed_size < line->m_size) {
                        successful_this_bank[fingerprint]++;
                        size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                    } else {
                        failed_this_bank[fingerprint]++;
                    }
                    lines_this_bank[fingerprint].push_back(tLine);
                    distance_this_bank[fingerprint] += tLine->m_compressed_size;
                }

            }
            // printf("bank %d done\n", i);
            m_banked_intra_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        // printf("done with thesaurus constructor\n");
        calculate_stats();
        
    }

    ThesaurusCompressedXORCache(const RandBankXORCache& xorcache, unsigned seed)   : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, xorcache.m_line_size, seed);
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<XORedLine*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);
            int centroid = 0;

            // iterate through all the lines in this bank
            for (XORedLine* line : xorcache.m_lines[i]) {
                // m_uncompressed_size += line->m_size;
                m_num_lines++;
                // apply thesaurus lsh on the line
                int fingerprint_size_in_bit;
                u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                assert (fingerprint_size_in_byte <= 2);
                unsigned fingerprint = 0;
                memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                delete[] lshfp;

                assert(fingerprint < 4096);
                // printf("    fingerprint %u, ", fingerprint);
                
                // check if centroid exists
                if (centroids_this_bank[fingerprint] == NULL) {
                    // printf("new centroid added\n");
                    // create a new centroid
                    centroids_this_bank[fingerprint] = new XORedLine(line->m_line_ptrs);
                    cntrs_this_bank[fingerprint] = 1;
                    distance_this_bank[fingerprint] = 0;
                    centroid++;

                    successful_this_bank[fingerprint] = 0;
                    failed_this_bank[fingerprint] = 0;
                    size_successful_this_bank[fingerprint] = 0;
                } else {
                    // printf("found existing centroid\n");
                    // update the centroid
                    cntrs_this_bank[fingerprint]++;
                    ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                    if (tLine->m_compressed_size < line->m_size) {
                        successful_this_bank[fingerprint]++;
                        size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                    } else {
                        failed_this_bank[fingerprint]++;
                    }
                    lines_this_bank[fingerprint].push_back(tLine);
                    distance_this_bank[fingerprint] += tLine->m_compressed_size;
                }

            }
            // printf("bank %d done\n", i);
            m_banked_intra_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        calculate_stats();
    }
    ThesaurusCompressedXORCache(const RandSetXORCache& xorcache, unsigned seed)   : 
        BaseCache(xorcache.m_num_banks, xorcache.m_size_per_bank_KB, xorcache.m_assoc, xorcache.m_line_size, 
            xorcache.m_shift_bank, xorcache.m_shift_set),
        m_uncompressed_size(xorcache.get_uncompressed_size()),
        m_xor_compress_size(xorcache.get_compressed_size())
    {
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, xorcache.m_line_size, seed);
        for (int i = 0; i < xorcache.m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<XORedLine*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);
            vector<int> distance_this_bank(4096);

            vector<int> successful_this_bank(4096);
            vector<int> failed_this_bank(4096);
            vector<int> size_successful_this_bank(4096);
            int centroid = 0;

            // iterate through all the lines in this bank
            for (XORedLine* line : xorcache.m_lines[i]) {
                // m_uncompressed_size += line->m_size;
                m_num_lines++;
                // apply thesaurus lsh on the line
                int fingerprint_size_in_bit;
                u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                assert (fingerprint_size_in_byte <= 2);
                unsigned fingerprint = 0;
                memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 
                delete[] lshfp;

                assert(fingerprint < 4096);
                // printf("    fingerprint %u, ", fingerprint);
                
                // check if centroid exists
                if (centroids_this_bank[fingerprint] == NULL) {
                    // printf("new centroid added\n");
                    // create a new centroid
                    centroids_this_bank[fingerprint] = new XORedLine(line->m_line_ptrs);
                    cntrs_this_bank[fingerprint] = 1;
                    distance_this_bank[fingerprint] = 0;
                    centroid++;

                    successful_this_bank[fingerprint] = 0;
                    failed_this_bank[fingerprint] = 0;
                    size_successful_this_bank[fingerprint] = 0;
                } else {
                    // printf("found existing centroid\n");
                    // update the centroid
                    cntrs_this_bank[fingerprint]++;
                    ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                    if (tLine->m_compressed_size < line->m_size) {
                        successful_this_bank[fingerprint]++;
                        size_successful_this_bank[fingerprint] += tLine->m_compressed_size;
                    } else {
                        failed_this_bank[fingerprint]++;
                    }
                    lines_this_bank[fingerprint].push_back(tLine);
                    distance_this_bank[fingerprint] += tLine->m_compressed_size;
                }

            }
            // printf("bank %d done\n", i);
            m_banked_intra_lines.push_back(lines_this_bank);
            m_cluster_centroid.push_back(centroids_this_bank);
            m_cluster_size.push_back(cntrs_this_bank);
            m_cluster_noncentroid_size_in_byte.push_back(distance_this_bank);

            m_bank_num_cluster_centroid.push_back(centroid);
            m_cluster_num_comp_successful_lines_noncentroid.push_back(successful_this_bank);
            m_cluster_num_comp_failed_lines_noncentroid.push_back(failed_this_bank);
            m_cluster_size_of_comp_successful_lines_noncentroid.push_back(size_successful_this_bank);
        }
        calculate_stats();
    }
    ~ThesaurusCompressedXORCache()
    {
        // printf("deleting thesaurus compressed cache\n");
        for (unsigned i=0; i < m_banked_intra_lines.size(); i++) { //bank
            for (unsigned j=0; j < m_banked_intra_lines[i].size(); j++) { //bin
                for (unsigned k=0; k < m_banked_intra_lines[i][j].size(); k++) { // lines in this bin
                    delete m_banked_intra_lines[i][j][k];
                }
                m_banked_intra_lines[i][j].clear();
            }
            m_banked_intra_lines[i].clear();
        }
        m_banked_intra_lines.clear();
        // printf("done deleting banked centroids\n");


        for (unsigned i=0; i < m_cluster_centroid.size(); i++) {
            for (unsigned j=0; j < m_cluster_centroid[i].size(); j++) {
                // delete lines in each set
                if (m_cluster_centroid[i][j] != NULL) {
                    delete m_cluster_centroid[i][j];
                }
            }
            m_cluster_centroid[i].clear();
        }
        m_cluster_centroid.clear();
        // printf("done deleting banked centroids\n");


        for (unsigned i=0; i < m_cluster_size.size(); i++) {
            m_cluster_size[i].clear();
        }
        m_cluster_size.clear();

        // printf("done deleting thesaurus compressed cache\n");

        delete m_interCompressor;
        delete m_lsh;
    }
    void print() const;
    void calculate_stats() {
        for (unsigned i=0; i < m_bank_num_cluster_centroid.size(); i++) {
            m_num_centroid_lines += m_bank_num_cluster_centroid[i];
        }
        m_size_of_centroid_lines = m_line_size * m_num_centroid_lines;

        for (unsigned i=0; i<m_cluster_num_comp_successful_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_num_comp_successful_lines_noncentroid[i].size(); j++) {
                m_num_successful_noncentroid_lines += m_cluster_num_comp_successful_lines_noncentroid[i][j];
            }
        }

        for (unsigned i=0; i<m_cluster_num_comp_failed_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_num_comp_failed_lines_noncentroid[i].size(); j++) {
                m_num_failed_noncentroid_lines += m_cluster_num_comp_failed_lines_noncentroid[i][j];
            }
        }

        m_num_noncentroid_lines = m_num_successful_noncentroid_lines + m_num_failed_noncentroid_lines;

        for (unsigned i=0; i<m_cluster_size_of_comp_successful_lines_noncentroid.size(); i++) {
            for (unsigned j=0; j<m_cluster_size_of_comp_successful_lines_noncentroid[i].size(); j++) {
                m_size_of_successful_noncentroid_lines += m_cluster_size_of_comp_successful_lines_noncentroid[i][j];
            }
        }

        m_size_of_failed_noncentroid_lines = m_line_size * m_num_failed_noncentroid_lines;
        m_size_of_noncentroid_lines = m_size_of_successful_noncentroid_lines + m_size_of_failed_noncentroid_lines;

        m_avg_size_of_successful_noncentroid_lines = (double)m_size_of_successful_noncentroid_lines / (double)m_num_successful_noncentroid_lines;
        m_avg_size_of_noncentroid_lines = (double)m_size_of_noncentroid_lines / (double)m_num_noncentroid_lines;

        m_after_compression_size = m_size_of_centroid_lines + m_size_of_noncentroid_lines;
    }

    int get_all_compressed_size() const;
    int get_xor_compressed_size() const;
    int get_uncompressed_size() const;
    double get_total_compression_ratio() const;
    double get_inter_compression_ratio() const;
    double get_intra_compression_ratio() const;

    double get_centroid_size() const {return m_size_of_centroid_lines;}
    double get_noncentroid_success_size() const {return m_size_of_successful_noncentroid_lines;}
    double get_noncentroid_fail_size() const {return m_size_of_failed_noncentroid_lines;}

    int get_num_lines() const;
    int get_num_compressed_lines() const;
    double get_coverage_rate() const;
};


#endif // CACHE_COMPRESSEDCACHE_HH