#include "compress40.h"
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb2cv.h"
#include "dct.h"
#include "stdlib.h"
#include "mem.h"
#include "quantize.h"
#include "pack_word.h"
#include "bitpack.h"

void compress40(FILE *input)
{
        assert(input != NULL);
        Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);

        printf("COMP40 Compressed image format 2\n%u %u\n", image->width & ~1, image->height & ~1);

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

                        Quantized quantized_original = quantize(dct);

                        uint32_t packed = pack_word(quantized_original);

                        for (int i = 3; i >= 0; i--)
                        {
                                uint32_t val = Bitpack_getu(packed, 8, i * 8);
                                putchar(val);
                        }

                        FREECVBlock(&block);
                        FREE(dct);
                }
        }

        Pnm_ppmfree(&image);
}

void decompress40(FILE *input)
{
        assert(input != NULL);

        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        Pnm_ppm image;
        NEW(image);

        image->width = width;
        image->height = height;

        image->denominator = 255;

        image->methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels = image->methods->new(image->width, image->height, 12);
        image->pixels = pixels;

        for (size_t col = 0; col < width / 2; col++)
        {
                for (size_t row = 0; row < height / 2; row++)
                {
                        uint32_t packed = 0;

                        for (int i = 3; i >= 0; i--)
                        {
                                int byte = getc(input);
                                packed = Bitpack_newu(packed, 8, i * 8, byte);
                        }

                        Quantized quantized = unpack_word(packed);

                        DCT dct = inv_quantize(quantized);

                        CVBlock block = InvertDCT(dct);

                        for (size_t i = 0; i < 2; i++)
                        {
                                for (size_t j = 0; j < 2; j++)
                                {
                                        Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                        Pnm_rgb rgb_new = CVtoRGB(block->data[i + j * 2], image->denominator);
                                        rgb->red = rgb_new->red;
                                        rgb->green = rgb_new->green;
                                        rgb->blue = rgb_new->blue;
                                }
                        }
                        FREECVBlock(&block);
                        FREE(dct);
                }
        }

        Pnm_ppmwrite(stdout, image);
}
