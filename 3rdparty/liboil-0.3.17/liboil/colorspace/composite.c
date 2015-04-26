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
#include <liboil/liboildebug.h>

#define COMPOSITE_OVER(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))
#define COMPOSITE_ADD(d,s) oil_clamp_255((d) + (s))
#define COMPOSITE_IN(s,m) oil_muldiv_255((s),(m))

OIL_DECLARE_CLASS (composite_over_argb);

static void
composite_over_argb_noclamp (uint32_t *dest, const uint32_t *src, int n)
{
  int i;
  uint8_t a;

  for(i=0;i<n;i++){
    a = oil_argb_A(src[i]);
    dest[i] = oil_argb_noclamp(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(src[i]),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(src[i]),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(src[i]),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(src[i]),a));
  }

}
OIL_DEFINE_IMPL (composite_over_argb_noclamp, composite_over_argb);

#define oil_divide_255_2(x)  ((((x)+128)*257)>>24)
#define oil_muldiv_255_2(a,b) oil_divide_255_2((a)*(b))
#define COMPOSITE_OVER_2(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))

static void
composite_over_argb_noclamp_2 (uint32_t *dest, const uint32_t *src, int n)
{
  int i;
  uint8_t a;

  for(i=0;i<n;i++){
    a = oil_argb_A(src[i]);
    dest[i] = oil_argb_noclamp(
        COMPOSITE_OVER_2(oil_argb_A(dest[i]),oil_argb_A(src[i]),a),
        COMPOSITE_OVER_2(oil_argb_R(dest[i]),oil_argb_R(src[i]),a),
        COMPOSITE_OVER_2(oil_argb_G(dest[i]),oil_argb_G(src[i]),a),
        COMPOSITE_OVER_2(oil_argb_B(dest[i]),oil_argb_B(src[i]),a));
  }

}
OIL_DEFINE_IMPL (composite_over_argb_noclamp_2, composite_over_argb);

