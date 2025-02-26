#include "assert.h"
#include "arith40.h"
#include "dct.h"
#include "mem.h"
#include "pnm.h"
#include "rgb2cv.h"

/********** ComputeDCT ********
 *
 * Computes the Discrete Cosine Transform (DCT) of a given CVBlock.
 *
 * Parameters:
 *      CVBlock block:  A pointer to the CVBlock structure containing the data.
 *
 * Return:
 *      DCT:            A pointer to the computed DCT structure.
 *
 * Expects:
 *      The pointer block must not be NULL.
 *      The 2x2 block must be filled with valid ComponentVideo data.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
DCT ComputeDCT(CVBlock block)
{
        assert(block != NULL);
        assert(block->data != NULL);

        ComponentVideo *data = block->data;

        DCT dct;
        NEW(dct);

        /* Compute Pbar_b and Pbar_r */
        float pbar_b = (data[3]->P_b + data[2]->P_b + data[1]->P_b + data[0]->P_b) / 4.0;
        float pbar_r = (data[3]->P_r + data[2]->P_r + data[1]->P_r + data[0]->P_r) / 4.0;

        /* Get the index of the chroma values */
        dct->Pbar_b = Arith40_index_of_chroma(pbar_b);
        dct->Pbar_r = Arith40_index_of_chroma(pbar_r);

        /* Compute the DCT coefficients */
        dct->a = (data[3]->Y + data[2]->Y + data[1]->Y + data[0]->Y) / 4.0;
        dct->b = (data[3]->Y + data[2]->Y - data[1]->Y - data[0]->Y) / 4.0;
        dct->c = (data[3]->Y - data[2]->Y + data[1]->Y - data[0]->Y) / 4.0;
        dct->d = (data[3]->Y - data[2]->Y - data[1]->Y + data[0]->Y) / 4.0;

        return dct;
}

/********** InvertDCT ********
 *
 * Inverts the Discrete Cosine Transform (DCT) to obtain the original CVBlock.
 *
 * Parameters:
 *      DCT dct:        A pointer to the DCT structure containing the data.
 *
 * Return:
 *      CVBlock:        A pointer to the reconstructed CVBlock structure.
 *
 * Expects:
 *      The pointer dct must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
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
                NEW(block->data[i]);
                block->data[i]->P_b = Arith40_chroma_of_index(dct->Pbar_b);
                block->data[i]->P_r = Arith40_chroma_of_index(dct->Pbar_r);
        }

        /* Compute the Y values */
        block->data[0]->Y = dct->a - dct->b - dct->c + dct->d;
        block->data[1]->Y = dct->a - dct->b + dct->c - dct->d;
        block->data[2]->Y = dct->a + dct->b - dct->c - dct->d;
        block->data[3]->Y = dct->a + dct->b + dct->c + dct->d;

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
 *      The pointer block must not be NULL.
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
                FREE((*block)->data[i]);

        FREE(*block);
        block = NULL;
}