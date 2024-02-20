#ifndef COMPRESSION_INTRACOMPRESSOR_HH
#define COMPRESSION_INTRACOMPRESSOR_HH

#include "cache/line.hh"
#include "cache/compressedLine.hh"

template <class T>
class BaseIntraCompressor
{
public:
    BaseIntraCompressor() {};
    virtual ~BaseIntraCompressor() {};
    virtual T * compress_a_line(Line* line) = 0;
    bool is_line_compressible_zero(Line* line) const;
    bool is_line_compressible_repeatByte(Line* line) const;
};


class BDICompressor : public BaseIntraCompressor <BDILine>
{
public:
    BDICompressor(bool little) {
        m_use_little_endian = little;
        m_allow_immo = false;
    };
    BDICompressor(bool little, bool allow_immo) {
        m_use_little_endian = little;
        m_allow_immo = allow_immo;
    };
    BDICompressor() {
        // vanila bdi using little endian and no immediate only patterns
        m_use_little_endian = true;
        m_allow_immo = false;
    };
    ~BDICompressor() {};
    BDILine * compress_a_line(Line* line);


    template<typename T>
    bool int_fall_into_range(T int_num, int test_bit_width) const;

    template<typename T>
    BDILine * try_compress_line_bdi(Line* line, int base_size, int delta_size);

    template<typename Ts>
    Ts* reinterpret_segment_base_on_endianness(Line* line, int base_size);

    // BDILine * try_compress_line_b8d1(Line* line);

    template<typename T>
    BDILine * try_compress_line_imm(Line* line, int base_size, int delta_size);

    void print() const;

    bool m_use_little_endian;
    bool m_allow_immo;
};


#endif