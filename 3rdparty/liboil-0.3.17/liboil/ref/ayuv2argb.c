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
#include <liboil/liboilcolorspace.h>
#include <liboil/liboiltest.h>
#include <string.h>

/**
 * oil_ayuv2argb_u8:
 * @d_4xn:
 * @s_4xn:
 * @n:
 *
 * Converts AYUV pixels to ARGB pixels.  AYUV pixels are in the
 * JPEG colorspace.  Note that this function doesn't follow normal
 * liboil pixel conventions.
 *
 * (This function should be replaced by one that handles other
 * conversion factors.)
 */
OIL_DEFINE_CLASS (ayuv2argb_u8, "uint8_t *d_4xn, uint8_t *s_4xn, int n");


#define clamp(x,a,b) clamp_lower(clamp_upper(x,b),a)
#define clamp_lower(x,a) ((x<a)?(a):(x))
#define clamp_upper(x,b) ((x>b)?(b):(x))

/* from the JFIF spec */
#define YUV_TO_R(y,u,v) clamp((y) + 1.402*((v)-128.0),0,255)
#define YUV_TO_G(y,u,v) clamp((y) - 0.34414*((u)-128.0) - 0.71414*((v)-128.0),0,255)
#define YUV_TO_B(y,u,v) clamp((y) + 1.772*((u)-128.0),0,255)

#define muldiv256(x,y) (((x)*(y) + 128)>>8)
#define YUV_TO_R_INT8(y,u,v) clamp((y) + muldiv256(359,(v)-128),0,255)
#define YUV_TO_G_INT8(y,u,v) clamp((y) + muldiv256(-88,(u)-128) + muldiv256(-183,(v)-128),0,255)
#define YUV_TO_B_INT8(y,u,v) clamp((y) + muldiv256(454,(u)-128),0,255)

#define muldiv65536(x,y) (((x)*(y) + 32768)>>16)
#define YUV_TO_R_INT16(y,u,v) clamp((y) + muldiv65536(91881,(v)-128),0,255)
#define YUV_TO_G_INT16(y,u,v) clamp((y) - muldiv65536(22554,(u)-128) - muldiv65536(46802,(v)-128),0,255)
#define YUV_TO_B_INT16(y,u,v) clamp((y) + muldiv65536(116130,(u)-128),0,255)


#define SHIFT 6
#define MULT (1<<6)
#define X(x) ((int)((x)*(1<<(SHIFT+8)) + 0.5))
static int16_t jfif_matrix[][4] = {
  { 0,      0,      -8192,   -8192  },
  { 16384,  0,      0,       0      },
  { 0,      16384,  16384,   16384  },
  { 0,      0,      -5638,   29032  },
  { 0,      22970,  -11700,  0      },
  { 0, 0, 0, 0 }
};

static void
colorspace_argb_test (OilTest *test)
{
  int16_t *data = oil_test_get_source_data (test, OIL_ARG_SRC2);

  memcpy (data, jfif_matrix, 4*2*6);
}

OIL_DEFINE_CLASS_FULL (colorspace_argb, "uint32_t *d, uint32_t *s, int16_t *s2_24, int n", colorspace_argb_test);

static void
colorspace_argb_ref (uint32_t *dest, const uint32_t *src, const int16_t *matrix, int n)
{
  int i;

  for(i=0;i<n;i++){
    int sa = (oil_argb_A(src[i])<<SHIFT) + matrix[0*4+0];
    int sr = (oil_argb_R(src[i])<<SHIFT) + matrix[0*4+1];
    int sg = (oil_argb_G(src[i])<<SHIFT) + matrix[0*4+2];
    int sb = (oil_argb_B(src[i])<<SHIFT) + matrix[0*4+3];
    int da, dr, dg, db;

#define MUL(a,b) (((a)*(b))>>16)
    da = (MUL(sa,matrix[1*4+0]) + MUL(sr,matrix[2*4+0]) + MUL(sg,matrix[3*4+0]) +
        MUL(sb,matrix[4*4+0]) + matrix[5*4+0] + 8)>>(2*SHIFT - 8);
    dr = (MUL(sa,matrix[1*4+1]) + MUL(sr,matrix[2*4+1]) + MUL(sg,matrix[3*4+1]) +
        MUL(sb,matrix[4*4+1]) + matrix[5*4+1] + 8)>>(2*SHIFT - 8);
    dg = (MUL(sa,matrix[1*4+2]) + MUL(sr,matrix[2*4+2]) + MUL(sg,matrix[3*4+2]) +
        MUL(sb,matrix[4*4+2]) + matrix[5*4+2] + 8)>>(2*SHIFT - 8);
    db = (MUL(sa,matrix[1*4+3]) + MUL(sr,matrix[2*4+3]) + MUL(sg,matrix[3*4+3]) +
        MUL(sb,matrix[4*4+3]) + matrix[5*4+3] + 8)>>(2*SHIFT - 8);
    dest[i] = oil_argb(da, dr, dg, db);
  }

}
OIL_DEFINE_IMPL_REF (colorspace_argb_ref, colorspace_argb);


static void
ayuv2argb_u8_ref (uint8_t *argb, const uint8_t *ayuv, int n)
{
  int i;

  for(i=0;i<n;i++){
    argb[0] = ayuv[0];
    argb[1] = YUV_TO_R_INT8(ayuv[1], ayuv[2], ayuv[3]);
    argb[2] = YUV_TO_G_INT8(ayuv[1], ayuv[2], ayuv[3]);
    argb[3] = YUV_TO_B_INT8(ayuv[1], ayuv[2], ayuv[3]);
    argb+=4;
    ayuv+=4;
  }

}
OIL_DEFINE_IMPL_REF (ayuv2argb_u8_ref, ayuv2argb_u8);


