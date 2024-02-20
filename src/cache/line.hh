#ifndef CACHE_LINE_HH
#define CACHE_LINE_HH
#include "common/type/type.hh" //include type.hh
using namespace std;

class Line
{
    public:
        Line(int size, int set, int bank, addr_t addr, u_int8_t * data) : 
            m_size(size), 
            m_set(set), 
            m_bank(bank), 
            m_addr(addr),
            m_alloc(false)
        { 
            if (data != nullptr) {
                copyData(data);
            }
        };

        // Line(const Line &cp);

        ~Line()
        {
            if (m_alloc) {
                delete[] m_segs;
            }
        }

        // Line& operator=(const Line& obj);

        void print() const;

        void copyData(u_int8_t * data);

        int m_size; // in byte
        int m_set;
        int m_bank;
        addr_t m_addr;
        u_int8_t * m_segs;
        bool m_alloc;
};

#endif // CACHE_LINE_HH