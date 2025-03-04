# Makefile for arith (CS 40 Assignment 4)
#
# ....
#
# New syntax rules added:  wildcards (%.o, %.c) and special variables:
# $@ - the target of the rule
# $< - the first dependency
# $^ - all dependencies
#
# Last updated: February 4, 2016


############## Variables ###############

CC = gcc   # The compiler being used

# Updating include path to use CS 40 .h files and CII interfaces
IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# Both programs need cii40 (Hanson binaries) and *may* need -lm (math)
# Only brightness requires the binary for pnmrdr.
# LDLIBS = -lpnmrdr -lnetpbm -lm
LDLIBS = -l40locality -lnetpbm -lcii40 -lm -lrt -larith40


# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: ppmdiff 40image


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

ppmdiff: ppmdiff.o uarray2.o a2plain.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

40image: 40image.o compress40.o a2plain.o uarray2.o rgb2cav.o dct.o quantize.o packword.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

unit_tests: unit_tests.o a2plain.o uarray2.o rgb2cav.o dct.o bitpack.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f ppmdiff *.o

