/**************************************************************
 *
 *                     packword.h
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       3/2/2025
 *
 *     This file contains the interface for the pack word module. It provides
 *     functions to pack and unpack quantized values from a 32-bit word.
 *
 ************************/

#ifndef PACK_WORD_H
#define PACK_WORD_H

#include <stdio.h>
#include "quantize.h"

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
uint32_t packWord(Quantized q);

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
 *      Quantized q:     A Quantized structure to store the unpacked values.
 *
 * Expects:
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 *      Client is responsible for freeing the returned Quantized structure.
 ************************/
void unpackWord(Quantized q,
                uint32_t word);

#endif