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


#define MULTSUM_DEFINE_REF(type)	\
static void multsum_ ## type ## _ref(	\
    oil_type_ ## type *dest,		\
    oil_type_ ## type *src1, int sstr1,	\
    oil_type_ ## type *src2, int sstr2,	\
    int n)				\
{					\
  int i;				\
  double sum = 0;			\
  double errsum = 0;			\
  for(i=0;i<n;i++){			\
    oil_type_ ## type x;                    \
    oil_type_ ## type tmp;                  \
    x = OIL_GET(src1,sstr1*i,oil_type_ ## type) * OIL_GET(src2,sstr2*i,oil_type_ ## type);		\
    tmp = sum;				\
    sum += x;				\
    errsum += (tmp - sum) + x;		\
  }					\
  *dest = sum + errsum;			\
}					\
OIL_DEFINE_IMPL_REF (multsum_ ## type ## _ref, multsum_ ## type); \
OIL_DEFINE_CLASS (multsum_ ## type, \
    "oil_type_" #type " *dest, "		\
    "oil_type_" #type " *src1, int sstr1, "	\
    "oil_type_" #type " *src2, int sstr2, "	\
    "int n")

/**
 * oil_multsum_f32:
 * @dest:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Multiplies each element in @src1 and @src2 and sums the results
 * over the entire array, and places the sum into @dest.
 */
MULTSUM_DEFINE_REF(f32);
/**
 * oil_multsum_f64:
 * @dest:
 * @src1:
 * @sstr1:
 * @src2:
 * @sstr2:
 * @n:
 *
 * Multiplies each element in @src1 and @src2 and sums the results
 * over the entire array, and places the sum into @dest.
 */
MULTSUM_DEFINE_REF(f64);

