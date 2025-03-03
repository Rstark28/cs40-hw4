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
 *      DCT dct:    A pointer to the DCT structure containing the DCT values.
 *
 * Return:
 *      Quantized:  A pointer to a Quantized structure containing the quantized
 *                  values.
 *
 * Expects:
 *      The pointer dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned Quantized structure.
 ************************/
Quantized quantize(DCT dct);

/********** inv_quantize ********
 *
 * Dequantizes the values of a Quantized structure.
 *
 * Parameters:
 *      Quantized q:    A pointer to the Quantized structure containing the
 *                      quantized values.
 *
 * Return:
 *      DCT:            A pointer to a DCT structure containing the DCT values.
 *
 * Expects:
 *      The pointer q must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned DCT structure.
 ************************/
DCT inv_quantize(Quantized q);

#endif