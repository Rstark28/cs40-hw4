#ifndef PACK_WORD_H
#define PACK_WORD_H

#include <stdio.h>
#include "quantize.h"

uint32_t pack_word(Quantized q);

Quantized unpack_word(uint32_t word);

#endif