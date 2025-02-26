#include "pnm.h"
#include "assert.h"
#include "mem.h"
#include "rgb2cv.h"

ComponentVideo RGBtoCV(Pnm_rgb rgb, int denominator)
{
    assert(rgb != NULL);
    // convert to floats

    float redf = (float)rgb->red;
    float greenf = (float)rgb->green;
    float bluef = (float)rgb->blue;

    float red = (redf) / denominator;
    float green = (greenf) / denominator;
    float blue = (bluef) / denominator;

    ComponentVideo cv;
    NEW(cv);

    /* Convert to component video */
    cv->y = 0.299 * red + 0.587 * green + 0.114 * blue;
    cv->pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
    cv->pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;

    // printf("y=%f, pb=%f, pr=%f", y, pb, pr);

    return cv;
}

Pnm_rgb CVtoRGB(ComponentVideo cv, int denominator)
{
    printf("%f\n", cv->y);
    float red = 1.0 * cv->y + 0.0 * cv->pb + 1.402 * cv->pr;
    float green = 1.0 * cv->y - 0.344136 * cv->pb - 0.714136 * cv->pr;
    float blue = 1.0 * cv->y + 1.772 * cv->pb + 0.0 * cv->pr;

    printf("%f %f %f\n\n", red, green, blue);

    Pnm_rgb rgb;
    NEW(rgb);

    rgb->red = red * denominator;
    rgb->green = green * denominator;
    rgb->blue = blue * denominator;

    return rgb;
}