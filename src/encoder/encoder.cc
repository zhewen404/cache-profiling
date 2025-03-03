#include "encoder/encoder.hh"
#include "common/bit/bitvec.hh"
#include <assert.h> 
#include <inttypes.h>

DBIEncodedLine * 
DBIEncoder::encode_a_line(Line* line)
{
    DBIEncodedLine * dbiLine = new DBIEncodedLine(line, m_granularity_in_bit);
    return dbiLine;
}

void
DBIEncoder::print() const
{
    printf("[ DBI (data block inversion) Encoder ]\n");
}
