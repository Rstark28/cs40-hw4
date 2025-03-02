#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <stdio.h>
#include "dct.h"

typedef struct Quantized
{
    unsigned a;
    int b;
    int c;
    int d;
    unsigned Pbar_b;
    unsigned Pbar_r;
} *Quantized;

Quantized quantize(DCT dct);

DCT inv_quantize(Quantized q);

#endif