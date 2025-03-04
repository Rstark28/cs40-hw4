/**************************************************************
 *
 *                     rgb2cav.h
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the interface for the RGB to Component Video
 *     conversion module. It provides functions to convert between RGB and
 *     component video representations.
 *
 ************************/

#ifndef RGB2CAV_H
#define RGB2CAV_H

#include <stdio.h>
#include "pnm.h"

/********** CAV ********
 *
 * A structure to hold the Y, Pb, and Pr values of a pixel.
 *
 * Members:
 *      float Y:    The Y value of the pixel.
 *      float P_b:  The Pb value of the pixel.
 *      float P_r:  The Pr value of the pixel.
 ************************/
typedef struct CAV
{
        float Y;
        float P_b;
        float P_r;
} *CAV;

/********** RGB_block ********
 *
 * A 2x2 block of Pnm_rgb structs. Expects the following layout:
 * Top left:     rgb[0]   Top right:     rgb[1]
 * Bottom left:  rgb[2]   Bottom right:  rgb[3]
 *
 * Elements:
 *      Pnm_rgb rgb[4]:   The array of Pnm_rgb structs.
 ************************/
typedef struct RGB_block
{
        Pnm_rgb rgb[4];
} *RGB_block;

/********** CAV_block ********
 *
 * A 2x2 block of ComponentVideo structs. Expects the following layout:
 * Top left:     cav[0]   Top right:     cav[1]
 * Bottom left:  cav[2]   Bottom right:  cav[3]
 *
 * Elements:
 *      ComponentVideo cav[4]:   The array of ComponentVideo structs.
 *                              Each struct contains the Y, Pb, and Pr values.
 ***********************/
typedef struct CAV_block
{
        CAV cav[4];
} *CAV_block;

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
                    int denominator);

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
                    int denominator);

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
              int denominator);

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
              int denominator);

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
RGB_block RGB_block_new();

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
void RGB_block_free(RGB_block *block);

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
CAV_block CAV_block_new();

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
 *      Frees the memory allocated for the ComponentVideo structures
 *      within the CAV_block.
 *      The pointer *block will be set to NULL after freeing.
 ************************/
void CAV_block_free(CAV_block *block);

#endif