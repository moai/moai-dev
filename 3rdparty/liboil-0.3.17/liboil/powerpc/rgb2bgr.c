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
#include <liboil/liboilfuncs.h>

OIL_DECLARE_CLASS (rgb2bgr);

static void
rgb2bgr_ppc (uint8_t *dest, uint8_t* src, int n)
{
  while (n&3) {
    uint8_t tmp;
    tmp = src[2];
    dest[1] = src[1];
    dest[2] = src[0];
    dest[0] = tmp;
    dest += 3;
    src += 3;
    n--;
  }
  n /= 4;
  dest -= 4;
  src -= 4;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lwzu r10, 4(%1)		\n" /* rgbr x */
	"	rotlwi r10, r10, 16	\n" /* brrg x */
	"	and r11, r10, %3		\n" /* -r-g .x */
	"	sub r10, r10, r11		\n" /* b-r- x. */
	"	andi. r12, r11, 0xFF	\n" /* ---g ..x */
	"	rotlwi r12, r12, 16	\n" /* -g-- ..x */
	"	or r10, r10, r12		\n" /* bgr- x.. */
	"	lwzu r12, 4(%1)		\n" /* gbrg ..x */
	"	and r9, r12, %4		\n" /* -br- ...x */
	"	sub r12, r12, r9		\n" /* g--g ..x. */
	"	rotlwi r9, r9, 16	\n" /* r--b ...x */
	"	andis. r11, r11, 0xFF	\n" /* -r-- .x.. */
	"	or r12, r12, r11		\n" /* gr-g ..x. */
	"	andi. r11, r9, 0xFF	\n" /* ---b .x.. */
	"	or r10, r10, r11		\n" /* bgrb x... */
	"	stwu r10, 4(%0)		\n"
	"	lwzu r10, 4(%1)		\n" /* brgb x... */
	"	rotlwi r10, r10, 16	\n" /* gbbr x... */
	"	and r11, r10, %3		\n" /* -b-r .x.. */
	"	sub r10, r10, r11		\n" /* g-b- x... */
	"	andis. r9, r9, 0xFF00	\n" /* r--- ...x */
	"	or r11, r11, r9		\n" /* rb-r .x.. */
	"	andi. r9, r10, 0xFF00	\n" /* --b- ...x */
	"       or r12, r12, r9		\n" /* grbg ..x. */
	"	stwu r12, 4(%0)		\n"
	"	andis. r10, r10, 0xFF00	\n" /* g--- x... */
	"	rotlwi r10, r10, 16	\n" /* --g- x... */
	"	or r11, r11, r10		\n" /* rbgr .x.. */
	"	stwu r11, 4(%0)		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n), "b" (0x00FF00FF), "b" (0x00FFFF00)
      : "10", "11", "12", "9", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc, rgb2bgr);

#ifdef HAVE_UNALIGNED_ACCESS
/* This doesn't work on ppc64. */
static void
rgb2bgr_ppc2 (uint8_t *dest, uint8_t* src, int n)
{
  asm volatile (
	"	mtctr %2		\n"
	"1:	lswi r10, %1, 3		\n"
	"	addi %1, %1, 3		\n"
	"	andis. r11, r10, 0xFF	\n"
	"	rotlwi r10, r10, 16	\n"
	"	or r10, r10, r11		\n"
	"	stswi r10, %0, 3		\n"
	"	addi %0, %0, 3		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "11", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc2, rgb2bgr);
#endif

static void
rgb2bgr_ppc3 (uint8_t *dest, uint8_t* src, int n)
{
  dest--;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lbz r10, 2(%1)		\n"
	"	stbu r10, 1(%0)		\n"
	"	lbz r10, 1(%1)		\n"
	"	stbu r10, 1(%0)		\n"
	"	lbz r10, 0(%1)		\n"
	"	stbu r10, 1(%0)		\n"
	"	addi %1, %1, 3		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc3, rgb2bgr);

static void
rgb2bgr_ppc4 (uint8_t *dest, uint8_t* src, int n)
{
  dest--;
  src--;
  asm volatile (
	"	mtctr %2		\n"
	"1:	lbzu r10, 1(%1)		\n"
	"	lbzu r11, 1(%1)		\n"
	"	lbzu r12, 1(%1)		\n"
	"	stbu r12, 1(%0)		\n"
	"	stbu r11, 1(%0)		\n"
	"	stbu r10, 1(%0)		\n"
	"	bdnz 1b			\n"
      : "+b" (dest), "+b" (src)
      : "b" (n)
      : "10", "11", "12", "ctr");
}

OIL_DEFINE_IMPL_ASM (rgb2bgr_ppc4, rgb2bgr);

