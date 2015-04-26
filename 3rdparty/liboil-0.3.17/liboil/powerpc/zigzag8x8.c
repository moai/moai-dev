/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>


/* Derivation:
 * zigzag_order in zigzag8z8_c.c is defined in terms of where the
 * elements are moving from.  We rewrite the matrix in terms of
 * where the elements are moving _to_ (which is the same as
 * unzigzag_order):
 *
 *         0,  1,  5,  6,  14, 15, 27, 28,
 *         2,  4,  7,  13, 16, 26, 29, 42,
 *         3,  8,  12, 17, 25, 30, 41, 43,
 *         9,  11, 18, 24, 31, 40, 44, 53,
 *         10, 19, 23, 32, 39, 45, 52, 54,
 *         20, 22, 33, 38, 46, 51, 55, 60,
 *         21, 34, 37, 47, 50, 56, 59, 61,
 *         35, 36, 48, 49, 57, 58, 62, 63,
 *
 * If we shift each successive row to the right by one place, we
 * get:
 *
 *         0,  1,  5,  6,  14, 15, 27, 28,
 *         42, 2,  4,  7,  13, 16, 26, 29,
 *         41, 43, 3,  8,  12, 17, 25, 30,
 *         40, 44, 53, 9,  11, 18, 24, 31,
 *         39, 45, 52, 54, 10, 19, 23, 32,
 *         38, 46, 51, 55, 60, 20, 22, 33,
 *         47, 50, 56, 59, 61, 21, 34, 37,
 *         36, 48, 49, 57, 58, 62, 63, 35,
 *
 * And by transposing,
 *
 *         0,  42, 41, 40, 39, 38, 47, 36,
 *         1,  2,  43, 44, 45, 46, 50, 48,
 *         5,  4,  3,  53, 52, 51, 56, 49,
 *         6,  7,  8,  9,  54, 55, 59, 57,
 *         14, 13, 12, 11, 10, 60, 61, 58,
 *         15, 16, 17, 18, 19, 20, 21, 62,
 *         27, 26, 25, 24, 23, 22, 34, 63,
 *         28, 29, 30, 31, 32, 33, 37, 35,
 *
 * we see that groups of numbers tend to be collected on the same
 * line.  In fact, aside from a few exceptions, rows on our final
 * matrix can be created simply by merging 2 rows of the above
 * matrix.  For example, the first line can (almost) be created
 * by merging lines 2 (called 'a') and 3 ('b') above:
 *
 *         xx, a0, a1, b2, b1, b0, xx, xx,
 *
 * (where 'xx' denotes elements that cannot be created. */


#if 0
/* 00 indicates that the element can't be handled by vperm, and needs
 * to be fixed up later. */
static const uint8_t mangle[128] __attribute__ ((__aligned__ (16))) = {
	00,00, 2, 3, 0, 1,16,17,18,19,20,21,00,00,00,00, /* 1, 2 */
	 2, 3, 0, 1,16,17,18,19,20,21,22,23,24,25,00,00, /* 3, 4 */
	 8, 9, 6, 7, 4, 5, 2, 3, 0, 1,16,17,18,19,20,21, /* 5, 6 */
	 6, 7, 8, 9,10,11,12,13,30,31,28,29,26,27,24,25, /* 6, 7 */
	 6, 7, 4, 5, 2, 3, 0, 1,18,19,20,21,22,23,24,25, /* 7, 0 */
	10,11,12,13,14,15,30,31,28,29,26,27,24,25,22,23, /* 0, 1 */
	00,00, 6, 7, 8, 9,10,11,12,13,14,15,30,31,28,29, /* 2, 3 */
	00,00,00,00,10,11,12,13,14,15,30,31,28,29,00,00, /* 4, 5 */
};

static void
zigzag8x8_s16_a16_altivec(int16_t *dest, int dstr, int16_t *src, int sstr)
{
	__asm__ __volatile__(
                "lvx 0, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 1, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 2, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 3, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 4, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 5, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 6, %0, %1\n"
                "\tadd %0, %0, %3\n"
                "\tlvx 7, %0, %1\n"
                "\tadd %0, %0, %3\n"
        : "+b" (src)
        : "b" (0), "b" (8), "b" (sstr)
        );

	/* "slide" vectors to right */
	__asm__ __volatile__(
		"vsldoi 0,0,0,0\n"
		"vsldoi 1,1,1,14\n"
		"vsldoi 2,2,2,12\n"
		"vsldoi 3,3,3,10\n"
		"vsldoi 4,4,4,8\n"
		"vsldoi 5,5,5,6\n"
		"vsldoi 6,6,6,4\n"
		"vsldoi 7,7,7,2\n"
	);

	/* transpose */
	__asm__ __volatile__(
		"vmrghh    8, 0, 4\n"
		"\tvmrglh  9, 0, 4\n"
		"\tvmrghh 10, 1, 5\n"
		"\tvmrglh 11, 1, 5\n"
		"\tvmrghh 12, 2, 6\n"
		"\tvmrglh 13, 2, 6\n"
		"\tvmrghh 14, 3, 7\n"
		"\tvmrglh 15, 3, 7\n"

		"\tvmrghh 16,  8, 12\n"
		"\tvmrglh 17,  8, 12\n"
		"\tvmrghh 18,  9, 13\n"
		"\tvmrglh 19,  9, 13\n"
		"\tvmrghh 20, 10, 14\n"
		"\tvmrglh 21, 10, 14\n"
		"\tvmrghh 22, 11, 15\n"
		"\tvmrglh 23, 11, 15\n"

		"\tvmrghh  0, 16, 20\n"
		"\tvmrglh  1, 16, 20\n"
		"\tvmrghh  2, 17, 21\n"
		"\tvmrglh  3, 17, 21\n"
		"\tvmrghh  4, 18, 22\n"
		"\tvmrglh  5, 18, 22\n"
		"\tvmrghh  6, 19, 23\n"
		"\tvmrglh  7, 19, 23\n"
	);

	sl_altivec_load8_8(mangle,16);

	__asm__ __volatile__(
		"\n"
		"\tvperm 16,1,2,8\n"
		"\tvperm 17,3,4,9\n"
		"\tvperm 18,5,6,10\n"
		"\tvperm 19,6,7,11\n"
		"\tvperm 20,7,0,12\n"
		"\tvperm 21,0,1,13\n"
		"\tvperm 22,2,3,14\n"
		"\tvperm 23,4,5,15\n"
	);

	sl_altivec_store8_16(dest,16);

	/* fix up the elements that were missed */

	block8x8_s16(dest,dstr,0,0) = block8x8_s16(src,sstr,0,0);
	block8x8_s16(dest,dstr,0,6) = block8x8_s16(src,sstr,3,0);
	block8x8_s16(dest,dstr,0,7) = block8x8_s16(src,sstr,2,1);
	block8x8_s16(dest,dstr,1,7) = block8x8_s16(src,sstr,5,0);

	block8x8_s16(dest,dstr,6,0) = block8x8_s16(src,sstr,2,7);
	block8x8_s16(dest,dstr,7,0) = block8x8_s16(src,sstr,5,6);
	block8x8_s16(dest,dstr,7,1) = block8x8_s16(src,sstr,4,7);
	block8x8_s16(dest,dstr,7,7) = block8x8_s16(src,sstr,7,7);
}
OIL_DEFINE_IMPL_FULL (zigzag8x8_s16_altivec, zigzag8x8_s16, OIL_FLAG_ALTIVEC);
#endif


