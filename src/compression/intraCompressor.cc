#include "compression/intraCompressor.hh"
#include "common/bit/bitvec.hh"
#include <assert.h> 
#include <inttypes.h>

template <typename T> bool
BaseIntraCompressor<T>::is_line_compressible_zero(Line* line) const
{
    for (int i = 0; i < line->m_size; i++) {
        if (line->m_segs[i] != 0) {
            return false;
        }
    }
    return true;
}

template <typename T> bool
BaseIntraCompressor<T>::is_line_compressible_repeatByte(Line* line) const
{
    int64_t* segs = reinterpret_cast<int64_t*>(line->m_segs);
    int64_t seg0 = segs[0];
    for (int i = 1; i < line->m_size/8; i++) {
        int64_t seg = segs[i];
        if (seg != seg0) {
            return false;
        }
    }
    return true;
}


BDILine * 
BDICompressor::compress_a_line(Line* line)
{
    if (is_line_compressible_zero(line)) {
        return new BDILine(line, true); // Z
    }
    else if (is_line_compressible_repeatByte(line)) {
        return new BDILine(line, 8, line->m_segs); // B
    } else {
        if (m_allow_immo) {
            // 8
            BDILine * imm8x1 = try_compress_line_imm<int64_t>(line, 8, 1);
            if (imm8x1 != nullptr) {
                return imm8x1;
            }

            // 16
            BDILine * imm8x2 = try_compress_line_imm<int64_t>(line, 8, 2);
            if (imm8x2 != nullptr) {
                return imm8x2;
            }
            BDILine * imm16x1 = try_compress_line_imm<int32_t>(line, 4, 1);
            if (imm16x1 != nullptr) {
                return imm16x1;
            }
        }
        
        BDILine * b8d1 = try_compress_line_bdi<int64_t>(line, 8, 1);
        // BDILine * b8d1 = try_compress_line_b8d1(line);
        if (b8d1 != nullptr) {
            return b8d1;
        }

        //20
        BDILine * b4d1 = try_compress_line_bdi<int32_t>(line, 4, 1);
        if (b4d1 != nullptr) {
            return b4d1;
        }

        // 24
        BDILine * b8d2 = try_compress_line_bdi<int64_t>(line, 8, 2);
        if (b8d2 != nullptr) {
            return b8d2;
        }

        if (m_allow_immo) {
            // 32
            BDILine * imm8x4 = try_compress_line_imm<int64_t>(line, 8, 4);
            if (imm8x4 != nullptr) {
                return imm8x4;
            }
            BDILine * imm16x2 = try_compress_line_imm<int32_t>(line, 4, 2);
            if (imm16x2 != nullptr) {
                return imm16x2;
            }
            BDILine * imm32x1 = try_compress_line_imm<int16_t>(line, 2, 1);
            if (imm32x1 != nullptr) {
                return imm32x1;
            }
        }

        // 34
        BDILine * b2d1 = try_compress_line_bdi<int16_t>(line, 2, 1);
        if (b2d1 != nullptr) {
            return b2d1;
        }


        //36
        BDILine * b4d2 = try_compress_line_bdi<int32_t>(line, 4, 2);
        if (b4d2 != nullptr) {
            return b4d2;
        }

        //40
        BDILine * b8d4 = try_compress_line_bdi<int64_t>(line, 8, 4);
        if (b8d4 != nullptr) {
            return b8d4;
        }

    }

    return new BDILine(line, false); // UNC
}

template<typename T>
bool
BDICompressor::int_fall_into_range(T int_num, int test_bit_width) const
{
    if (test_bit_width == 8) {
        return int_num <= INT8_MAX && int_num >= INT8_MIN;
    } else if (test_bit_width == 16) {
        return int_num <= INT16_MAX && int_num >= INT16_MIN;
    } else if (test_bit_width == 32) {
        return int_num <= INT32_MAX && int_num >= INT32_MIN;
    } else {
        assert(false);
        return false;
    }
}


template<typename T>
BDILine * 
BDICompressor::try_compress_line_bdi(Line* line, int base_size, int delta_size)
{

    int num_deltas = line->m_size / base_size;
    int8_t * d_i = new int8_t[num_deltas * delta_size];
    int8_t * base = new int8_t[base_size];
    T base0;
    vector<bool> is_immediate;
    bool found_base = false;

    // T* segs = reinterpret_cast<T*>(line->m_segs); // little endian
    T* segs = reinterpret_segment_base_on_endianness<T>(line, base_size);


    for (int i = 0; i < num_deltas; i++) {
        T segment = segs[i];
        if (int_fall_into_range<T>(segment, delta_size * 8)) {
            is_immediate.push_back(true);
            memcpy(d_i + i * delta_size, line->m_segs + i * base_size, delta_size);
        } else {
            if (!found_base) {
                found_base = true;
                memcpy(base, line->m_segs + i * base_size, base_size);
                base0 = segment;
            }
            assert(found_base);

            // calculate delta
            // T delta = base0 - segs[i];
            T delta = segs[i] - base0;
            if (int_fall_into_range<T>(delta, delta_size * 8)) {
                is_immediate.push_back(false);
                memcpy(d_i + i * delta_size, &delta, delta_size);
            } else {
                // free memory
                delete[] d_i;
                delete[] base;
                if (!m_use_little_endian) {
                    delete[] segs;
                }
                is_immediate.clear();
                return nullptr;
            }
        }
    }
    BDILine * bdiLine = new BDILine(line, base_size, delta_size, is_immediate, base, d_i);
    // free memory
    delete[] d_i;
    delete[] base;
    if (!m_use_little_endian) {
        delete[] segs;
    }

    return bdiLine;
}

template<typename Ts>
Ts* 
BDICompressor::reinterpret_segment_base_on_endianness(Line* line, int base_size)
{
    if (m_use_little_endian) {
        Ts* segs = reinterpret_cast<Ts*>(line->m_segs);
        return segs;
    }
    Ts* segs = new Ts[line->m_size/base_size];
    for (int js = 0; js < line->m_size/base_size; js++) {
        Ts p_seg = 0; 
        for (int j = 0; j < base_size-1; j++) {
            p_seg += line->m_segs[js * base_size + j];
            p_seg = p_seg << 8;
        }
        p_seg += line->m_segs[js * base_size + base_size - 1];
        p_seg = (Ts) p_seg;
        segs[js] = p_seg;
        // printf("%016lx ", segs[js]);
    }
    // printf("\n");
    return segs;
}

template<typename T>
BDILine * 
BDICompressor::try_compress_line_imm(Line* line, int base_size, int delta_size)
{
    int num_deltas = line->m_size / base_size;
    int8_t * d_i = new int8_t[num_deltas * delta_size];

    // T* segs = reinterpret_cast<T*>(line->m_segs); // little endian
    T* segs = reinterpret_segment_base_on_endianness<T>(line, base_size);

    for (int i = 0; i < num_deltas; i++) {
        T segment = segs[i];
        if (int_fall_into_range<T>(segment, delta_size * 8)) {
            memcpy(d_i + i * delta_size, line->m_segs + i * base_size, delta_size);
        } else {
            // free memory
            delete[] d_i;
            if (!m_use_little_endian) {
                delete[] segs;
            }
            return nullptr;
        }
    }
    BDILine *immline = new BDILine(line, base_size, delta_size, d_i);
    // free memory
    delete[] d_i;
    if (!m_use_little_endian) {
        delete[] segs;
    }

    return immline;
}

void
BDICompressor::print() const
{
    printf("[ BDICompressor: little endian %d ]\n", m_use_little_endian);
}

