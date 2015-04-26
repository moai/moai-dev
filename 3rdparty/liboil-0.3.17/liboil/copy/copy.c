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

OIL_DECLARE_CLASS (copy_u8);

static void
copy_u8_libc (uint8_t *dest, const uint8_t *src, int n)
{
  memcpy (dest, src, n);
}
OIL_DEFINE_IMPL (copy_u8_libc, copy_u8);

static void
copy_u8_ptr (uint8_t *dest, const uint8_t *src, int n)
{
  while(n--) {
    *dest++ = *src++;
  }
}
OIL_DEFINE_IMPL (copy_u8_ptr, copy_u8);

#ifdef HAVE_UNALIGNED_ACCESS
static void
copy_u8_ints (uint8_t *dest, const uint8_t *src, int n)
{
  int i;
  for(i=0;i<(n&3);i++){
    *dest++ = *src++;
  }
  n >>= 2;
  for(i=0;i<n;i++){
    *(uint32_t *)dest = *(uint32_t *)src;
    dest += 4;
    src += 4;
  }
}
OIL_DEFINE_IMPL (copy_u8_ints, copy_u8);
#endif

#ifdef HAVE_UNALIGNED_ACCESS
/* Submitted by Adam Moss */
static void
copy_u8_llints (uint8_t *dest, const uint8_t *src, int n)
{
  int i;
  for(i=0;i<(n&7);i++){
    *dest++ = *src++;
  }
  n >>= 3;
  for(i=0;i<n;i++){
    *(uint64_t *)dest = *(uint64_t *)src;
    dest += 8;
    src += 8;
  }
}
OIL_DEFINE_IMPL (copy_u8_llints, copy_u8);
#endif

#ifdef HAVE_UNALIGNED_ACCESS
/* Submitted by Adam Moss */
static void
copy_u8_llints_duff (uint8_t *dest, const uint8_t *src, int n)
{
  switch(n&7) {
  case 7: *dest++ = *src++;
  case 6: *dest++ = *src++;
  case 5: *dest++ = *src++;
  case 4: *dest++ = *src++;
  case 3: *dest++ = *src++;
  case 2: *dest++ = *src++;
  case 1: *dest++ = *src++;
  default: ;
  }
  n >>= 3;
  while (n) {
    switch (n & 15) {
    default:
    case 0: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 15: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 14: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 13: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 12: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 11: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 10: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 9: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 8: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 7: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 6: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 5: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 4: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 3: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 2: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    case 1: *(uint64_t *)dest = *(uint64_t *)src; dest += 8; src += 8;
    }
    n = (n - 1) & ~(int)15;
  }
}
OIL_DEFINE_IMPL (copy_u8_llints_duff, copy_u8);
#endif

