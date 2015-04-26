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

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>
#include <math.h>


#if 0
static void
sad8x8_s16_a16_altivec (uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvmrghh 20, 19, 18\n"
		"\tvmrglh 21, 19, 18\n"
		"\tvsumsws 22, 20, 22\n"
		"\tvsumsws 22, 21, 22\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}

static void
sad8x8_s16_l15_a16_altivec(uint32_t *dest, int16_t *src1, int16_t *src2, int s1str, int s2str)
{
	static uint32_t x[4] __attribute__ ((__aligned__ (16)));

	sl_altivec_load8_0(src1, s1str);
	sl_altivec_load8_8(src2, s2str);

	__asm__ __volatile__(
		"\n"
		"\tvspltisw 19, 0\n"
		"\tvspltisw 22, 0\n"

		"\tvminsh 16, 0, 8\n"
		"\tvmaxsh 17, 0, 8\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 1, 9\n"
		"\tvmaxsh 17, 1, 9\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 2, 10\n"
		"\tvmaxsh 17, 2, 10\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 3, 11\n"
		"\tvmaxsh 17, 3, 11\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 4, 12\n"
		"\tvmaxsh 17, 4, 12\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 5, 13\n"
		"\tvmaxsh 17, 5, 13\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 6, 14\n"
		"\tvmaxsh 17, 6, 14\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvminsh 16, 7, 15\n"
		"\tvmaxsh 17, 7, 15\n"
		"\tvsubuhm 18, 17, 16\n"
		"\tvsum4shs 22, 18, 22\n"

		"\tvsumsws 22, 22, 19\n"

		"\tli 0, 0\n"
		"\tstvx 22, %0, 0\n"
	:
	: "r" (x)
	);
	*dest = x[3];
}
#endif


