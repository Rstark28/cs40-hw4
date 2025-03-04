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

/********** compress40 ********
 *
 * Compresses a PPM image to a 40image format.
 *
 * Parameters:
 *      FILE *input:    A pointer to the input file.
 *
 * Expects:
 *      input must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void compress40(FILE *input)
{
        assert(input != NULL);
        Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);

        /* Print compressed image header */
        printf("COMP40 Compressed image format 2\n%u %u\n", image->width & ~1, image->height & ~1);

        /* Allocate memory for processing blocks */
        /* Only allocate RGB wrapper to reuse existing structs from image */
        RGB_block rgb_block;
        NEW(rgb_block);
        CAV_block cav_block = CAV_block_new();
        DCT dct = DCT_new();
        Quantized quantized = Quantized_new();

        /* Iterate through 2×2 blocks */
        for (size_t col = 0; col < image->width / 2; col++)
        {
                for (size_t row = 0; row < image->height / 2; row++)
                {
                        for (size_t i = 0; i < 2; i++)
                        {
                                for (size_t j = 0; j < 2; j++)
                                {
                                        Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                        rgb_block->rgb[i + j * 2] = rgb;
                                }
                        }

                        /* Process the block */
                        RGBtoCAV_block(cav_block, rgb_block, image->denominator);
                        computeDCT(dct, cav_block);
                        quantize(quantized, dct);
                        uint32_t packed = packWord(quantized);

                        /* Output packed word as 4 bytes */
                        for (int shift = 24; shift >= 0; shift -= 8)
                        {
                                putchar((packed >> shift) & 0xFF);
                        }
                }
        }

        /* Free allocated memory */
        Quantized_free(&quantized);
        DCT_free(&dct);
        CAV_block_free(&cav_block);
        FREE(rgb_block);
        Pnm_ppmfree(&image);
}

/********** decompress40 ********
 *
 * Decompresses a 40image format to a PPM image.
 *
 * Parameters:
 *      FILE *input:    A pointer to the input file.
 *
 * Expects:
 *      input must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void decompress40(FILE *input)
{
        assert(input != NULL);

        /* Read header */
        unsigned width, height;
        assert(fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height) == 2);
        assert(getc(input) == '\n');

        /* Allocate and initialize image */
        Pnm_ppm image;
        NEW(image);
        image->width = width;
        image->height = height;
        image->denominator = 255;
        image->methods = uarray2_methods_plain;
        image->pixels = image->methods->new(width, height, 12);

        /* Allocate memory for processing blocks */
        RGB_block rgb_block = RGB_block_new();
        CAV_block cav_block = CAV_block_new();
        DCT dct = DCT_new();
        Quantized quantized = Quantized_new();

        /* Read and decompress image */
        for (size_t col = 0; col < width / 2; col++)
        {
                for (size_t row = 0; row < height / 2; row++)
                {
                        /* Read packed word */
                        uint32_t packed = 0;
                        for (int shift = 24; shift >= 0; shift -= 8)
                        {
                                packed = Bitpack_newu(packed, 8, shift, getc(input));
                        }
                        unpackWord(quantized, packed);
                        dequantize(dct, quantized);
                        invertDCT(cav_block, dct);
                        CAVtoRGB_block(rgb_block, cav_block, image->denominator);

                        /* Reconstruct 2×2 RGB block */
                        for (size_t k = 0; k < 4; k++)
                        {
                                size_t i = k % 2, j = k / 2;
                                Pnm_rgb rgb = image->methods->at(image->pixels, col * 2 + i, row * 2 + j);
                                *rgb = *(rgb_block->rgb[k]);
                        }
                }
        }

        /* Output decompressed image */
        Pnm_ppmwrite(stdout, image);

        /* Free allocated memory */
        Quantized_free(&quantized);
        DCT_free(&dct);
        CAV_block_free(&cav_block);
        RGB_block_free(&rgb_block);
        Pnm_ppmfree(&image);
}
