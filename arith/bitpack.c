#include "bitpack.h"
#include "assert.h"
#include "stdio.h"

#define MAX_WIDTH 64

/********** Bitpack_fitsu ********
 *
 * Determines if an unsigned value can fit within a specified bit width.
 *
 * Parameters:
 *      uint64_t n:        The unsigned value to check.
 *      unsigned width:    The bit width to check against.
 *
 * Return:
 *      bool:              True if the value can fit within the specified width,
 *                         false otherwise.
 *
 * Expects:
 *      The width is at most 64.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
bool Bitpack_fitsu(uint64_t n,
                   unsigned width)
{
        assert(width <= MAX_WIDTH);
        uint64_t upper_bound = (1ULL << width);
        return width == MAX_WIDTH || n < upper_bound;
}

/********** Bitpack_fitss ********
 *
 * Determines if a signed value can fit within a specified bit width.
 *
 * Parameters:
 *      int64_t n:         The signed value to check.
 *      unsigned width:    The bit width to check against.
 *
 * Return:
 *      bool:              True if the value can fit within the specified width,
 *                         false otherwise.
 *
 * Expects:
 *      The width is at most 64.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
bool Bitpack_fitss(int64_t n,
                   unsigned width)
{
        assert(width <= MAX_WIDTH);
        int64_t upper_bound = 1LL << (width - 1);
        int64_t lower_bound = -upper_bound;
        return width == MAX_WIDTH || ((n >= lower_bound) && (n < upper_bound));
}

/********** Bitpack_getu ********
 *
 * Extracts an unsigned value from a word.
 *
 * Parameters:
 *      uint64_t word:     The word to extract from.
 *      unsigned width:    The bit width of the value to extract.
 *      unsigned lsb:      The least significant bit of the value to extract.
 *
 * Return:
 *      uint64_t:          The extracted unsigned value.
 *
 * Expects:
 *      The width is at most 64.
 *      The sum of width and lsb is at most 64.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
uint64_t Bitpack_getu(uint64_t word,
                      unsigned width,
                      unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        return (word >> lsb) & mask;
}

/********** Bitpack_gets ********
 *
 * Extracts a signed value from a word.
 *
 * Parameters:
 *      uint64_t word:     The word to extract from.
 *      unsigned width:    The bit width of the value to extract.
 *      unsigned lsb:      The least significant bit of the value to extract.
 *
 * Return:
 *      int64_t:           The extracted signed value.
 *
 * Expects:
 *      The width is at most 64.
 *      The sum of width and lsb is at most 64.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
int64_t Bitpack_gets(uint64_t word,
                     unsigned width,
                     unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        int64_t value = (word >> lsb) & mask;

        /* If signed bit is on, then sign extend */
        if (value >> (width - 1))
        {
                value |= ~mask;
        }
        return value;
}

/********** Bitpack_newu ********
 *
 * Inserts an unsigned value into a word.
 *
 * Parameters:
 *      uint64_t word:     The word to insert into.
 *      unsigned width:    The bit width of the value to insert.
 *      unsigned lsb:      The least significant bit of the value to insert.
 *      uint64_t value:    The value to insert.
 *
 * Return:
 *      uint64_t:          The word with the value inserted.
 *
 * Expects:
 *      The width is at most 64.
 *      The sum of width and lsb is at most 64.
 *      The value must fit within the specified width.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
uint64_t Bitpack_newu(uint64_t word,
                      unsigned width,
                      unsigned lsb,
                      uint64_t value)
{
        assert(width <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        assert(Bitpack_fitsu(value, width));

        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        uint64_t cleared_word = word & ~(mask << lsb);

        return cleared_word | (value << lsb);
}

/********** Bitpack_news ********
 *
 * Inserts a signed value into a word.
 *
 * Parameters:
 *      uint64_t word:     The word to insert into.
 *      unsigned width:    The bit width of the value to insert.
 *      unsigned lsb:      The least significant bit of the value to insert.
 *      int64_t value:     The value to insert.
 *
 * Return:
 *      uint64_t:          The word with the value inserted.
 *
 * Expects:
 *      The width is at most 64.
 *      The sum of width and lsb is at most 64.
 *      The value must fit within the specified width.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
uint64_t Bitpack_news(uint64_t word,
                      unsigned width,
                      unsigned lsb,
                      int64_t value)
{
        assert(Bitpack_fitss(value, width));
        uint64_t mask = (width == MAX_WIDTH) ? ~0ULL : ((1ULL << width) - 1);
        return Bitpack_newu(word, width, lsb, (uint64_t)value & mask);
}
