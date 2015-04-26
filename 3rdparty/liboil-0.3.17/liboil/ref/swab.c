/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboiltest.h>


/**
 * oil_swab_u16:
 * @d_n:
 * @s_n:
 * n:
 *
 * Swaps the endianness of values in the source array and places the
 * results in the destination array.  The arguments @s_n and @d_n may
 * be equal.
 */
OIL_DEFINE_CLASS (swab_u16, "uint16_t *d_n, uint16_t *s_n, int n");

/**
 * oil_swab_u32:
 * @d_n:
 * @s_n:
 * n:
 *
 * Swaps the endianness of values in the source array and places the
 * results in the destination array.  The arguments @s_n and @d_n may
 * be equal.
 */
OIL_DEFINE_CLASS (swab_u32, "uint32_t *d_n, uint32_t *s_n, int n");

static void
swab_u16_ref (uint16_t *d, uint16_t *s, int n)
{
  int i;

  for(i=0;i<n;i++){
    d[i] = (s[i]<<8) | (s[i]>>8);
  }
}
OIL_DEFINE_IMPL_REF (swab_u16_ref, swab_u16);

static void
swab_u32_ref (uint32_t *d, uint32_t *s, int n)
{
  int i;

  for(i=0;i<n;i++){
    d[i] = (s[i]<<24) | ((s[i]&0x0000ff00)<<8) |
      ((s[i]&0x00ff0000)>>8) | (s[i]>>24);
  }
}
OIL_DEFINE_IMPL_REF (swab_u32_ref, swab_u32);

