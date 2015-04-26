/*
 * Copyright (c) 2005
 *	Eric Anholt.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>

#define CLAMP_DEFINE_REF(type) \
static void clamp_ ## type ## _ref ( \
    oil_type_ ## type *dest, \
    const oil_type_ ## type *src, \
    int n, \
    const oil_type_ ## type *min, \
    const oil_type_ ## type *max) \
{ \
  int i; \
  oil_type_ ## type x; \
  for (i = 0; i < n; i++) { \
    x = src[i]; \
    if (x < *min) \
      x = *min; \
    if (x > *max) \
      x = *max; \
    dest[i] = x; \
  } \
} \
static void clamp_ ## type ## _test (OilTest *test) \
{ \
  oil_type_ ## type *lo = (oil_type_ ## type *) \
      (test->params[OIL_ARG_SRC2].src_data + \
       test->params[OIL_ARG_SRC2].test_header); \
  oil_type_ ## type *hi = (oil_type_ ## type *) \
      (test->params[OIL_ARG_SRC3].src_data + \
       test->params[OIL_ARG_SRC3].test_header); \
  if (*lo > *hi) { \
    oil_type_ ## type tmp; \
    tmp = *lo; \
    *lo = *hi; \
    *hi = tmp; \
  } \
} \
OIL_DEFINE_CLASS_FULL(clamp_ ## type, \
    "oil_type_" #type " *dest, " \
    "oil_type_" #type " *src, " \
    "int n, " \
    "oil_type_" #type " *s2_1, " \
    "oil_type_" #type " *s3_1", \
    clamp_ ## type ## _test); \
OIL_DEFINE_IMPL_REF(clamp_ ## type ## _ref, clamp_ ## type)

CLAMP_DEFINE_REF (s8);
CLAMP_DEFINE_REF (u8);
CLAMP_DEFINE_REF (s16);
CLAMP_DEFINE_REF (u16);
CLAMP_DEFINE_REF (s32);
CLAMP_DEFINE_REF (u32);
CLAMP_DEFINE_REF (f32);
CLAMP_DEFINE_REF (f64);

/**
 * oil_clamp_s8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_u8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_s16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_u16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_s32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_u32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_f32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

/**
 * oil_clamp_f64:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 * @s3_1:
 *
 * Clamps each value in @src to the range [@s2_1,@s3_1] and places
 * the result in @dest.
 */

#define CLAMPLOW_DEFINE_REF(type) \
static void clamplow_ ## type ## _ref ( \
    oil_type_ ## type *dest, \
    const oil_type_ ## type *src, \
    int n, \
    const oil_type_ ## type *min) \
{ \
  int i; \
  oil_type_ ## type x; \
  for (i = 0; i < n; i++) { \
    x = src[i]; \
    if (x < *min) \
      x = *min; \
    dest[i] = x; \
  } \
} \
OIL_DEFINE_CLASS(clamplow_ ## type, \
    "oil_type_" #type " *dest, " \
    "oil_type_" #type " *src, " \
    "int n, " \
    "oil_type_" #type " *s2_1"); \
OIL_DEFINE_IMPL_REF(clamplow_ ## type ## _ref, clamplow_ ## type)

CLAMPLOW_DEFINE_REF (s8);
CLAMPLOW_DEFINE_REF (u8);
CLAMPLOW_DEFINE_REF (s16);
CLAMPLOW_DEFINE_REF (u16);
CLAMPLOW_DEFINE_REF (s32);
CLAMPLOW_DEFINE_REF (u32);
CLAMPLOW_DEFINE_REF (f32);
CLAMPLOW_DEFINE_REF (f64);

/**
 * oil_clamplow_s8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */


/**
 * oil_clamplow_u8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamplow_s16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */


/**
 * oil_clamplow_u16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamplow_s32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */


/**
 * oil_clamplow_u32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamplow_f32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */


/**
 * oil_clamplow_f64:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to a lower bound of @s2_1 and places the result in
 * @dest.
 */

#define CLAMPHIGH_DEFINE_REF(type) \
static void clamphigh_ ## type ## _ref ( \
    oil_type_ ## type *dest, \
    const oil_type_ ## type *src, \
    int n, \
    const oil_type_ ## type *max) \
{ \
  int i; \
  oil_type_ ## type x; \
  for (i = 0; i < n; i++) { \
    x = src[i]; \
    if (x > *max) \
      x = *max; \
    dest[i] = x; \
  } \
} \
OIL_DEFINE_CLASS(clamphigh_ ## type, \
    "oil_type_" #type " *dest, " \
    "oil_type_" #type " *src, " \
    "int n, " \
    "oil_type_" #type " *s2_1"); \
OIL_DEFINE_IMPL_REF(clamphigh_ ## type ## _ref, clamphigh_ ## type)

CLAMPHIGH_DEFINE_REF (s8);
CLAMPHIGH_DEFINE_REF (u8);
CLAMPHIGH_DEFINE_REF (s16);
CLAMPHIGH_DEFINE_REF (u16);
CLAMPHIGH_DEFINE_REF (s32);
CLAMPHIGH_DEFINE_REF (u32);
CLAMPHIGH_DEFINE_REF (f32);
CLAMPHIGH_DEFINE_REF (f64);

/**
 * oil_clamphigh_s8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_u8:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_s16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_u16:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_s32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_u32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_f32:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */

/**
 * oil_clamphigh_f64:
 * @dest:
 * @src:
 * @n:
 * @s2_1:
 *
 * Clamps each value in @src to an upper bound of @s2_1 and places the result in
 * @dest.
 */
