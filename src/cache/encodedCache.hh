#ifndef CACHE_ENCODEDCACHE_HH
#define CACHE_ENCODEDCACHE_HH
#include "cache/cache.hh"
#include "cache/encodedLine.hh"
#include "encoder/encoder.hh"
#include "cache/xorCache.hh"
#include <vector>

class DBIEncodedCache : public BaseCache
{
public:
    vector<vector<vector<DBIEncodedLine*>>> m_lines;
    DBIEncoder* m_encoder;
    DBIEncodedCache(const Cache& cache, int granularity_in_bit) : BaseCache(cache.m_num_banks, cache.m_size_per_bank_KB, cache.m_assoc, cache.m_line_size, cache.m_shift_bank, cache.m_shift_set)
    {
        m_encoder = new DBIEncoder(granularity_in_bit);

        for (int i = 0; i < m_num_banks; i++) {
            m_lines.push_back(vector<vector<DBIEncodedLine*>>());
            for (int j = 0; j < m_num_sets; j++) {
                m_lines[i].push_back(vector<DBIEncodedLine*>());
                for (Line* line : cache.m_lines[i][j]) {
                    // line->print();
                    // Apply your encoding algorithm here and create a EncodedLine object
                    DBIEncodedLine* encodedLine = m_encoder->encode_a_line(line);
                    // compressedLine->print();
                    m_lines[i][j].push_back(encodedLine);
                }
            }
        }
    }
    ~DBIEncodedCache()
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
        delete m_encoder;
    }
    
    void print() const {
        printf("DBI Encoded Cache\n");
    }
};

///////////////////////////////////////////////

#endif // CACHE_ENCODEDCACHE_HH