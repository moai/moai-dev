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

#include <math.h>

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>

/**
 * SECTION:liboilfuncs-math
 * @title: Simple Arithmetic
 * @short_description: Aritmetic operations
 *
 */

/**
 * SECTION:liboilfuncs-math8x8
 * @title: Simple Arithmetic on Blocks
 * @short_description: Aritmetic operations on 8x8 blocks
 *
 */

/**
 * oil_add_s16:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Adds elements in @s2 and @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (add_s16, "int16_t *d, int16_t *src1, int16_t *src2, int n");
/**
 * oil_subtract_s16:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Subtracts elements in @s2 from @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (subtract_s16, "int16_t *d, int16_t *src1, int16_t *src2, int n");
/**
 * oil_add_s16_u8:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Adds elements in @s2 and @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (add_s16_u8, "int16_t *d, int16_t *src1, uint8_t *src2, int n");
/**
 * oil_subtract_s16_u8:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Subtracts elements in @s2 from @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (subtract_s16_u8, "int16_t *d, int16_t *src1, uint8_t *src2, int n");

/**
 * oil_add_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Adds elements in @s2 and @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (add_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_add_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Adds elements in @s2 and @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (add_f64, "double *d, double *s1, double *s2, int n");
/**
 * oil_subtract_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Subtracts elements in @s2 from @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (subtract_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_subtract_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Subtracts elements in @s2 from @s1 and places the result in @d.
 */
OIL_DEFINE_CLASS (subtract_f64, "double *d, double *s1, double *s2, int n");
/**
 * oil_multiply_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Multiplies elements in @s1 and @s2 and places the result in @d.
 */
OIL_DEFINE_CLASS (multiply_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_multiply_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Multiplies elements in @s1 and @s2 and places the result in @d.
 */
OIL_DEFINE_CLASS (multiply_f64, "double *d, double *s1, double *s2, int n");
/**
 * oil_divide_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Divides elements in @s1 by @s2 and places the result in @d.
 */
OIL_DEFINE_CLASS (divide_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_divide_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Divides elements in @s1 by @s2 and places the result in @d.
 */
OIL_DEFINE_CLASS (divide_f64, "double *d, double *s1, double *s2, int n");
/**
 * oil_minimum_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Places the lesser of @s1 and @s2 in @d.
 */
OIL_DEFINE_CLASS (minimum_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_minimum_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Places the lesser of @s1 and @s2 in @d.
 */
OIL_DEFINE_CLASS (minimum_f64, "float *d, float *s1, float *s2, int n");
/**
 * oil_maximum_f32:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Places the greater of @s1 and @s2 in @d.
 */
OIL_DEFINE_CLASS (maximum_f32, "float *d, float *s1, float *s2, int n");
/**
 * oil_maximum_f64:
 * @d: destination
 * @s1: source 1
 * @s2: source 2
 * @n: number of elements
 *
 * Places the greater of @s1 and @s2 in @d.
 */
OIL_DEFINE_CLASS (maximum_f64, "float *d, float *s1, float *s2, int n");

/**
 * oil_negative_f32:
 * @d: destination
 * @s: source
 * @n: number of elements
 *
 * Negates each element in @s and places the result in @d.
 */
OIL_DEFINE_CLASS (negative_f32, "float *d, float *s, int n");
/**
 * oil_inverse_f32:
 * @d: destination
 * @s: source
 * @n: number of elements
 *
 * Calculates the multiplicative inverse of each element in @s and
 * places the result in @d.
 */
OIL_DEFINE_CLASS (inverse_f32, "float *d, float *s, int n");
/**
 * oil_sign_f32:
 * @d: destination
 * @s: source
 * @n: number of elements
 *
 * Calculates the sign of each element in @s and
 * places the result in @d.
 */
OIL_DEFINE_CLASS (sign_f32, "float *d, float *s, int n");
/**
 * oil_floor_f32:
 * @d: destination
 * @s: source
 * @n: number of elements
 *
 * Calculates the greatest integer less than or equal to each element
 * in @s and places the result in @d.
 */
OIL_DEFINE_CLASS (floor_f32, "float *d, float *s, int n");

/**
 * oil_scalaradd_f32_ns:
 * @d: destination
 * @s1: source
 * @s2_1: source
 * @n: number of elements
 *
 * Adds the constant value @s2_1 to each source element and places
 * the result in @d.
 */
OIL_DEFINE_CLASS (scalaradd_f32_ns, "float *d, float *s1, float *s2_1, int n");
/**
 * oil_scalarmultiply_f32_ns:
 * @d: destination
 * @s1: source
 * @s2_1: source
 * @n: number of elements
 *
 * Multiplies the constant value @s2_1 and each source element and places
 * the result in @d.
 */
OIL_DEFINE_CLASS (scalarmultiply_f32_ns, "float *d, float *s1, float *s2_1, int n");

/**
 * oil_scalarmultiply_f64_ns:
 * @d: destination
 * @s1: source
 * @s2_1: source
 * @n: number of elements
 *
 * Multiplies the constant value @s2_1 and each source element and places
 * the result in @d.
 */
OIL_DEFINE_CLASS (scalarmultiply_f64_ns, "double *d, double *s1, double *s2_1, int n");

static void
add_s16_ref (int16_t *d, int16_t *src1, int16_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL_REF (add_s16_ref, add_s16);

static void
subtract_s16_ref (int16_t *d, int16_t *src1, int16_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] - src2[i];
  }
}
OIL_DEFINE_IMPL_REF (subtract_s16_ref, subtract_s16);

static void
add_s16_u8_ref (int16_t *d, int16_t *src1, uint8_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL_REF (add_s16_u8_ref, add_s16_u8);

static void
subtract_s16_u8_ref (int16_t *d, int16_t *src1, uint8_t *src2, int n)
{
  int i;
  for(i=0;i<n;i++){
    d[i] = src1[i] - src2[i];
  }
}
OIL_DEFINE_IMPL_REF (subtract_s16_u8_ref, subtract_s16_u8);

static void
add_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL_REF (add_f32_ref, add_f32);

static void
add_f64_ref (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL_REF (add_f64_ref, add_f64);

static void
subtract_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] - src2[i];
  }
}
OIL_DEFINE_IMPL_REF (subtract_f32_ref, subtract_f32);

static void
subtract_f64_ref (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] - src2[i];
  }
}
OIL_DEFINE_IMPL_REF (subtract_f64_ref, subtract_f64);

