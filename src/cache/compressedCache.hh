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
                    // Apply your compression algorithm here and create a CompressedLine object
                    BPCLine* compressedLine = m_intraCompressor->compress_a_line(line);
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
    vector<vector<Line*>> m_banked_centroids; // bank, bin
    vector<vector<int>> m_banked_cntrs;
    vector<vector<vector<ThesaurusLine*>>> m_lines; // bank, bin
    ThesaurusCompressor* m_interCompressor;
    ThesaurusLSHash * m_lsh;
    
    int m_uncompressed_size;
    int m_num_lines;
    int m_num_compressed_lines;

    ThesaurusCompressedCache(const Cache& cache, unsigned seed) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_uncompressed_size = 0;
        m_interCompressor = new ThesaurusCompressor();
        m_lsh = new ThesaurusLSHash(12, cache.m_line_size, seed);
        // m_lsh->print();
        for (int i = 0; i < m_num_banks; i++) {
            // printf("bank %d\n", i);
            // add a vector of size 2^12
            vector<vector<ThesaurusLine*>> lines_this_bank(4096,vector<ThesaurusLine*>(0));
            vector<Line*> centroids_this_bank(4096);
            vector<int> cntrs_this_bank(4096);

            // iterate through all the lines in this bank
            for (unsigned j = 0; j < cache.m_lines[i].size(); j++) {
                // printf("set %d\n", j);
                for (Line* line : cache.m_lines[i][j]) {
                    m_uncompressed_size += line->m_size;
                    m_num_lines++;
                    // apply thesaurus lsh on the line
                    int fingerprint_size_in_bit;
                    u_int8_t * lshfp = m_lsh->hash(line->m_segs, (line->m_size)*8, fingerprint_size_in_bit);
                    int fingerprint_size_in_byte = (fingerprint_size_in_bit + 7) / 8;
                    assert (fingerprint_size_in_byte <= 2);
                    unsigned fingerprint = 0;
                    memcpy(&fingerprint, lshfp, ceil(fingerprint_size_in_bit/8.0)); 

                    assert(fingerprint < 4096);
                    // printf("    fingerprint %u, ", fingerprint);
                    
                    // check if centroid exists
                    if (centroids_this_bank[fingerprint] == NULL) {
                        // printf("new centroid added\n");
                        // create a new centroid
                        centroids_this_bank[fingerprint] = new Line(line->m_size, line->m_set, line->m_bank, 
                            line->m_addr, line->m_segs);
                        cntrs_this_bank[fingerprint] = 1;
                    } else {
                        // printf("found existing centroid\n");
                        // update the centroid
                        cntrs_this_bank[fingerprint]++;
                        ThesaurusLine * tLine = m_interCompressor->compress_a_line(line, centroids_this_bank[fingerprint]);
                        if (tLine->m_compressed_size < line->m_size) {
                            m_num_compressed_lines++;
                        }
                        lines_this_bank[fingerprint].push_back(tLine);
                    }

                }
            }
            // printf("bank %d done\n", i);
            m_lines.push_back(lines_this_bank);
            m_banked_centroids.push_back(centroids_this_bank);
            m_banked_cntrs.push_back(cntrs_this_bank);
        }
        // printf("done with thesaurus constructor\n");
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


        for (unsigned i=0; i < m_banked_centroids.size(); i++) {
            for (unsigned j=0; j < m_banked_centroids[i].size(); j++) {
                // delete lines in each set
                if (m_banked_centroids[i][j] != NULL) {
                    delete m_banked_centroids[i][j];
                }
            }
            m_banked_centroids[i].clear();
        }
        m_banked_centroids.clear();
        // printf("done deleting banked centroids\n");


        for (unsigned i=0; i < m_banked_cntrs.size(); i++) {
            m_banked_cntrs[i].clear();
        }
        m_banked_cntrs.clear();

        // printf("done deleting thesaurus compressed cache\n");

        delete m_interCompressor;
        delete m_lsh;
    }
    
    void print() const;

    int get_compressed_size() const;
    int get_uncompressed_size() const;
    double get_compression_ratio() const;

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
    BaseCompressedXORCache(const IdealBankXORCache& xorcache, Tcompressor* compressor) : 
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
    BaseCompressedXORCache(const IdealSetXORCache& xorcache, Tcompressor* compressor) : 
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
    BDICompressedXORCache(const IdealBankXORCache& xorcache, bool use_little_e, bool allow_immo) 
        : BaseCompressedXORCache<BDICompressor, BDILine>(xorcache, new BDICompressor(use_little_e, allow_immo)), 
            m_use_little_endian(use_little_e), m_allow_immo(allow_immo)
    {
    }
    BDICompressedXORCache(const IdealSetXORCache& xorcache, bool use_little_e, bool allow_immo) 
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
    ~BPCCompressedXORCache()
    {
    }
    void print() const;
};

#endif // CACHE_COMPRESSEDCACHE_HH