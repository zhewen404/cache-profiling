#ifndef FUNCTION_PROFILING_HH
#define FUNCTION_PROFILING_HH
#include <vector>
#include <string>
#include "compression/wordPatternRecognizer.hh"
#include "cache/cache.hh"
using namespace std;

void profiling_entropy_byte_position(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);
void profiling_entropy_byte_position_afterxor_randbank(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);
void profiling_entropy_byte_position_afterxor12_bytemap(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);
void profiling_entropy_byte_position_afterxor12_sparseshuffledbytemap_4_3(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);
void profiling_entropy_byte_position_afterxor12_thesaurus(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);
void profiling_entropy_byte_position_afterxor12_lowentropy_8_16(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &entropies, unsigned seed);

void profiling_histogram_word_pattern_epc(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_strong(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_hycomp(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_semantic(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_density(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_averagebytemsb_32(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_averagebytemsb_24(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);
void profiling_histogram_word_pattern_averagebytemsb_16(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &histogram, unsigned seed);

template <class Treg, typename Tpat>
class WordPatternCacheProfiler
{
    public:
        Treg * m_rec;
        WordPatternCacheProfiler(Treg * rec, const Cache * cache) : m_rec(rec), m_cache(cache) {
        }
        ~WordPatternCacheProfiler() {
            delete m_rec;
        }
        vector<int> profile_cache_word_pattern()
        {
            vector<Tpat> word_pattern_types;
            assert(m_cache->m_lines_populated);
            for (unsigned i = 0; i < m_cache->m_lines.size(); i++) {
                for (unsigned j = 0; j < m_cache->m_lines[i].size(); j++) {
                    for (unsigned k = 0; k < m_cache->m_lines[i][j].size(); k++) {
                        for (int l = 0; l < m_cache->m_line_size/8; l++) {
                            // printf("i: %d, j: %d, k: %d, l: %d\n", i, j, k, l);
                            u_int8_t * word = (m_cache->m_lines[i][j][k])->m_segs+l*8;
                            Tpat type = m_rec->get_word_pattern_type(word);
                            word_pattern_types.push_back(type);
                        }
                    }
                }
            }

            vector<int> hist = get_histogram(word_pattern_types, m_rec->get_num_patterns());
            return hist;
        }
        vector<int> get_histogram(const vector<Tpat> &word_pattern_types, int num_patterns)
        {
            vector<int> histogram = vector<int>(num_patterns, 0);
            for (unsigned i = 0; i < word_pattern_types.size(); i++) {
                histogram[word_pattern_types[i]]++;
            }
            return histogram;
        }
        virtual void print() const = 0;
    private:
        const Cache * m_cache;
};
class EPCWordPatternCacheProfiler : public WordPatternCacheProfiler<EPCWordPatternRecognizer, EPCWordPattern_t>
{
    public:
        EPCWordPatternCacheProfiler(const Cache * cache) : 
            WordPatternCacheProfiler<EPCWordPatternRecognizer, EPCWordPattern_t>(new EPCWordPatternRecognizer(), cache) {
        }
        virtual ~EPCWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("EPCWordPatternCacheProfiler\n");
            m_rec->print();
        }
};
class StrongWordPatternCacheProfiler : public WordPatternCacheProfiler<StrongWordPatternRecognizer, StrongWordPattern_t>
{
    public:
        StrongWordPatternCacheProfiler(const Cache * cache) : 
            WordPatternCacheProfiler<StrongWordPatternRecognizer, StrongWordPattern_t>(new StrongWordPatternRecognizer(), cache) {
        }
        virtual ~StrongWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("StrongWordPatternCacheProfiler\n");
            m_rec->print();
        }
};
class HyCompWordPatternCacheProfiler : public WordPatternCacheProfiler<HyCompWordPatternRecognizer, HyCompWordPattern_t>
{
    public:
        HyCompWordPatternCacheProfiler(const Cache * cache) : 
            WordPatternCacheProfiler<HyCompWordPatternRecognizer, HyCompWordPattern_t>(new HyCompWordPatternRecognizer(), cache) {
        }
        virtual ~HyCompWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("HyCompWordPatternCacheProfiler\n");
            m_rec->print();
        }
};
class SemanticWordPatternCacheProfiler : public WordPatternCacheProfiler<SemanticWordPatternRecognizer, SemanticWordPattern_t>
{
    public:
        SemanticWordPatternCacheProfiler(const Cache * cache) : 
            WordPatternCacheProfiler<SemanticWordPatternRecognizer, SemanticWordPattern_t>(new SemanticWordPatternRecognizer(), cache) {
        }
        virtual ~SemanticWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("SemanticWordPatternCacheProfiler\n");
            m_rec->print();
        }
};
class DensityWordPatternCacheProfiler : public WordPatternCacheProfiler<DensityWordPatternRecognizer, DensityWordPattern_t>
{
    public:
        DensityWordPatternCacheProfiler(const Cache * cache) : 
            WordPatternCacheProfiler<DensityWordPatternRecognizer, DensityWordPattern_t>(new DensityWordPatternRecognizer(), cache) {
        }
        virtual ~DensityWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("DensityWordPatternCacheProfiler\n");
            m_rec->print();
        }
};
class AverageByteMSBWordPatternCacheProfiler : public WordPatternCacheProfiler<AverageByteMSBWordPatternRecognizer, unsigned>
{
    public:
        AverageByteMSBWordPatternCacheProfiler(const Cache * cache, int num_msbs) : 
            WordPatternCacheProfiler<AverageByteMSBWordPatternRecognizer, unsigned>(new AverageByteMSBWordPatternRecognizer(num_msbs), cache) {
        }
        virtual ~AverageByteMSBWordPatternCacheProfiler() {
        }
        void print() const
        {
            printf("AverageByteMSBWordPatternCacheProfiler\n");
            m_rec->print();
        }
};

void profiling_x(int num_banks, int KB_per_bank, string dir, bool only_those_xored, vector <double> &results, unsigned seed, 
    void(*profiling_function)(int, int, string, bool, vector <double> &, double));

#endif