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
#include <liboil/liboilcolorspace.h>

/**
 * oil_rgb2bgr:
 * @d_3xn:
 * @s_3xn:
 * @n:
 *
 * Converts arrays of 24-bit RGB pixels from RGBRGBRGB ordering to
 * BGRBGRBGR ordering (and vice-versa).
 */
OIL_DEFINE_CLASS (rgb2bgr, "uint8_t *d_3xn, uint8_t* s_3xn, int n");
/**
 * oil_rgb2rgba:
 * @d_4xn:
 * @s_3xn:
 * @n:
 *
 * Converts arrays of 24-bit RGB pixels in RGBRGBRGB memory order
 * to 32-bit RGBA pixels in RGBARGBA order.
 */
OIL_DEFINE_CLASS (rgb2rgba, "uint8_t *d_4xn, uint8_t* s_3xn, int n");
/**
 * oil_rgb565_to_argb:
 * @d:
 * @s:
 * @n:
 *
 * Converts arrays of 16-bit RGB565 pixels to 32-bit ARGB pixels.
 */
OIL_DEFINE_CLASS (rgb565_to_argb, "uint32_t *d, uint16_t* s, int n");

static void
rgb2bgr_ref (uint8_t *dest, const uint8_t* src, int n)
{
  int i;
  uint8_t tmp;
  
  for (i = 0; i < n; i++) {
    tmp = src[2];
    dest[1] = src[1];
    dest[2] = src[0];
    dest[0] = tmp;
    dest += 3;
    src += 3;
  }
}

OIL_DEFINE_IMPL_REF (rgb2bgr_ref, rgb2bgr);


static void
rgb2rgba_ref (uint8_t *dest, const uint8_t* src, int n)
{
  int i;
  
  for (i = 0; i < n; i++) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = 0xFF;
    dest += 4;
    src += 3;
  }
}

OIL_DEFINE_IMPL_REF (rgb2rgba_ref, rgb2rgba);


void
rgb565_to_argb_ref (uint32_t *d, uint16_t* s, int n)
{
  int i;
  int r,g,b;

  for (i = 0; i < n; i++) {
    r = (s[i] >> 8) & 0xf8;
    g = (s[i] >> 3) & 0xfc;
    b = (s[i] << 3) & 0xf8;
    d[i] = oil_argb(255, r | (r>>5), g | (g>>6), b | (b>>5));
  }
}
OIL_DEFINE_IMPL_REF (rgb565_to_argb_ref, rgb565_to_argb);

