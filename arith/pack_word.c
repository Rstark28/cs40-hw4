#include "pack_word.h"
#include "bitpack.h"
#include "mem.h"

uint32_t pack_word(Quantized q)
{
    uint32_t word = 0;

    word = Bitpack_newu(word, 9, 23, q->a);
    word = Bitpack_news(word, 5, 18, q->b);
    word = Bitpack_news(word, 5, 13, q->c);
    word = Bitpack_news(word, 5, 8, q->d);
    word = Bitpack_newu(word, 4, 4, q->Pbar_b);
    word = Bitpack_newu(word, 4, 0, q->Pbar_r);

    return word;
}

Quantized unpack_word(uint32_t word)
{
    Quantized q;
    NEW(q);

    q->a = Bitpack_getu(word, 9, 23);
    q->b = Bitpack_gets(word, 5, 18);
    q->c = Bitpack_gets(word, 5, 13);
    q->d = Bitpack_gets(word, 5, 8);
    q->Pbar_b = Bitpack_getu(word, 4, 4);
    q->Pbar_r = Bitpack_getu(word, 4, 0);

    return q;
}