static void
multiply_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] * src2[i];
  }
}
OIL_DEFINE_IMPL_REF (multiply_f32_ref, multiply_f32);

static void
multiply_f64_ref (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] * src2[i];
  }
}
OIL_DEFINE_IMPL_REF (multiply_f64_ref, multiply_f64);

static void
divide_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] / src2[i];
  }
}
OIL_DEFINE_IMPL_REF (divide_f32_ref, divide_f32);

static void
divide_f64_ref (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] / src2[i];
  }
}
OIL_DEFINE_IMPL_REF (divide_f64_ref, divide_f64);

static void
minimum_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = (src1[i] < src2[i]) ? src1[i] : src2[i];
  }
}
OIL_DEFINE_IMPL_REF (minimum_f32_ref, minimum_f32);

static void
maximum_f32_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = (src1[i] > src2[i]) ? src1[i] : src2[i];
  }
}
OIL_DEFINE_IMPL_REF (maximum_f32_ref, maximum_f32);

static void
minimum_f64_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = (src1[i] < src2[i]) ? src1[i] : src2[i];
  }
}
OIL_DEFINE_IMPL_REF (minimum_f64_ref, minimum_f64);

static void
maximum_f64_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = (src1[i] > src2[i]) ? src1[i] : src2[i];
  }
}
OIL_DEFINE_IMPL_REF (maximum_f64_ref, maximum_f64);

static void
negative_f32_ref (float *dest, float *src1, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = -src1[i];
  }
}
OIL_DEFINE_IMPL_REF (negative_f32_ref, negative_f32);

static void
inverse_f32_ref (float *dest, float *src1, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = 1.0/src1[i];
  }
}
OIL_DEFINE_IMPL_REF (inverse_f32_ref, inverse_f32);

static void
sign_f32_ref (float *dest, float *src1, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = (src1[i] < 0) ? -src1[i] : src1[i];
  }
}
OIL_DEFINE_IMPL_REF (sign_f32_ref, sign_f32);

static void
floor_f32_ref (float *dest, float *src1, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = floor(src1[i]);
  }
}
OIL_DEFINE_IMPL_REF (floor_f32_ref, floor_f32);



static void
scalaradd_f32_ns_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] + src2[0];
  }
}
OIL_DEFINE_IMPL_REF (scalaradd_f32_ns_ref, scalaradd_f32_ns);

static void
scalarmultiply_f32_ns_ref (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] * src2[0];
  }
}
OIL_DEFINE_IMPL_REF (scalarmultiply_f32_ns_ref, scalarmultiply_f32_ns);

static void
scalarmultiply_f64_ns_ref (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = src1[i] * src2[0];
  }
}
OIL_DEFINE_IMPL_REF (scalarmultiply_f64_ns_ref, scalarmultiply_f64_ns);

