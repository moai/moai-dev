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
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS (argb_paint_u8);


#define div255(x) (((x + 128) + ((x + 128)>>8))>>8)
#define blend(x,y,a) div255((x)*(a) + (y)*(255-(a)))

static short constants[][4] = {
  { 255, 255, 255, 255 },
  { 128, 128, 128, 128 }
};

static void
argb_paint_u8_mmx (uint8_t *dest, uint8_t *color, uint8_t *alpha, int n)
{
  if (n<1)return;
  asm volatile (
      "  pxor %%mm0, %%mm0\n"
      "  movq (%1), %%mm3\n"
      "  punpcklbw %%mm0, %%mm3\n"
      "  movl $0, %1\n"
      "1:\n"
      "  movq (%0), %%mm1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  movb (%2), %%al\n"
      "  je 4f\n"
      "  cmpl $255, %1\n"
      "  jne 2f\n"
      "  movq %%mm3, %%mm2\n"
      "  jmp 3f\n"
      "2:\n"
      "  movd %1, %%mm2\n"
      "  pshufw $0x00, %%mm2, %%mm2\n"
      "  movq 0(%4), %%mm4\n"
      "  psubw %%mm2, %%mm4\n"
      "  pmullw %%mm1, %%mm4\n"
      "  pmullw %%mm3, %%mm2\n"
      "  paddw %%mm4, %%mm2\n"
      "  paddw 8(%4), %%mm2\n"
      "  movq %%mm2, %%mm1\n"
      "  psrlw $8, %%mm1\n"
      "  paddw %%mm1, %%mm2\n"
      "  psrlw $8, %%mm2\n"
      "3: \n"
      "  packuswb %%mm0, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "4:\n"
      "  add $4, %0\n"
      "  add $1, %2\n"
      "  decl %3\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (dest), "+a" (color), "+r" (alpha), "+r" (n)
      : "r" (&constants));
}
OIL_DEFINE_IMPL_FULL (argb_paint_u8_mmx, argb_paint_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);


