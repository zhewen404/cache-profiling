#include "compression/interCompressor.hh"
#include "common/bit/bitvec.hh"
#include <assert.h> 
#include <inttypes.h>
////// BaseIntraCompressor //////
template <typename T> bool
BaseInterCompressor<T>::is_line_compressible_zero(Line* line) const
{
    for (int i = 0; i < line->m_size; i++) {
        if (line->m_segs[i] != 0) {
            return false;
        }
    }
    return true;
}


//////////// Thesaurus ////////////
ThesaurusLine * 
ThesaurusCompressor::compress_a_line(Line* line, Line* ref)
{
    if (is_line_compressible_zero(line)) {
        return new ThesaurusLine(line);
    }
    ThesaurusLine * tLine = new ThesaurusLine(line, ref, is_line_compressible_zero(line), is_line_compressible_zero(ref));
    return tLine;
}

void
ThesaurusCompressor::print() const
{
    printf("[ ThesaurusCompressor ]\n");
}

int
ThesaurusCompressor::get_rank(Line* line, Line* ref)
{
    ThesaurusLine * tLine = compress_a_line(line, ref);
    int size = tLine->m_compressed_size;
    int rank = size;
    delete tLine;

    return rank;
}

int 
ThesaurusCompressor::get_max_rank() const
{
    return 64; //0-63
}

