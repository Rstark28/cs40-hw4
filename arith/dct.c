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
 *     2x2 block of component video data.
 *
 ************************/

#include "assert.h"
#include "dct.h"
#include "mem.h"
#include "pnm.h"
#include "rgb2cv.h"

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
DCT ComputeDCT(CVBlock block)
{
        assert(block != NULL);
        assert(block->cv != NULL);

        ComponentVideo *cv = block->cv;

        DCT dct;
        NEW(dct);

        /* Compute Pbar_b and Pbar_r */
        dct->Pbar_b = (cv[3]->P_b + cv[2]->P_b + cv[1]->P_b + cv[0]->P_b) / 4.0;
        dct->Pbar_r = (cv[3]->P_r + cv[2]->P_r + cv[1]->P_r + cv[0]->P_r) / 4.0;

        /* Compute the DCT coefficients */
        dct->a = (cv[3]->Y + cv[2]->Y + cv[1]->Y + cv[0]->Y) / 4.0;
        dct->b = (cv[3]->Y + cv[2]->Y - cv[1]->Y - cv[0]->Y) / 4.0;
        dct->c = (cv[3]->Y - cv[2]->Y + cv[1]->Y - cv[0]->Y) / 4.0;
        dct->d = (cv[3]->Y - cv[2]->Y - cv[1]->Y + cv[0]->Y) / 4.0;

        return dct;
}

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
CVBlock InvertDCT(DCT dct)
{
        assert(dct != NULL);

        CVBlock block;
        NEW(block);

        /* Compute Pbar_b and Pbar_r.
         * Use Pbar_b, Pbar_r for each P_b, P_r.*/
        for (int i = 0; i < 4; i++)
        {
                NEW(block->cv[i]);
                block->cv[i]->P_b = dct->Pbar_b;
                block->cv[i]->P_r = dct->Pbar_r;
        }

        /* Compute the Y values */
        block->cv[0]->Y = dct->a - dct->b - dct->c + dct->d;
        block->cv[1]->Y = dct->a - dct->b + dct->c - dct->d;
        block->cv[2]->Y = dct->a + dct->b - dct->c - dct->d;
        block->cv[3]->Y = dct->a + dct->b + dct->c + dct->d;

        return block;
}

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
void FREECVBlock(CVBlock *block)
{
        assert(block != NULL);
        assert(*block != NULL);

        for (int i = 0; i < 4; i++)
                FREE((*block)->cv[i]);

        FREE(*block);
        block = NULL;
}
