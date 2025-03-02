#include "bitpack.h"
#include "assert.h"
#include "stdio.h"

#define MAX_WIDTH 64

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= MAX_WIDTH);
        return width == MAX_WIDTH || n < (1ULL << width);
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= MAX_WIDTH);
        if (width == 0)
        {
                return false;
        }
        int64_t max = 1LL << (width - 1);
        return width == MAX_WIDTH || ((n >= -max) && (n < max));
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        return (word >> lsb) & mask;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        uint64_t value = (word >> lsb) & mask;

        /* If signed bit is on, then sign extend */
        if (value >> (width - 1))
        {
                value |= ~mask;
        }
        return (int64_t)value;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        assert(Bitpack_fitsu(value, width));

        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        uint64_t cleared = word & ~(mask << lsb);

        return cleared | (value << lsb);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value)
{
        assert(Bitpack_fitss(value, width));
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        return Bitpack_newu(word, width, lsb, (uint64_t)value & mask);

        // assert(width <= MAX_WIDTH);
        // assert(width + lsb <= MAX_WIDTH);
        // assert(Bitpack_fitss(value, width));

        // uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        // uint64_t cleared = word & ~(mask << lsb);

        // return cleared | (((uint64_t)value & mask) << lsb);
}
