/**************************************************************
 *
 *                     dct.c
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the implementations for the Discrete Cosine Transform
 *     (DCT) module. The module provides functions to compute the DCT of a
 *     2x2 block of CAV data.
 *
 ************************/

#include "assert.h"
#include "dct.h"
#include "mem.h"
#include "pnm.h"
#include "rgb2cav.h"

/********** computeDCT ********
 *
 * Computes the Discrete Cosine Transform (DCT) of a given CAV_block.
 * The DCT coefficients are stored in the DCT structure.
 *
 * Parameters:
 *      dct:    A DCT structure to store the computed DCT values.
 *      block:  A CAV_block structure containing the 2x2 block of
 *              CAV data.
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
                CAV_block block)
{
        assert(dct != NULL);
        assert(block != NULL);
        assert(block->cav != NULL);

        CAV *cav = block->cav;

        /* Compute Pbar_b and Pbar_r */
        dct->Pbar_b = (cav[3]->P_b + cav[2]->P_b + cav[1]->P_b + cav[0]->P_b) / 4.0;
        dct->Pbar_r = (cav[3]->P_r + cav[2]->P_r + cav[1]->P_r + cav[0]->P_r) / 4.0;

        /* Compute the DCT coefficients */
        dct->a = (cav[3]->Y + cav[2]->Y + cav[1]->Y + cav[0]->Y) / 4.0;
        dct->b = (cav[3]->Y + cav[2]->Y - cav[1]->Y - cav[0]->Y) / 4.0;
        dct->c = (cav[3]->Y - cav[2]->Y + cav[1]->Y - cav[0]->Y) / 4.0;
        dct->d = (cav[3]->Y - cav[2]->Y - cav[1]->Y + cav[0]->Y) / 4.0;
}

/********** invertDCT ********
 *
 * Computes the inverse Discrete Cosine Transform (DCT) of a given DCT
 * structure. The resulting CAV data is stored in the
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
               DCT dct)
{
        assert(block != NULL);
        assert(dct != NULL);

        /* Compute Pbar_b and Pbar_r.
         * Use Pbar_b, Pbar_r for each P_b, P_r.*/
        for (int i = 0; i < 4; i++)
        {
                block->cav[i]->P_b = dct->Pbar_b;
                block->cav[i]->P_r = dct->Pbar_r;
        }

        /* Compute the Y values */
        block->cav[0]->Y = dct->a - dct->b - dct->c + dct->d;
        block->cav[1]->Y = dct->a - dct->b + dct->c - dct->d;
        block->cav[2]->Y = dct->a + dct->b - dct->c - dct->d;
        block->cav[3]->Y = dct->a + dct->b + dct->c + dct->d;
}

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
DCT DCT_new()
{
        DCT dct;
        NEW(dct);
        return dct;
}

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
void DCT_free(DCT *dct)
{
        assert(dct != NULL);
        assert(*dct != NULL);

        FREE(*dct);
        dct = NULL;
}