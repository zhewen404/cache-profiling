#ifndef ENCODER_ENCODER_HH
#define ENCODER_ENCODER_HH

#include "cache/line.hh"
#include "cache/encodedLine.hh"

template <class T>
class BaseEncoder
{
public:
    BaseEncoder() {};
    virtual ~BaseEncoder() {};
    virtual T * encode_a_line(Line* line) = 0;
    virtual void print() const = 0;
};

// https://ieeexplore.ieee.org/document/4801969
class DBIEncoder : public BaseEncoder <DBIEncodedLine>
{
public:
    int m_granularity_in_bit;
    DBIEncoder(int granularity_in_bit) {
        m_granularity_in_bit = granularity_in_bit;
    };
    ~DBIEncoder() {};
    DBIEncodedLine * encode_a_line(Line* line);
    void print() const;
};

#endif