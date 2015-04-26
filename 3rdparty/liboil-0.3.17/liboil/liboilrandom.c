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

#include <liboil/liboilrandom.h>
#include <liboil/liboilcolorspace.h>
#include <stdlib.h>

/**
 * SECTION:liboilrandom
 * @title: Random Number Generation
 * @short_description: Random number generation
 */

static void
_oil_random_bits (void *dest, int n)
{
  int i;
  uint8_t *d = dest;
  for(i=0;i<n;i++){
    d[i] = (rand()>>16);
  }
}

/**
 * oil_random_s32:
 * @dest:
 * @n:
 *
 * Writes random values in the range [-(1<<31), (1<<31)-1] to the
 * destination array.
 */
void
oil_random_s32(oil_type_s32 *dest, int n)
{
  _oil_random_bits (dest, n*4);
}

/**
 * oil_random_s64:
 *
 * Writes random values in the range [-(1<<63), (1<<63)-1] to the
 * destination array.
 */
void
oil_random_s64 (oil_type_s64 *dest, int n)
{
  _oil_random_bits (dest, n*8);
}

/**
 * oil_random_s16:
 *
 * Writes random values in the range [-(1<<15), (1<<15)-1] to the
 * destination array.
 */
void
oil_random_s16 (oil_type_s16 *dest, int n)
{
  _oil_random_bits (dest, n*2);
}

/**
 * oil_random_s8:
 *
 * Writes random values in the range [-(1<<7), (1<<7)-1] to the
 * destination array.
 */
void
oil_random_s8 (oil_type_s8 *dest, int n)
{
  _oil_random_bits (dest, n);
}

/**
 * oil_random_u32:
 *
 * Writes random values in the range [0, (1<<32)-1] to the
 * destination array.
 */
void
oil_random_u32 (oil_type_u32 *dest, int n)
{
  _oil_random_bits (dest, n*4);
}

/**
 * oil_random_u64:
 *
 * Writes random values in the range [0, (1<<64)-1] to the
 * destination array.
 */
void
oil_random_u64 (oil_type_u64 *dest, int n)
{
  _oil_random_bits (dest, n*8);
}

/**
 * oil_random_u16:
 *
 * Writes random values in the range [0, (1<<16)-1] to the
 * destination array.
 */
void
oil_random_u16 (oil_type_u16 *dest, int n)
{
  _oil_random_bits (dest, n*2);
}

/**
 * oil_random_u8:
 *
 * Writes random values in the range [0, (1<<8)-1] to the
 * destination array.
 */
void
oil_random_u8 (oil_type_u8 *dest, int n)
{
  _oil_random_bits (dest, n);
}

/**
 * oil_random_f64:
 *
 * Writes random double-precision floating point values in the
 * range [0, 1.0) to the destination array.
 */
void
oil_random_f64 (oil_type_f64 *dest, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = (((rand()/(RAND_MAX+1.0))+rand())/(RAND_MAX+1.0));
  }
}

/**
 * oil_random_f32:
 *
 * Writes random single-precision floating point values in the
 * range [0, 1.0) to the destination array.
 */
void
oil_random_f32 (oil_type_f32 *dest, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = (rand()/(RAND_MAX+1.0));
  }
}

/**
 * oil_random_alpha:
 *
 * Writes random values in the range [0, 255] to the destination
 * array suitable for alpha values.  This is similar to oil_random_u8(),
 * except the values 0 and 255 are strongly favored.
 */
void
oil_random_alpha(uint8_t *dest, int n)
{
  int i;
  int x;
  for(i=0;i<n;i++){
    x = ((rand()>>8) & 0x1ff) - 0x80;
    if (x<0) x = 0;
    if (x>255) x = 255;
    dest[i] = x;
  }
}

/**
 * oil_random_argb:
 * @dest: destination array.
 * @n: number of values to write.
 *
 * Creates valid random RGBA values and places them in the destination
 * array.
 */
void
oil_random_argb(uint32_t *dest, int n)
{
  int i;
  int x;
  for(i=0;i<n;i++){
    x = ((rand()>>8) & 0x1ff) - 0x80;
    if (x<0) x = 0;
    if (x>255) x = 255;
    dest[i] = oil_argb_noclamp(x,
        oil_muldiv_255(x,oil_rand_u8()),
        oil_muldiv_255(x,oil_rand_u8()),
        oil_muldiv_255(x,oil_rand_u8()));
  }

}


