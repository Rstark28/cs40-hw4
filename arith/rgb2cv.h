#ifndef RGB2CV_H
#define RGB2CV_H

#include <stdio.h>
#include "pnm.h"

/********** ComponentVideo ********
 *
 * A structure representing a Component Video (CV) value.
 *
 * Elements:
 *      float Y: The Y component (luminance).
 *      float P_b: The P_b component (blue-difference chroma).
 *      float P_r: The P_r component (red-difference chroma).
 ***********************/
typedef struct ComponentVideo
{
        float Y;
        float P_b;
        float P_r;
} *ComponentVideo;

/********** RGBtoCV ********
 *
 * Converts a Pnm_rgb structure to a ComponentVideo structure.
 *
 * Parameters:
 *      Pnm_rgb rgb:       A pointer to the Pnm_rgb structure containing the
 *                         RGB values.
 *      int denominator:   The denominator used for scaling the RGB values.
 *
 * Return:
 *      ComponentVideo:    A pointer to the converted ComponentVideo structure.
 *
 * Expects:
 *      The pointer rgb must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
ComponentVideo RGBtoCV(Pnm_rgb rgb, int denominator);

/********** CVtoRGB ********
 *
 * Converts a ComponentVideo structure to a Pnm_rgb structure.
 *
 * Parameters:
 *      ComponentVideo cv: A pointer to the ComponentVideo structure containing
 *                         the Y, Pb, and Pr values.
 *      int denominator:   The denominator used for scaling the RGB values.
 *
 * Return:
 *      Pnm_rgb:          A pointer to the converted Pnm_rgb structure.
 *
 * Expects:
 *      The pointer cv must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
Pnm_rgb CVtoRGB(ComponentVideo cv, int denominator);

#endif