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

OIL_DEFINE_CLASS (argb_paint_u8, "uint8_t *i_4xn, uint8_t *s1_4, uint8_t *s2_n, int n");


#define div255(x) (((x + 128) + ((x + 128)>>8))>>8)
#define blend(x,y,a) div255((x)*(a) + (y)*(255-(a)))

/**
 * oil_argb_paint_u8:
 * @i_4xn: array
 * @s1_4: source color
 * @s2_n: source alpha array
 * @n: number of elements
 *
 * Composites source color onto in-place array according to the alpha
 * array.
 *
 * Deprecated.
 */
static void
argb_paint_u8_ref (uint8_t *dest, const uint8_t *color, const uint8_t *alpha, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[0] = blend(color[0],dest[0],alpha[0]);
    dest[1] = blend(color[1],dest[1],alpha[0]);
    dest[2] = blend(color[2],dest[2],alpha[0]);
    dest[3] = blend(color[3],dest[3],alpha[0]);
    dest+=4;
    alpha++;
  }

}
OIL_DEFINE_IMPL_REF (argb_paint_u8_ref, argb_paint_u8);

