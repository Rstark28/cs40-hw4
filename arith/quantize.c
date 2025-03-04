/**************************************************************
 *
 *                     quantize.c
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the implementation for the quantize module. It
 *     provides functions to quantize and dequantize the values of a
 *     DCT structure.
 *
 ************************/

#include "quantize.h"
#include <assert.h>
#include "arith40.h"
#include "mem.h"
#include <math.h>

/********** quantize_bcd ********
 *
 * Quantizes b, c, or d value.
 *
 * Parameters:
 *      float in:   The b, c, or d value.
 *
 * Return:
 *      int:        The quantized b, c, or d value.
 *
 * Expects:
 *      The returned value will be between -15 and 15.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int quantize_bcd(float in)
{
        in = in < -0.3 ? -0.3 : in;
        in = in > 0.3 ? 0.3 : in;
        return (int)roundf(in * 50);
}

/********** quantize_a ********
 *
 * Quantizes a.
 *
 * Parameters:
 *      float in:    The a value.
 *
 * Return:
 *      unsigned:    The quantized a value.
 *
 * Expects:
 *      The returned value will be between 0 and 511.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
unsigned quantize_a(float in)
{
        in = in < 0 ? 0 : in;
        in = in > 1 ? 1 : in;
        return (unsigned)roundf(in * 511);
}

/********** inv_quantize_bcd ********
 *
 * Dequantizes b, c, or d value.
 *
 * Parameters:
 *      int in:    The quantized b, c, or d value.
 *
 * Return:
 *      float:     The dequantized b, c, or d value.
 *
 * Expects:
 *      The value in must be between -15 and 15.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
float inv_quantize_bcd(int in)
{
        assert(in >= -15 && in <= 15);
        return in / 50.0;
}

/********** inv_quantize_a ********
 *
 * Dequantizes a.
 *
 * Parameters:
 *      unsigned in:   The quantized a value.
 *
 * Return:
 *      float:         The dequantized a value.
 *
 * Expects:
 *      The value in must be less than 512.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
float inv_quantize_a(unsigned in)
{
        assert(in < 512);
        return in / 511.0;
}

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
              DCT dct)
{
        assert(q != NULL);
        assert(dct != NULL);

        q->a = quantize_a(dct->a);
        q->b = quantize_bcd(dct->b);
        q->c = quantize_bcd(dct->c);
        q->d = quantize_bcd(dct->d);
        q->Pbar_b = Arith40_index_of_chroma(dct->Pbar_b);
        q->Pbar_r = Arith40_index_of_chroma(dct->Pbar_r);
}

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
                Quantized q)
{
        assert(dct != NULL);
        assert(q != NULL);

        dct->a = inv_quantize_a(q->a);
        dct->b = inv_quantize_bcd(q->b);
        dct->c = inv_quantize_bcd(q->c);
        dct->d = inv_quantize_bcd(q->d);
        dct->Pbar_b = Arith40_chroma_of_index(q->Pbar_b);
        dct->Pbar_r = Arith40_chroma_of_index(q->Pbar_r);
}

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
Quantized Quantized_new()
{
        Quantized q;
        NEW(q);
        return q;
}

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
void Quantized_free(Quantized *q)
{
        assert(q != NULL);
        assert(*q != NULL);

        FREE(*q);
        q = NULL;
}