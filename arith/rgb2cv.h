#ifndef RGB2CV_H
#define RGB2CV_H

#include <stdio.h>
#include "pnm.h"

typedef struct ComponentVideo
{
    float y;
    float pb;
    float pr;
} *ComponentVideo;

ComponentVideo RGBtoCV(Pnm_rgb rgb, int denominator);

Pnm_rgb CVtoRGB(ComponentVideo cv, int denominator);

#endif