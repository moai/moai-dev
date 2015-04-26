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
#include <liboil/liboiltest.h>
#include "jpeg.h"

/**
 * oil_yuv2rgbx_u8:
 * @d_4xn:
 * @src1: Y component
 * @src2: U component
 * @src3: V component
 * @n:
 *
 * Converts YUV pixels to RGB pixels.  Each YUV component is in a
 * separate source array, and are combined and converted to RGB.
 *
 * This function should be replaced by one that makes sense.
 */
OIL_DEFINE_CLASS (yuv2rgbx_u8,
    "uint8_t *d_4xn, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");
/**
 * oil_yuv2rgbx_sub2_u8:
 * @d_4xn:
 * @src1: Y component
 * @src2: U component
 * @src3: V component
 * @n:
 *
 * Converts YUV pixels to RGB pixels.  Each YUV component is in a
 * separate source array, and are combined and converted to RGB.
 * The U and V arrays are subsampled by a factor of 2, so only
 * half of each array is used.
 *
 * This function should be replaced by one that makes sense.
 */
OIL_DEFINE_CLASS (yuv2rgbx_sub2_u8,
    "uint8_t *d_4xn, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");
/**
 * oil_yuv2rgbx_sub4_u8:
 * @d_4xn:
 * @src1: Y component
 * @src2: U component
 * @src3: V component
 * @n:
 *
 * Converts YUV pixels to RGB pixels.  Each YUV component is in a
 * separate source array, and are combined and converted to RGB.
 * The U and V arrays are subsampled by a factor of 4, so only
 * a quarter of each array is used.
 *
 * This function should be replaced by one that makes sense.
 */
OIL_DEFINE_CLASS (yuv2rgbx_sub4_u8,
    "uint8_t *d_4xn, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");

#define clamp(x,a,b) clamp_lower(clamp_upper(x,b),a)
#define clamp_lower(x,a) ((x<a)?(a):(x))
#define clamp_upper(x,b) ((x>b)?(b):(x))

/* from the JFIF spec */
#define YUV_TO_R(y,u,v) clamp((y) + 1.402*((v)-128),0,255)
#define YUV_TO_G(y,u,v) clamp((y) - 0.34414*((u)-128) - 0.71414*((v)-128),0,255)
#define YUV_TO_B(y,u,v) clamp((y) + 1.772*((u)-128),0,255)

#define YUV_TO_R_INT(y,u,v) clamp(((y)*256 + 358*((v)-128))>>8,0,255)
#define YUV_TO_G_INT(y,u,v) clamp(((y)*256 - 88*((u)-128) - 183*((v)-128))>>8,0,255)
#define YUV_TO_B_INT(y,u,v) clamp(((y)*256 + 454*((u)-128))>>8,0,255)


static void
yuv2rgbx_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_u8_ref, yuv2rgbx_u8);

#ifdef ENABLE_BROKEN_IMPLS
static void
yuv2rgbx_u8_int (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n;i++){
		rgbp[0] = YUV_TO_R_INT(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G_INT(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B_INT(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL (yuv2rgbx_u8_int, yuv2rgbx_u8);
#endif

static void
yuv2rgbx_sub2_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_sub2_u8_ref, yuv2rgbx_sub2_u8);

static void
yuv2rgbx_sub4_u8_ref (uint8_t *rgbp, uint8_t *yp, uint8_t *up, uint8_t *vp, int n)
{
	int i;

	for(i=0;i<n/4;i++){
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		rgbp[0] = YUV_TO_R(yp[0], up[0], vp[0]);
		rgbp[1] = YUV_TO_G(yp[0], up[0], vp[0]);
		rgbp[2] = YUV_TO_B(yp[0], up[0], vp[0]);
		rgbp[3] = 0;
		rgbp+=4;
		yp++;
		up++;
		vp++;
	}
}

OIL_DEFINE_IMPL_REF (yuv2rgbx_sub4_u8_ref, yuv2rgbx_sub4_u8);

