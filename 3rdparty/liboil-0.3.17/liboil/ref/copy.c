/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
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

#include <string.h>

#include <liboil/liboilfunction.h>

/**
 * SECTION:liboilfuncs-copy:
 * @title: Copying
 * @short_description: Functions for copying data
 *
 */

/**
 * oil_copy_u8:
 * @dest: destination array
 * @src: source array
 * @n: number of elements
 *
 * Copies from source to destination.
 */
OIL_DEFINE_CLASS (copy_u8, "uint8_t *dest, uint8_t *src, int n");

static void
copy_u8_ref (uint8_t *dest, uint8_t *src, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = src[i];
  }
}
OIL_DEFINE_IMPL_REF (copy_u8_ref, copy_u8);

/**
 * oil_compare_u8:
 * @d_1: destination array
 * @s1: source array
 * @s2: source array
 * @n: number of elements
 *
 * Compares two arrays.  The index of the first two elements that are
 * unequal is written into dest.  If all elements are equal, @n is
 * written into dest.
 */
OIL_DEFINE_CLASS (compare_u8, "uint32_t *d_1, uint8_t *s1, uint8_t *s2, int n");

static void
compare_u8_ref (uint32_t *dest, uint8_t *src1, uint8_t *src2, int n)
{
  int i;

  for(i=0;i<n;i++){
    if (src1[i] != src2[i]) {
      dest[0] = i;
      return;
    }
  }
  dest[0] = n;
}
OIL_DEFINE_IMPL_REF (compare_u8_ref, compare_u8);

/**
 * oil_testzero_u8:
 * @d_1: destination array
 * @s: source array
 * @n: number of elements
 *
 * Tests each element in the source array for equality with 0.  The
 * index of the first zero element is written into dest.  If all
 * elements are non-zero, @n is written into dest.
 *
 * This function is roughly equivalent to strnlen().  One notable
 * difference is that implementations of this function may legally
 * read past the zero byte.
 */
OIL_DEFINE_CLASS (testzero_u8, "uint32_t *d_1, uint8_t *s, int n");

static void
testzero_u8_ref (uint32_t *dest, uint8_t *src1, int n)
{
  int i;

  for(i=0;i<n;i++){
    if (src1[i] == 0) {
      dest[0] = i;
      return;
    }
  }
  dest[0] = n;
}
OIL_DEFINE_IMPL_REF (testzero_u8_ref, testzero_u8);

