#ifndef COMPRESSION_INTERCOMPRESSOR_HH
#define COMPRESSION_INTERCOMPRESSOR_HH

#include "cache/line.hh"
#include "cache/compressedLine.hh"

template <class T>
class BaseInterCompressor
{
public:
    BaseInterCompressor() {};
    virtual ~BaseInterCompressor() {};
    virtual T * compress_a_line(Line* line, Line* ref) = 0;
    bool is_line_compressible_zero(Line* line) const;
    virtual void print() const = 0;

    // get compressed size ranking of a line
    virtual int get_rank(Line* line, Line* ref) = 0;
    // get the maximum rank of a line, i.e., uncompressed rank
    virtual int get_max_rank() const = 0;
};
class ThesaurusCompressor : public BaseInterCompressor <ThesaurusLine>
{
public:
    ThesaurusCompressor() {};
    ~ThesaurusCompressor() {};
    ThesaurusLine * compress_a_line(Line* line, Line* ref);

    void print() const;
    
    int get_rank(Line* line, Line* ref);
    int get_max_rank() const;

};
#endif
