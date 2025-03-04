/**************************************************************
 *
 *                     quantize.h
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the interface for the quantize module. It
 *     provides functions to quantize and dequantize the values of a
 *     DCT structure.
 *
 ************************/

#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <stdio.h>
#include "dct.h"

/********** Quantized ********
 *
 * A structure to hold the quantized values of a DCT structure.
 *
 * Members:
 *      unsigned a:        The quantized a value.
 *      int b:             The quantized b value.
 *      int c:             The quantized c value.
 *      int d:             The quantized d value.
 *      unsigned Pbar_b:   The quantized Pbar_b value.
 *      unsigned Pbar_r:   The quantized Pbar_r value.
 ************************/
typedef struct Quantized
{
        unsigned a;
        int b;
        int c;
        int d;
        unsigned Pbar_b;
        unsigned Pbar_r;
} *Quantized;

/********** quantize ********
 *
 * Quantizes the values of a DCT structure.
 *
 * Parameters:
 *      q:      A Quantized structure to store the quantized values.
 *      dct:    A DCT structure containing the DCT values.
 *
 * Expects:
 *      q must not be NULL.
 *      dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void quantize(Quantized q,
              DCT dct);

/********** dequantize ********
 *
 * Dequantizes the values of a Quantized structure.
 *
 * Parameters:
 *      dct:    A DCT structure to store the dequantized values.
 *      q:      A Quantized structure containing the quantized values.
 *
 * Expects:
 *      dct must not be NULL.
 *      q must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
void dequantize(DCT dct,
                Quantized q);

/********** Quantized_new ********
 *
 * Allocates memory for a new Quantized structure.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Quantized: A newly allocated Quantized structure.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned Quantized structure with
 *      FREE.
 ************************/
Quantized Quantized_new();

/********** Quantized_free ********
 *
 * Frees the memory allocated for a Quantized structure.
 *
 * Parameters:
 *      Quantized *q:  A pointer to a Quantized structure to be freed.
 *
 * Expects:
 *      q must not be NULL.
 *      *q must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      The pointer *q will be set to NULL after freeing.
 ************************/
void Quantized_free(Quantized *q);

#endif