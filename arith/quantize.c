#include "quantize.h"
#include <assert.h>
#include "arith40.h"
#include "mem.h"

int quantize_bcd(float in)
{
    assert(in > -0.5 && in < 0.5);
    if (in < -0.3)
    {
        return -15;
    }
    else if (in > 0.3)
    {
        return 15;
    }
    else
    {
        return in * 50;
    }
}

unsigned quantize_a(float in)
{
    assert(in >= 0 && in <= 1);
    return in * 511;
}

float inv_quantize_bcd(int in)
{
    assert(in >= -15 && in <= 15);
    return (float)in / 50;
}

float inv_quantize_a(unsigned in)
{
    assert(in < 512);
    return (float)in / 511;
}

Quantized quantize(DCT dct)
{
    Quantized q;
    NEW(q);

    q->a = quantize_a(dct->a);
    q->b = quantize_bcd(dct->b);
    q->c = quantize_bcd(dct->c);
    q->d = quantize_bcd(dct->d);
    q->Pbar_b = Arith40_index_of_chroma(dct->Pbar_b);
    q->Pbar_r = Arith40_index_of_chroma(dct->Pbar_r);

    return q;
}

DCT inv_quantize(Quantized q)
{
    DCT dct;
    NEW(dct);

    dct->a = inv_quantize_a(q->a);
    dct->b = inv_quantize_bcd(q->b);
    dct->c = inv_quantize_bcd(q->c);
    dct->d = inv_quantize_bcd(q->d);
    dct->Pbar_b = Arith40_chroma_of_index(q->Pbar_b);
    dct->Pbar_r = Arith40_chroma_of_index(q->Pbar_r);

    return dct;
}