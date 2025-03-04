/**************************************************************
 *
 *                     packword.c
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the implementation for the pack word module.
 *     It provides functions to pack and unpack quantized values from a
 *     32-bit word.
 *
 ************************/

#include "packword.h"
#include "bitpack.h"
#include "mem.h"
#include "assert.h"

/********** packWord ********
 *
 * Packs a, b, c, d, Pbar_b, and Pbar_r values into a 32-bit word. The word is
 * stored in the following format:
 *     a: 9 bits, stored at bits 23-31
 *     b: 5 bits, stored at bits 18-22
 *     c: 5 bits, stored at bits 13-17
 *     d: 5 bits, stored at bits 8-12
 *     Pbar_b: 4 bits, stored at bits 4-7
 *     Pbar_r: 4 bits, stored at bits 0-3
 *
 * Parameters:
 *      Quantized q:    A Quantized structure containing the
 *                      quantized values.
 *
 * Return:
 *      uint32_t:       The 32-bit word containing the packed values.
 *
 * Expects:
 *      The pointer q must not be NULL.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
uint32_t packWord(Quantized q)
{
        assert(q != NULL);

        uint32_t word = 0;

        word = Bitpack_newu(word, 9, 23, q->a);
        word = Bitpack_news(word, 5, 18, q->b);
        word = Bitpack_news(word, 5, 13, q->c);
        word = Bitpack_news(word, 5, 8, q->d);
        word = Bitpack_newu(word, 4, 4, q->Pbar_b);
        word = Bitpack_newu(word, 4, 0, q->Pbar_r);

        return word;
}

/********** unpackWord ********
 *
 * Unpacks a 32-bit word into a Quantized structure. The word is stored in the
 * following format:
 *     a: 9 bits, stored at bits 23-31
 *     b: 5 bits, stored at bits 18-22
 *     c: 5 bits, stored at bits 13-17
 *     d: 5 bits, stored at bits 8-12
 *     Pbar_b: 4 bits, stored at bits 4-7
 *     Pbar_r: 4 bits, stored at bits 0-3
 *
 * Parameters:
 *      uint32_t word:   The 32-bit word containing the packed values.
 *
 * Return:
 *      Quantized:       A Quantized structure containing the
 *                       unpacked values.
 *
 * Expects:
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned Quantized structure.
 ************************/
Quantized unpackWord(uint32_t word)
{
        Quantized q;
        NEW(q);

        q->a = Bitpack_getu(word, 9, 23);
        q->b = Bitpack_gets(word, 5, 18);
        q->c = Bitpack_gets(word, 5, 13);
        q->d = Bitpack_gets(word, 5, 8);
        q->Pbar_b = Bitpack_getu(word, 4, 4);
        q->Pbar_r = Bitpack_getu(word, 4, 0);

        return q;
}