/**************************************************************
 *
 *                     rgb2cav.c
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the implementation for the RGB to Component Video
 *     conversion module. It provides functions to convert between RGB and
 *     component video representations.
 *
 ************************/

#include "assert.h"
#include "mem.h"
#include "pnm.h"
#include "rgb2cav.h"

/********** RGBtoCAV_block ********
 *
 * Converts an RGB_block to a CAV_block.
 *
 * Parameters:
 *      CAV_block cav_block:  A CAV_block structure to store the Y, Pb, and
 *                            Pr values.
 *      RGB_block rgb_block:  An RGB_block structure containing the RGB values.
 *      int denominator:      The denominator used for scaling the RGB values.
 *
 * Expects:
 *      cav_block must not be NULL.
 *      rgb_block must not be NULL.
 *      denominator must be greater than 0.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void RGBtoCAV_block(CAV_block cav_block,
                    RGB_block rgb_block,
                    int denominator)
{
        assert(cav_block != NULL);
        assert(rgb_block != NULL);
        assert(denominator > 0);

        for (int i = 0; i < 4; i++)
        {
                RGBtoCAV(cav_block->cav[i], rgb_block->rgb[i], denominator);
        }
}

/********** CAVtoRGB_block ********
 * Converts a CAV_block to an RGB_block.
 *
 * Parameters:
 *     RGB_block rgb_block:  An RGB_block structure to store the RGB values.
 *     CAV_block cav_block:  A CAV_block structure containing the Y, Pb, and
 *                           Pr values.
 *     int denominator:      The denominator used for scaling the RGB values.
 *
 * Expects:
 *    rgb_block must not be NULL.
 *    cav_block must not be NULL.
 *    denominator must be greater than 0.
 *
 * Notes:
 *    Will CRE if any expectation is violated.
 * ************************/
void CAVtoRGB_block(RGB_block rgb_block,
                    CAV_block cav_block,
                    int denominator)
{
        assert(rgb_block != NULL);
        assert(cav_block != NULL);
        assert(denominator > 0);

        for (int i = 0; i < 4; i++)
        {
                CAVtoRGB(rgb_block->rgb[i], cav_block->cav[i], denominator);
        }
}

/********** RGBtoCAV ********
 *
 * Converts RGB to CAV.
 *
 * Parameters:
 *      CAV cav:         A CAV structure to store the Y, Pb, and Pr values.
 *      Pnm_rgb rgb:     A Pnm_rgb structure containing the RGB values.
 *      int denominator: The denominator used for scaling the RGB values.
 *
 * Expects:
 *      cav must not be NULL.
 *      rgb must not be NULL.
 *      denominator must be greater than 0.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void RGBtoCAV(CAV cav,
              Pnm_rgb rgb,
              int denominator)
{
        assert(cav != NULL);
        assert(rgb != NULL);
        assert(denominator > 0);

        /* Convert RGB values to floats */
        float red = (float)rgb->red / denominator;
        float green = (float)rgb->green / denominator;
        float blue = (float)rgb->blue / denominator;

        /* Convert RGB to Component Video */
        cav->Y = 0.299 * red + 0.587 * green + 0.114 * blue;
        cav->P_b = -0.168736 * red - 0.331264 * green + 0.5 * blue;
        cav->P_r = 0.5 * red - 0.418688 * green - 0.081312 * blue;

        /* Clamp Y to the range [0, 1] */
        cav->Y = cav->Y < 0 ? 0 : (cav->Y > 1 ? 1 : cav->Y);

        /* Clamp Pb and Pr to the range [-0.5, 0.5] */
        cav->P_b = cav->P_b < -0.5 ? -0.5 : (cav->P_b > 0.5 ? 0.5 : cav->P_b);
        cav->P_r = cav->P_r < -0.5 ? -0.5 : (cav->P_r > 0.5 ? 0.5 : cav->P_r);
}

/********** CAVtoRGB ********
 *
 * Converts CAV to RGB.
 *
 * Parameters:
 *      Pnm_rgb rgb:     A Pnm_rgb structure to store the RGB values.
 *      CAV cav:         A CAV structure containing the Y, Pb,
 *                       and Pr values.
 *      int denominator: The denominator used for scaling the RGB values.
 *
 * Expects:
 *      rgb must not be NULL.
 *      cav must not be NULL.
 *      denominator must be greater than 0.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void CAVtoRGB(Pnm_rgb rgb,
              CAV cav,
              int denominator)
{
        assert(rgb != NULL);
        assert(cav != NULL);
        assert(denominator > 0);

        /* Convert Component Video to RGB */
        float red = cav->Y + 1.402 * cav->P_r;
        float green = cav->Y - 0.344136 * cav->P_b - 0.714136 * cav->P_r;
        float blue = cav->Y + 1.772 * cav->P_b;

        /* Clamp RGB values to the range [0, 1] */
        red = red < 0 ? 0 : (red > 1 ? 1 : red);
        green = green < 0 ? 0 : (green > 1 ? 1 : green);
        blue = blue < 0 ? 0 : (blue > 1 ? 1 : blue);

        /* Scale RGB values to the specified denominator */
        rgb->red = red * denominator;
        rgb->green = green * denominator;
        rgb->blue = blue * denominator;
}

/********** RGB_block_new ********
 *
 * Allocates memory for a new RGB_block, including the Pnm_rgb structs.
 *
 * Parameters:
 *     None.
 *
 * Returns:
 *    RGB_block: A newly allocated RGB_block structure.
 *
 * Notes:
 *  Will CRE if any expectation is violated.
 *  Client is responsible for freeing the returned RGB_block structure with
 *  RGB_block_free.
 ************************/
RGB_block RGB_block_new()
{
        RGB_block block;
        NEW(block);

        for (int i = 0; i < 4; i++)
        {
                NEW(block->rgb[i]);
        }

        return block;
}

/********** RGB_block_free ********
 *
 * Frees the memory allocated for a RGB_block.
 *
 * Parameters:
 *      RGB_block *block:  A pointer to a RGB_block structure to be freed.
 *
 * Expects:
 *      block must not be NULL.
 *      *block must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The pointer *block will be set to NULL after freeing.
 ************************/
void RGB_block_free(RGB_block *block)
{
        assert(block != NULL);
        assert(*block != NULL);

        for (int i = 0; i < 4; i++)
        {
                FREE((*block)->rgb[i]);
        }

        FREE(*block);
        block = NULL;
}

/********** CAV_block_new ********
 *
 * Allocates memory for a new CAV_block, including the ComponentVideo structs.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      CAV_block: A newly allocated CAV_block structure.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned CAV_block structure with
 *      CAV_block_free.
 ************************/
CAV_block CAV_block_new()
{
        CAV_block block;
        NEW(block);

        for (int i = 0; i < 4; i++)
        {
                NEW(block->cav[i]);
        }
        return block;
}

/********** CAV_block_free ********
 *
 * Frees the memory allocated for a CAV_block.
 *
 * Parameters:
 *      CAV_block *block:  A pointer to a CAV_block structure to be freed.
 *
 * Expects:
 *      block must not be NULL.
 *      *block must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The pointer *block will be set to NULL after freeing.
 ************************/
void CAV_block_free(CAV_block *block)
{
        assert(block != NULL);
        assert(*block != NULL);

        for (int i = 0; i < 4; i++)
        {
                FREE((*block)->cav[i]);
        }
        FREE(*block);
        block = NULL;
}