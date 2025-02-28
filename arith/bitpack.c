#include "bitpack.h"

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    return (n >> width) == 0;
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
    int max_val = 1 << (width - 1);

    return n >= -max_val && n < max_val;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value);
