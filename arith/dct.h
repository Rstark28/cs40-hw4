#ifndef DCT_H
#define DCT_H

#include <stdio.h>
#include "rgb2cv.h"
#include "pnm.h"

typedef struct CVBlock
{
    ComponentVideo data[4];
} *CVBlock;

typedef struct DCT
{
    float a;
    float b;
    float c;
    float d;
    unsigned Pbar_b;
    unsigned Pbar_r;
} *DCT;

DCT ComputeDCT(CVBlock block);

CVBlock InvertDCT(DCT dct);

#endif