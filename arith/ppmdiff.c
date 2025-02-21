#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <string.h>
#include <math.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

/********** openFile ********
 *
 * Opens a file with the given filename and mode.
 *
 * Parameters:
 *      char *fname:    A pointer to the name of the file to be opened.
 *      char *mode:     A pointer to the mode string.
 *
 * Return:
 *      FILE *:         A pointer to the opened file.
 *
 * Expects:
 *      The pointers fname and mode must not be NULL.
 *      The file must exist and be accessible with the given mode.
 *
 * Notes:
 *      Will CRE if any expectation is violated.
 ************************/
static FILE *openFile(char *fname, char *mode)
{
    assert(fname != NULL && mode != NULL);
    FILE *fp = fopen(fname, mode);
    assert(fp != NULL);
    return fp;
}

typedef struct Wrapper {
    Pnm_ppm other_image;
    float sum;
    int self_denom;
} Wrapper;

void compute_difference(int col, int row, A2Methods_UArray2 array2,
                        A2Methods_Object *ptr, void *cl)
{
    assert(cl != NULL);
    assert(array2 != NULL);
    Wrapper *w = cl;

    Pnm_rgb rgb_this = (void*)ptr; 

    if ((col >= w->other_image->methods->width(w->other_image->pixels))
    || (row >= w->other_image->methods->height(w->other_image->pixels))) {
        return;
    }

    A2Methods_Object *other_ptr = w->other_image->methods->at(w->other_image->pixels, col, row);
    Pnm_rgb rgb_other = (void*)other_ptr;

    float red_diff = ((float)(rgb_this->red) / w->self_denom) - ((float)(rgb_other->red) / w->other_image->denominator);
    float green_diff = ((float)(rgb_this->green) / w->self_denom) - ((float)(rgb_other->green) / w->other_image->denominator);
    float blue_diff = ((float)(rgb_this->blue) / w->self_denom) - ((float)(rgb_other->blue) / w->other_image->denominator);

    float diff = (red_diff * red_diff) + (green_diff * green_diff) + (blue_diff * blue_diff);
    
    w->sum += diff;
}

float computeE(Pnm_ppm image1, Pnm_ppm image2) {
    assert(image1 != NULL && image2 != NULL);
    int min_width = image1->width > image2->width ? image2->width : image1->width;
    int min_height = image1->height > image2->height ? image2->height : image1->height;
    (void)min_width;
    (void)min_height;

    Wrapper w = {image2, 0, image1->denominator};

    image1->methods->map_row_major(image1->pixels, compute_difference, &w);

    float denom = 3 * min_width * min_height;
    return sqrt(w.sum / denom);
}

int main(int argc, char *argv[])
{
    (void)argv;
    A2Methods_T methods = uarray2_methods_plain;

    (void)methods;
    if (argc < 3)
    {
        return EXIT_FAILURE;
    }

    FILE *i1_fp;
    FILE *i2_fp;

    if (strcmp(argv[1], "-") == 0)
    {
        i1_fp = stdin;
    }
    else
    {
        i1_fp = openFile(argv[1], "r");
    }

    if (strcmp(argv[2], "-") == 0)
    {
        if (strcmp(argv[1], "-") == 0)
        {
            fprintf(stderr, "Could not read both inputs from stdin");
            return EXIT_FAILURE;
        }
        else
        {
            i2_fp = stdin;
        }
    }
    else
    {
        i2_fp = openFile(argv[2], "r");
    }

    Pnm_ppm image1 = Pnm_ppmread(i1_fp, methods);
    Pnm_ppm image2 = Pnm_ppmread(i2_fp, methods);

    (void)image1;
    (void)image2;

    if (((image1->width - image2->width) > 1) ||
        ((image1->height - image2->height) > 1))
    {
        fprintf(stderr, "Could not compare images with too large difference.\n");   
        printf("1.0");
        return EXIT_FAILURE;
    }

    float E = computeE(image1, image2);

    printf("%.4f\n", E);
 
    return EXIT_SUCCESS;
}