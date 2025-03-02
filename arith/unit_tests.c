#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <except.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb2cv.h"
#include "dct.h"
#include "stdlib.h"
#include "mem.h"
#include "bitpack.h"

/* Helper function to compare floating-point if not exactly equal */
bool close_f(double a, double b)
{
        return fabs(a - b) < 0.001;
}

bool close_i(int a, int b)
{
        return abs(a - b) <= 1;
}

/*****************************************************************
 *                          rgb2cv Tests
 *****************************************************************/
void test_RGBtoCV()
{
        Pnm_rgb rgb;
        NEW(rgb);
        rgb->red = 255;
        rgb->green = 0;
        rgb->blue = 0;

        ComponentVideo cv = RGBtoCV(rgb, 255);

        assert(close_f(cv->Y, 0.299));
        assert(close_f(cv->P_b, -0.168736));
        assert(close_f(cv->P_r, 0.5));

        FREE(rgb);
        FREE(cv);
}
void test_CVtoRGB()
{
        ComponentVideo cv;
        NEW(cv);
        cv->Y = 0.299;
        cv->P_b = -0.168736;
        cv->P_r = 0.5;

        Pnm_rgb rgb = CVtoRGB(cv, 255);

        assert(rgb->red == 255);
        assert(rgb->green == 0);
        assert(rgb->blue == 0);

        FREE(rgb);
        FREE(cv);
}

void test_RGBtoCV_and_back()
{
        Pnm_rgb rgb;
        NEW(rgb);
        rgb->red = 100;
        rgb->green = 100;
        rgb->blue = 100;

        ComponentVideo cv = RGBtoCV(rgb, 255);
        Pnm_rgb rgb2 = CVtoRGB(cv, 255);

        assert(close_i(rgb->red, rgb2->red));
        assert(close_i(rgb->green, rgb2->green));
        assert(close_i(rgb->blue, rgb2->blue));

        FREE(rgb);
        FREE(cv);
        FREE(rgb2);
}

void test_bitpack_fitsu()
{
        assert(Bitpack_fitsu(5, 3));
        assert(Bitpack_fitsu(7, 3));
        assert(!Bitpack_fitsu(8, 3));
        assert(Bitpack_fitsu(~0ULL, 64));
        assert(Bitpack_fitsu(0, 0));
        assert(!Bitpack_fitsu(1, 0));
}

void test_bitpack_fitss()
{
        assert(!Bitpack_fitss(5, 3));
        assert(Bitpack_fitss(-2, 3));
        assert(Bitpack_fitss(2, 3));
        assert(Bitpack_fitss(INT64_MAX, 64));
        assert(!Bitpack_fitss(0, 0));
}

void test_bitpack_getu()
{
        assert(Bitpack_getu(0x000000000000000F, 4, 0) == 0xF);
        assert(Bitpack_getu(0x000000000000000F, 4, 12) == 0x0);
        assert(Bitpack_getu(0x123456789ABCDEF0, 32, 0) == 0x9ABCDEF0);
        assert(Bitpack_getu(0x123456789ABCDEF0, 32, 32) == 0x12345678);
        assert(Bitpack_getu(UINT64_MAX, 64, 0) == UINT64_MAX);
}

void test_bitpack_gets()
{
        assert(Bitpack_gets(0x000000000000000F, 4, 0) == -1);
        assert(Bitpack_gets(0x000000000000000F, 4, 12) == 0x0);
        assert(Bitpack_gets(0x123456789ABCDEF0, 32, 32) == 0x12345678);
        assert(Bitpack_gets(INT64_MAX, 64, 0) == INT64_MAX);
}

void test_bitpack_newu()
{
        assert(Bitpack_newu(0x3, 2, 2, 3) == 15);
        assert(Bitpack_newu(0xFF, 4, 4, 0) == 15);
}

void test_bitpack_news()
{
        assert(Bitpack_newu(0x3, 2, 2, 3) == 15);
        assert(Bitpack_newu(0xFF, 4, 4, 0) == 15);
}

void test_bitpack_u()
{
        uint64_t a = ~0;

        a = Bitpack_newu(a, 4, 4, 13);
        a = Bitpack_newu(a, 8, 16, 230);
        a = Bitpack_newu(a, 4, 0, 12);

        assert(Bitpack_getu(a, 4, 4) == 13);
        assert(Bitpack_getu(a, 8, 16) == 230);
        assert(Bitpack_getu(a, 4, 0) == 12);
}

void test_bitpack_s()
{
        uint64_t a = 0;

        a = Bitpack_news(a, 4, 4, -3);
        a = Bitpack_news(a, 8, 16, -6);
        a = Bitpack_news(a, 4, 0, -7);
        a = Bitpack_news(a, 8, 24, 100);
        assert(Bitpack_gets(a, 4, 4) == -3);
        assert(Bitpack_gets(a, 8, 16) == -6);
        assert(Bitpack_gets(a, 4, 0) == -7);
        assert(Bitpack_gets(a, 8, 24) == 100);
}

/*****************************************************************
 *                          Main Function
 *****************************************************************/
int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        test_RGBtoCV();
        test_CVtoRGB();
        test_RGBtoCV_and_back();
        test_bitpack_fitsu();
        test_bitpack_fitss();
        test_bitpack_getu();
        test_bitpack_gets();
        test_bitpack_newu();
        test_bitpack_news();
        test_bitpack_u();
        test_bitpack_s();

        return 0;
}