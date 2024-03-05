#ifndef CACHE_COMPRESSEDCACHE_HH
#define CACHE_COMPRESSEDCACHE_HH
#include "cache/cache.hh"
#include "cache/compressedLine.hh"
#include "compression/intraCompressor.hh"
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