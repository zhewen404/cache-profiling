#ifndef COMPRESSION_LINEPATTERNRECOGNIZER_HH
#define COMPRESSION_LINEPATTERNRECOGNIZER_HH
#include "compression/intraCompressor.hh"

template <typename T>
class BaseLinePatternRecognizer
{
    public:
        int m_num_patterns;
        BaseLinePatternRecognizer(int num_patterns) : m_num_patterns(num_patterns) {
        }
        virtual ~BaseLinePatternRecognizer() {}
        virtual T get_line_pattern_type(Line* line) const = 0;
        int get_num_patterns() const 
        {
            return m_num_patterns;
        }
        int get_pattern_size_in_bits() const
        {
            return int(ceil(log2(get_num_patterns())));
        }
        virtual void print() const = 0;
};

template <class Tcompressor>
class IntraCompLinePatternRecognizer : public BaseLinePatternRecognizer<u_int64_t>
{
    public:
        Tcompressor * m_intra_compressor;
        IntraCompLinePatternRecognizer(Tcompressor * compressor) : 
            BaseLinePatternRecognizer<u_int64_t>(-1), 
            m_intra_compressor(compressor) 
        {
            m_num_patterns = m_intra_compressor->get_max_rank()+1;
        }
        
        ~IntraCompLinePatternRecognizer() {
            delete m_intra_compressor;
        }

        u_int64_t get_line_pattern_type(Line* line) const override
        {
            int rank = m_intra_compressor->get_rank(line);
            assert(rank < m_num_patterns);
            return (u_int64_t)rank;
        }

        void print() const
        {
            printf("IntraCompLinePatternRecognizer, total %d patterns\n", m_num_patterns);
        }
};

class BDILinePatternRecognizer : public IntraCompLinePatternRecognizer<BDICompressor>
{
    public:
        BDILinePatternRecognizer(bool little, bool allow_immo) : 
            IntraCompLinePatternRecognizer<BDICompressor>(new BDICompressor(little, allow_immo))
        {
        }

        void print() const
        {
            printf("BDILinePatternRecognizer, total %d patterns\n", m_num_patterns);
        }
};

class BPCLinePatternRecognizer : public IntraCompLinePatternRecognizer<BitPlaneCompressor>
{
    public:
        BPCLinePatternRecognizer() : 
            IntraCompLinePatternRecognizer<BitPlaneCompressor>(new BitPlaneCompressor())
        {
        }

        void print() const
        {
            printf("BPCLinePatternRecognizer, total %d patterns\n", m_num_patterns);
        }
};

#endif