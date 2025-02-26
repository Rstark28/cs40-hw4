#include "compress40.h"

#include "assert.h"

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb2cv.h"
#include "dct.h"
#include "stdlib.h"
#include "mem.h"

void compress40(FILE *input)
{
    assert(input != NULL);
    Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);

    for (size_t col = 0; col < image->width / 2; col++)
    {
        for (size_t row = 0; row < image->height / 2; row++)
        {
            CVBlock block;
            NEW(block);
            for (size_t i = 0; i < 2; i++)
            {
                for (size_t j = 0; j < 2; j++)
                {
                    Pnm_rgb rgb = image->methods->at(image->pixels, col + i, row + j);
                    printf("%u, %u, %u\n", rgb->red, rgb->green, rgb->blue);
                    ComponentVideo cv = RGBtoCV(rgb, image->denominator);
                    Pnm_rgb rgb2 = CVtoRGB(cv, image->denominator);
                    printf("%u, %u, %u\n", rgb2->red, rgb2->green, rgb2->blue);
                    block->data[i + j * 2] = cv;
                }
            }
            DCT dct = ComputeDCT(block);

            printf("%f %f %u %u", dct->a, dct->b, dct->Pbar_b, dct->Pbar_r);

            CVBlock block2 = InvertDCT(dct);

            for (size_t i = 0; i < 4; i++)
            {
                printf("IN CV: %f %f %f\n", block->data[i]->y, block->data[i]->pb, block->data[i]->pr);
                printf("OUT CV: %f %f %f\n", block2->data[i]->y, block2->data[i]->pb, block2->data[i]->pr);
            }
            exit(0);
        }
    }
}

void decompress40(FILE *input)
{
    (void)input;
}
