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
#include <liboil/liboiltest.h>

/**
 * SECTION:liboilfuncs-doc:
 * @title: Miscellaneous
 * @short_description: Miscellaneous functions
 */
#define CLIP_DEFINE_REF(type) \
static void clip_ ## type ## _ref ( \
    oil_type_ ## type *dest, \
    int dstr, \
    oil_type_ ## type *src, \
    int sstr, int n, \
    oil_type_ ## type *min, oil_type_ ## type *max) \
{ \
  int i; \
  oil_type_ ## type x; \
  for(i=0;i<n;i++){ \
    x = OIL_GET(src,i*sstr,oil_type_ ## type); \
    if (x<*min) x = *min; \
    if (x>*max) x = *max; \
    OIL_GET(dest,i*dstr,oil_type_ ## type) = x; \
  } \
} \
static void clip_ ## type ## _test (OilTest *test) \
{ \
  oil_type_ ## type *lo = (oil_type_ ## type *) \
    oil_test_get_source_data (test, OIL_ARG_SRC2); \
  oil_type_ ## type *hi = (oil_type_ ## type *) \
    oil_test_get_source_data (test, OIL_ARG_SRC3); \
  if (*lo > *hi) { \
    oil_type_ ## type tmp; \
    tmp = *lo; \
    *lo = *hi; \
    *hi = tmp; \
  } \
} \
OIL_DEFINE_CLASS_FULL(clip_ ## type, \
    "oil_type_" #type " *dest, " \
    "int dstr, " \
    "oil_type_" #type " *src, " \
    "int sstr, int n, " \
    "oil_type_" #type " *s2_1, oil_type_" #type " *s3_1", \
    clip_ ## type ## _test); \
OIL_DEFINE_IMPL_REF(clip_ ## type ## _ref, clip_ ## type)

CLIP_DEFINE_REF (s8);
CLIP_DEFINE_REF (u8);
CLIP_DEFINE_REF (s16);
CLIP_DEFINE_REF (u16);
CLIP_DEFINE_REF (s32);
CLIP_DEFINE_REF (u32);
CLIP_DEFINE_REF (f32);
CLIP_DEFINE_REF (f64);


/**
 * oil_clip_s8:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_u8:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_s16:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_u16:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_s32:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_u32:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_f32:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clip_f64:
 * @dest:
 * @dstr:
 * @src:
 * @sstr:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clips each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

