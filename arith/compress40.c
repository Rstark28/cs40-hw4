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
                                        Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                        ComponentVideo cv = RGBtoCV(rgb, image->denominator);
                                        block->data[i + j * 2] = cv;
                                }
                        }
                        DCT dct = ComputeDCT(block);

                        CVBlock block2 = InvertDCT(dct);

                        for (size_t i = 0; i < 4; i++)
                        {
                                // printf("IN CV: %f %f %f\n", block->data[i]->Y, block->data[i]->P_b, block->data[i]->P_r);
                                // printf("OUT CV: %f %f %f\n", block2->data[i]->Y, block2->data[i]->P_b, block2->data[i]->P_r);
                        }
                        FREECVBlock(&block);
                        FREECVBlock(&block2);
                        FREE(dct);
                }
        }
        Pnm_ppmfree(&image);
}

void decompress40(FILE *input)
{
        (void)input;
}
