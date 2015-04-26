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

/**
 * oil_average2_u8:
 * @d:
 * @dstr:
 * @s1:
 * @sstr1:
 * @s2:
 * @sstr2:
 * @n:
 *
 * Averages each element in @s1 and @s2 and places the result in @d.
 *
 * FIXME: This class is difficult to optimize and will likely be
 * replaced.
 */
OIL_DEFINE_CLASS (average2_u8, 
    "uint8_t *d, int dstr, uint8_t *s1, int sstr1, "
    "uint8_t *s2, int sstr2, int n");

static void
average2_u8_ref (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    dest[dstr * i] = (src1[sstr1 * i] + src2[sstr2 * i]) >> 1;
  }
}

OIL_DEFINE_IMPL_REF (average2_u8_ref, average2_u8);

