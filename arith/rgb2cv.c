#include "assert.h"
#include "mem.h"
#include "pnm.h"
#include "rgb2cv.h"

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
ComponentVideo RGBtoCV(Pnm_rgb rgb, int denominator)
{
        assert(rgb != NULL);
        assert(denominator > 0);

        ComponentVideo cv;
        NEW(cv);

        /* Convert RGB values to floats */
        float red = (float)rgb->red / denominator;
        float green = (float)rgb->green / denominator;
        float blue = (float)rgb->blue / denominator;

        /* Convert RGB to Component Video */
        cv->Y = 0.299 * red + 0.587 * green + 0.114 * blue;
        cv->P_b = -0.168736 * red - 0.331264 * green + 0.5 * blue;
        cv->P_r = 0.5 * red - 0.418688 * green - 0.081312 * blue;

        return cv;
}

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
Pnm_rgb CVtoRGB(ComponentVideo cv, int denominator)
{
        assert(cv != NULL);
        assert(denominator > 0);

        Pnm_rgb rgb;
        NEW(rgb);

        /* Convert Component Video to RGB */
        float red = 1.0 * cv->Y + 0.0 * cv->P_b + 1.402 * cv->P_r;
        float green = 1.0 * cv->Y - 0.344136 * cv->P_b - 0.714136 * cv->P_r;
        float blue = 1.0 * cv->Y + 1.772 * cv->P_b + 0.0 * cv->P_r;

        /* Scale RGB values to the specified denominator */
        rgb->red = red * denominator;
        rgb->green = green * denominator;
        rgb->blue = blue * denominator;

        return rgb;
}