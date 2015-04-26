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
#include <liboil/liboilcolorspace.h>


/**
 * oil_recon8x8_intra:
 * @d_8x8:
 * @ds:
 * @s_8x8:
 *
 * Adds 128 to each value in the source array, clamps to the range [0,255],
 * and places the result in the destination array.
 */
OIL_DEFINE_CLASS (recon8x8_intra,
    "uint8_t *d_8x8, int ds, int16_t *s_8x8");
/**
 * oil_recon8x8_inter:
 * @d_8x8:
 * @ds:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 *
 * Adds each element in @s1_8x8 and @s2_8x8, clamps to the range [0,255],
 * and places the result in the destination array.
 */
OIL_DEFINE_CLASS (recon8x8_inter,
    "uint8_t *d_8x8, int ds, uint8_t *s1_8x8, int ss1, int16_t *s2_8x8");
/**
 * oil_recon8x8_inter2:
 * @d_8x8:
 * @ds:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 * @s3_8x8:
 *
 * Adds each element in @s1_8x8 and @s2_8x8, divides by 2, and adds
 * to @s3_8x8, clamps to the range [0,255], and places the result in
 * the destination array.
 */
OIL_DEFINE_CLASS (recon8x8_inter2,
    "uint8_t *d_8x8, int ds, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2, int16_t *s3_8x8");


static void
recon8x8_intra_ref (uint8_t *dest, int ds, int16_t *change)
{
  uint32_t i;

  for (i = 8; i; i--){
    dest[0] = oil_clamp_255(change[0] + 128);
    dest[1] = oil_clamp_255(change[1] + 128);
    dest[2] = oil_clamp_255(change[2] + 128);
    dest[3] = oil_clamp_255(change[3] + 128);
    dest[4] = oil_clamp_255(change[4] + 128);
    dest[5] = oil_clamp_255(change[5] + 128);
    dest[6] = oil_clamp_255(change[6] + 128);
    dest[7] = oil_clamp_255(change[7] + 128);

    dest += ds;
    change += 8;
  }
}

OIL_DEFINE_IMPL_REF (recon8x8_intra_ref, recon8x8_intra);

static void
recon8x8_inter_ref (uint8_t *dest, int ds, uint8_t *src, int ss, int16_t *change, int dss)
{
  uint32_t i;

  for (i = 8; i; i--){
    dest[0] = oil_clamp_255(src[0] + change[0]);
    dest[1] = oil_clamp_255(src[1] + change[1]);
    dest[2] = oil_clamp_255(src[2] + change[2]);
    dest[3] = oil_clamp_255(src[3] + change[3]);
    dest[4] = oil_clamp_255(src[4] + change[4]);
    dest[5] = oil_clamp_255(src[5] + change[5]);
    dest[6] = oil_clamp_255(src[6] + change[6]);
    dest[7] = oil_clamp_255(src[7] + change[7]);

    change += 8;
    dest += ds;
    src += ss;
  }
}

OIL_DEFINE_IMPL_REF (recon8x8_inter_ref, recon8x8_inter);

static void
recon8x8_inter2_ref (uint8_t *dest, int ds, uint8_t *s1, int ss1, uint8_t *s2, int ss2, int16_t *change)
{
  uint32_t  i;

  for (i = 8; i; i--){
    dest[0] = oil_clamp_255((((int16_t)s1[0] + (int16_t)s2[0]) >> 1) + change[0]);
    dest[1] = oil_clamp_255((((int16_t)s1[1] + (int16_t)s2[1]) >> 1) + change[1]);
    dest[2] = oil_clamp_255((((int16_t)s1[2] + (int16_t)s2[2]) >> 1) + change[2]);
    dest[3] = oil_clamp_255((((int16_t)s1[3] + (int16_t)s2[3]) >> 1) + change[3]);
    dest[4] = oil_clamp_255((((int16_t)s1[4] + (int16_t)s2[4]) >> 1) + change[4]);
    dest[5] = oil_clamp_255((((int16_t)s1[5] + (int16_t)s2[5]) >> 1) + change[5]);
    dest[6] = oil_clamp_255((((int16_t)s1[6] + (int16_t)s2[6]) >> 1) + change[6]);
    dest[7] = oil_clamp_255((((int16_t)s1[7] + (int16_t)s2[7]) >> 1) + change[7]);

    change += 8;
    dest += ds;
    s1 += ss1;
    s2 += ss2;
  }
}

OIL_DEFINE_IMPL_REF (recon8x8_inter2_ref, recon8x8_inter2);
