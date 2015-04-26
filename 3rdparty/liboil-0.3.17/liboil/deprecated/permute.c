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
#include <liboil/simdpack/simdpack.h>
#include <math.h>

static void
permute_test (OilTest *test)
{
  int i;
  int n = test->n;
  int stride = oil_test_get_value (test, OIL_ARG_SSTR2);
  uint8_t *ptr = (uint8_t *) oil_test_get_source_data (test, OIL_ARG_SRC2);

  for(i=0;i<n;i++){
    /* FIXME */
    OIL_GET(ptr, i*stride, int32_t) = 0; /* oil_rand_s32(); */
  }

}

#define PERMUTE_DEFINE_REF(type)		\
static void permute_ ## type ## _ref(		\
    oil_type_ ## type *dest, int dstr,		\
    oil_type_ ## type *src1, int sstr1,		\
    int32_t *src2, int sstr2, int n)		\
{						\
  int i;					\
  for(i=0;i<n;i++){				\
    OIL_GET(dest,dstr*i, oil_type_ ## type) = OIL_GET(src1,sstr1*	\
	OIL_GET(src2,sstr2*i, int), oil_type_ ## type);		\
  }						\
}						\
OIL_DEFINE_IMPL_REF (permute_ ## type ## _ref, permute_ ## type); \
OIL_DEFINE_CLASS_FULL (permute_ ## type, "oil_type_" #type " *dest, int dstr, " \
    "oil_type_" #type " *src1, int sstr1, int32_t *src2, int sstr2, int n", \
    permute_test)

/**
 * oil_permute_s8:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (s8);
/**
 * oil_permute_u8:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (u8);
/**
 * oil_permute_s16:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (s16);
/**
 * oil_permute_u16:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (u16);
/**
 * oil_permute_s32:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (s32);
/**
 * oil_permute_u32:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (u32);
/**
 * oil_permute_f32:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (f32);
/**
 * oil_permute_f64:
 * @dest:
 * @dstr:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Copies elements in @src1 to @dest, permuting them by @src2.  That is,
 * @dest[i] is set to @src1[@src2[i]].  Values in @src2 must be
 * non-negative and less than @n.
 */
PERMUTE_DEFINE_REF (f64);

