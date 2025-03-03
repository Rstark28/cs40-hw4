/**************************************************************
 *
 *                     rgb2cv.h
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

#ifndef RGB2CV_H
#define RGB2CV_H

#include <stdio.h>
#include "pnm.h"

/********** ComponentVideo ********
 *
 * A structure to hold the Y, Pb, and Pr values of a pixel.
 *
 * Members:
 *      float Y:    The Y value of the pixel.
 *      float P_b:  The Pb value of the pixel.
 *      float P_r:  The Pr value of the pixel.
 ************************/
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
 *      Client is responsible for freeing the returned ComponentVideo structure.
 ************************/
ComponentVideo RGBtoCV(Pnm_rgb rgb,
                       int denominator);

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
 *      Client is responsible for freeing the returned Pnm_rgb structure.
 ************************/
Pnm_rgb CVtoRGB(ComponentVideo cv,
                int denominator);

#endif