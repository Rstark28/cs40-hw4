/**************************************************************
 *s
 *                     dct.h
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the interface for the Discrete Cosine Transform (DCT)
 *     module. The module provides functions to compute the DCT of a 2x2 block
 *     of component video data.
 *
 ************************/

#ifndef DCT_H
#define DCT_H

#include <stdio.h>
#include "rgb2cav.h"

/********** DCT ********
 *
 * A structure representing the Discrete Cosine Transform (DCT) of a CAV_block.
 *
 * Elements:
 *      float a:         The first DCT coefficient.
 *      float b:         The second DCT coefficient.
 *      float c:         The third DCT coefficient.
 *      float d:         The fourth DCT coefficient.
 *      unsigned Pbar_b: The chroma value for Pbar_b.
 *      unsigned Pbar_r: The chroma value for Pbar_r.
 ***********************/
typedef struct DCT
{
        float a;
        float b;
        float c;
        float d;
        float Pbar_b;
        float Pbar_r;
} *DCT;

/********** computeDCT ********
 *
 * Computes the Discrete Cosine Transform (DCT) of a given CAV_block.
 * The DCT coefficients are stored in the DCT structure.
 *
 * Parameters:
 *      dct:    A DCT structure to store the computed DCT values.
 *      block:  A CAV_block structure containing the 2x2 block of
 *              component video data.
 *
 * Expects:
 *      dct must not be NULL.
 *      block must not be NULL.
 *      The 2x2 block must be filled with valid ComponentVideo structs.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void computeDCT(DCT dct,
                CAV_block block);

/********** invertDCT ********
 *
 * Computes the inverse Discrete Cosine Transform (DCT) of a given DCT
 * structure. The resulting component video data is stored in the
 * CAV_block structure.
 *
 * Parameters:
 *      CAV_block block:  A CAV_block structure to store the computed component
 *                      video data.
 *      DCT dct:        A DCT structure containing the DCT coefficients.
 *
 * Expects:
 *      block must not be NULL.
 *      dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned CAV_block structure.
 ************************/
void invertDCT(CAV_block block,
               DCT dct);

/********** DCT_new ********
 *
 * Allocates memory for a new DCT structure.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      DCT: A newly allocated DCT structure.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned DCT structure with
 *      DCT_free.
 ************************/
DCT DCT_new();

/********** DCT_free ********
 *
 * Frees the memory allocated for a DCT structure.
 *
 * Parameters:
 *      DCT *dct:  A pointer to a DCT structure to be freed.
 *
 * Expects:
 *      dct must not be NULL.
 *      *dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The pointer *dct will be set to NULL after freeing.
 ************************/
void DCT_free(DCT *dct);

#endif