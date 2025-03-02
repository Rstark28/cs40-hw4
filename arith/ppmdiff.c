/**************************************************************
 *
 *                     ppmdiff.c
 *
 *     Assignment: arith
 *     Authors:    Robert Stark (rstark03), Kyle Wigdor (kwigdo01)
 *     Date:       2/20/2025
 *
 ************************/

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <string.h>
#include <math.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

#define SQUARE(x) ((x) * (x))

static FILE *openFile(char *fname, char *mode);
void computeDiff(int col, int row, A2Methods_UArray2 array2, A2Methods_Object *ptr, void *cl);
float computeE(Pnm_ppm image1, Pnm_ppm image2);

/********** main ********
 *
 * The main function for the ppmdiff program. It reads two PPM images
 * from the specified files (or stdin), computes the difference between
 * them, and prints the result.
 *
 * Parameters:
 *      int argc:       The number of command-line arguments.
 *      char *argv[]:   An array of command-line arguments.
 *
 * Return:
 *      EXIT_SUCCESS if the program completes successfully,
 *      EXIT_FAILURE otherwise.
 *
 * Expects:
 *      The program expects at least two command-line arguments specifying
 *      the input files.
 *      If "-" is provided as an argument, the program reads from stdin.
 *      The program can only handle "-" for one of the input files.
 *
 * Notes:
 *      The program will print an error message and exit with EXIT_FAILURE
 *      if the images have significantly different dimensions or if there
 *      are issues with reading the files.
 ************************/
int main(int argc, char *argv[])
{
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
                        fprintf(stderr, "Can't read both inputs from stdin\n");
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

        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm image1 = Pnm_ppmread(i1_fp, methods);
        Pnm_ppm image2 = Pnm_ppmread(i2_fp, methods);

        if (image1->width > image2->width + 1 ||
            image2->width > image1->width + 1)
        {
                fprintf(stderr, "Image dimensions are too different.\n");
                return EXIT_FAILURE;
        }

        printf("%.4f\n", computeE(image1, image2));
        return EXIT_SUCCESS;
}

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

/********** Wrapper ********
 *
 * A structure to hold the second image, the sum of differences,
 * and the denominator of the first image.
 *
 * Fields:
 *      Pnm_ppm other_image: A pointer to the second image.
 *      float diff:          The sum of squared differences.
 *      int self_denom:      The denominator of the first image.
 *
 * Notes:
 *      The fields other_image and self_denom must be initialized
 *      before use. The sum field is used to accumulate the differences.
 ************************/
typedef struct Wrapper
{
        Pnm_ppm other_image;
        float diff;
        int self_denom;
} Wrapper;

/********** computeDiff ********
 *
 * Computes the squared differences between the RGB values of the
 * corresponding pixels in two images and accumulates the sum.
 *
 * Parameters:
 *      int col:                        The column index of the pixel.
 *      int row:                        The row index of the pixel.
 *      A2Methods_UArray2 array2:       The array containing the pixels of the
 *                                      first image.
 *      A2Methods_Object *ptr:          A pointer to the current pixel in the
 *                                      first image.
 *      void *cl:                       A pointer to a Wrapper structure
 *                                      containing the second image, the sum of
 *                                      squared differences, and the
 *                                      denominator of the first image.
 *
 * Return:
 *      void
 *
 * Expects:
 *      The pointers array2 and cl must not be NULL.
 *      The pixel coordinates (col, row) must be within the bounds of the
 *      first image.
 *
 * Notes:
 *      The function will not compute differences for pixels that are out of
 *      bounds in the second image.
 ************************/
void computeDiff(int col,
                 int row,
                 A2Methods_UArray2 array2,
                 A2Methods_Object *ptr, void *cl)
{
        assert(cl != NULL);
        assert(array2 != NULL);
        Wrapper *w = cl;

        /* Check if the pixel is out of bounds in the other image */
        Pnm_ppm other_image = w->other_image;
        if ((unsigned int)col >= other_image->width ||
            (unsigned int)row >= other_image->height)
        {
                return;
        }

        /* Get the RGB values of the current pixel in both images */
        Pnm_rgb rgb_self = (Pnm_rgb)ptr;
        Pnm_rgb rgb_other = other_image->methods->at(other_image->pixels, col, row);
        float self_denom = (float)w->self_denom;
        float other_denom = (float)other_image->denominator;

        /* Compute the differences in the RGB values */
        float r_diff = ((float)rgb_self->red / self_denom) -
                       ((float)rgb_other->red / other_denom);
        float g_diff = ((float)rgb_self->green / self_denom) -
                       ((float)rgb_other->green / other_denom);
        float b_diff = ((float)rgb_self->blue / self_denom) -
                       ((float)rgb_other->blue / other_denom);

        float diff = (r_diff * r_diff) +
                     (g_diff * g_diff) +
                     (b_diff * b_diff);

        w->diff += diff;
}

/********** computeE ********
 *
 * Computes the root mean square error (RMSE) between two PPM images.
 *
 * Parameters:
 *      Pnm_ppm image1: A pointer to the first image.
 *      Pnm_ppm image2: A pointer to the second image.
 *
 * Return:
 *      float: The computed RMSE value.
 *
 * Expects:
 *      The pointers image1 and image2 must not be NULL.
 *      The images must have similar dimensions (width and height).
 *
 * Notes:
 *      The function will compute the RMSE based on the overlapping region
 *      of the two images.
 ************************/
float computeE(Pnm_ppm image1, Pnm_ppm image2)
{
        assert(image1 != NULL && image2 != NULL);

        /* Get the minimum width and height of the two images */
        int min_width = image1->width > image2->width ? image2->width : image1->width;
        int min_height = image1->height > image2->height ? image2->height : image1->height;

        Wrapper w = {image2, 0, image1->denominator};
        image1->methods->map_row_major(image1->pixels, computeDiff, &w);
        float denom = 3 * min_width * min_height;

        return sqrt(w.diff / denom);
}