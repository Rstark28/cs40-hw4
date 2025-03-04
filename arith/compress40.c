#include "compress40.h"
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb2cav.h"
#include "dct.h"
#include "stdlib.h"
#include "mem.h"
#include "quantize.h"
#include "packword.h"
#include "bitpack.h"

void compress40(FILE *input)
{
        assert(input != NULL);
        Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);

        printf("COMP40 Compressed image format 2\n%u %u\n", image->width & ~1, image->height & ~1);

        RGB_block rgb_block;
        NEW(rgb_block);
        CAV_block cav_block = CAV_block_new();
        DCT dct = DCT_new();
        Quantized quantized = Quantized_new();

        for (size_t col = 0; col < image->width / 2; col++)
        {
                for (size_t row = 0; row < image->height / 2; row++)
                {
                        for (size_t i = 0; i < 2; i++)
                        {
                                for (size_t j = 0; j < 2; j++)
                                {
                                        size_t index = i + j * 2;
                                        Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                        rgb_block->rgb[index] = rgb;
                                }
                        }

                        RGBtoCAV_block(cav_block, rgb_block, image->denominator);
                        computeDCT(dct, cav_block);
                        quantize(quantized, dct);
                        uint32_t packed = packWord(quantized);

                        for (int i = 3; i >= 0; i--)
                        {
                                putchar(Bitpack_getu(packed, 8, i * 8));
                        }
                }
        }

        Quantized_free(&quantized);
        DCT_free(&dct);
        CAV_block_free(&cav_block);
        FREE(rgb_block);
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

        RGB_block rgb_block = RGB_block_new();
        CAV_block cav_block = CAV_block_new();
        DCT dct = DCT_new();
        Quantized quantized = Quantized_new();

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

                        unpackWord(quantized, packed);
                        dequantize(dct, quantized);
                        invertDCT(cav_block, dct);

                        for (size_t i = 0; i < 2; i++)
                        {
                                for (size_t j = 0; j < 2; j++)
                                {
                                        int index = i + j * 2;
                                        Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                        CAVtoRGB_block(rgb_block, cav_block, image->denominator);
                                        rgb->red = rgb_block->rgb[index]->red;
                                        rgb->green = rgb_block->rgb[index]->green;
                                        rgb->blue = rgb_block->rgb[index]->blue;
                                }
                        }
                }
        }

        Pnm_ppmwrite(stdout, image);

        Quantized_free(&quantized);
        DCT_free(&dct);
        CAV_block_free(&cav_block);
        RGB_block_free(&rgb_block);
        Pnm_ppmfree(&image);
}