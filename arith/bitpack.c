#include "bitpack.h"
#include "assert.h"
#include "stdio.h"

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);
        if (width == 64)
                return true;
        return n < ((uint64_t)1 << width);
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= 64);
        if (width == 0 || width == 64)
                return false;
        int64_t max = (int64_t)1 << (width - 1);
        return (n >= -max) && (n < max);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        uint64_t mask = (width == 64) ? ~0ULL : ((1ULL << width) - 1);
        return (word >> lsb) & mask;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        uint64_t mask = ((uint64_t)1 << width) - 1;
        uint64_t value = (word >> lsb) & mask;
        /* If signed bit is on, then sign extend */
        if ((value >> (width - 1)))
        {
                value = value | ~mask;
        }
        return (int64_t)value;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value);

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value);
