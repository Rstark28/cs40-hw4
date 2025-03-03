/**************************************************************
 *
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
#include "rgb2cv.h"

/********** CVBlock ********
 *
 * A 2x2 block of ComponentVideo structs.
 *
 * Elements:
 *      ComponentVideo cv[4]:   The array of ComponentVideo structs.
 *                              Each struct contains the Y, Pb, and Pr values.
 ***********************/
typedef struct CVBlock
{
        ComponentVideo cv[4];
} *CVBlock;

/********** DCT ********
 *
 * A structure representing the Discrete Cosine Transform (DCT) of a CVBlock.
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

/********** ComputeDCT ********
 *
 * Computes the Discrete Cosine Transform (DCT) of a given CVBlock.
 *
 * Parameters:
 *      CVBlock block:  A pointer to the CVBlock structure containing the
 *                      2x2 block of component video data.
 *
 * Return:
 *      DCT:            A pointer to the computed DCT structure.
 *
 * Expects:
 *      The pointer block must not be NULL.
 *      The 2x2 block must be filled with valid ComponentVideo cv.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned DCT structure.
 ************************/
DCT ComputeDCT(CVBlock block);

/********** InvertDCT ********
 *
 * Inverts the Discrete Cosine Transform (DCT) to obtain the original CVBlock.
 *
 * Parameters:
 *      DCT dct:        A pointer to the DCT structure containing the
 *                      a, b, c, d, Pbar_b, and Pbar_r values.
 *
 * Return:
 *      CVBlock:        A pointer to the reconstructed CVBlock structure.
 *
 * Expects:
 *      The pointer dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned CVBlock structure.
 ************************/
CVBlock InvertDCT(DCT dct);

/**********FREECVBlock********
 *
 * Frees the memory allocated for a CVBlock.
 *
 * Parameters:
 *      CVBlock *block:  A pointer to the CVBlock structure to be freed.
 *
 * Expects:
 *      The pointer and the block must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Frees the memory allocated for the ComponentVideo structures
 *      within the CVBlock.
 *      The pointer block will be set to NULL after freeing.
 ************************/
void FREECVBlock(CVBlock *block);

#endif