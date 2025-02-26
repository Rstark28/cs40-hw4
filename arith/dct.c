#include "dct.h"
#include "arith40.h"
#include "mem.h"

DCT ComputeDCT(CVBlock block)
{
    ComponentVideo *data = block->data;

    DCT dct;
    NEW(dct);

    float pbar_b = (data[3]->pb + data[2]->pb + data[1]->pb + data[0]->pb) / 4.0;
    float pbar_r = (data[3]->pr + data[2]->pr + data[1]->pr + data[0]->pr) / 4.0;

    dct->Pbar_b = Arith40_index_of_chroma(pbar_b);
    dct->Pbar_r = Arith40_index_of_chroma(pbar_r);

    dct->a = (data[3]->y + data[2]->y + data[1]->y + data[0]->y) / 4.0;
    dct->b = (data[3]->y + data[2]->y - data[1]->y - data[0]->y) / 4.0;
    dct->c = (data[3]->y - data[2]->y + data[1]->y - data[0]->y) / 4.0;
    dct->d = (data[3]->y - data[2]->y - data[1]->y + data[0]->y) / 4.0;

    return dct;
}

CVBlock InvertDCT(DCT dct)
{
    CVBlock block;
    NEW(block);

    for (int i = 0; i < 4; i++)
    {
        NEW(block->data[i]);
        block->data[i]->pb = Arith40_chroma_of_index(dct->Pbar_b);
        block->data[i]->pr = Arith40_chroma_of_index(dct->Pbar_r);
    }

    block->data[0]->y = dct->a - dct->b - dct->c + dct->d;
    block->data[1]->y = dct->a - dct->b + dct->c - dct->d;
    block->data[2]->y = dct->a + dct->b - dct->c - dct->d;
    block->data[3]->y = dct->a + dct->b + dct->c + dct->d;

    return block;
}