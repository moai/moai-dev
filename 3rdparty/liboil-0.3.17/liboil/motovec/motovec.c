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

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(copy_u8);
OIL_DECLARE_CLASS(splat_u8_ns);

#ifdef HAVE_SYMBOL_UNDERSCORE
void *vec_memcpy(void *dest, void *src, int n);
void *vec_memset(void *dest, int val, int n);
#else
void *_vec_memcpy(void *dest, void *src, int n);
void *_vec_memset(void *dest, int val, int n);
#define vec_memcpy _vec_memcpy
#define vec_memset _vec_memset
#endif

static void
copy_u8_motovec (uint8_t *dest, uint8_t *src, int n)
{
  vec_memcpy(dest, src, n);
}
OIL_DEFINE_IMPL_FULL (copy_u8_motovec, copy_u8, OIL_IMPL_FLAG_ALTIVEC);

static void
splat_u8_ns_motovec (uint8_t *dest, uint8_t *src, int n)
{
  vec_memset(dest, src[0], n);
}
OIL_DEFINE_IMPL_FULL (splat_u8_ns_motovec, splat_u8_ns, OIL_IMPL_FLAG_ALTIVEC);



