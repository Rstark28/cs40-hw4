#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <except.h>
#include <math.h>
#include <stdbool.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "rgb2cv.h"
#include "dct.h"
#include "stdlib.h"
#include "mem.h"

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

        return 0;
}