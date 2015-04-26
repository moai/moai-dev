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

#include <liboil/liboil.h>
#include "liboil/utf8/utf8.h"


#ifdef HAVE_UNALIGNED_ACCESS
static void
utf8_validate_fast (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  int extra_bytes;
  int mask;

  i=0;
  while (i<n) {
    if (i < n-3 && (*(uint32_t *)(s+i) & 0x80808080) == 0) {
      i+=4;
      continue;
    }
    if (s[i] < 128) {
      i++;
      continue;
    }
    if ((s[i] & 0xe0) == 0xc0) {
      extra_bytes = 1;
      mask = 0x7f;
    } else if ((s[i] & 0xf0) == 0xe0) {
      extra_bytes = 2;
      mask = 0x1f;
    } else if ((s[i] & 0xf8) == 0xf0) {
      extra_bytes = 3;
      mask = 0x0f;
    } else {
      goto error;
    }
    if (i + extra_bytes >= n) goto error;
    while(extra_bytes--) {
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
    }
    i++;
  }

error:
  d_1[0] = i;
}
OIL_DEFINE_IMPL (utf8_validate_fast, utf8_validate);
#endif

static void
utf8_validate_fast2 (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  uint8_t x;

  i=0;
  while (i<n) {
    x = s[i];
    if (!(x & 0x80)) {
      i++;
      continue;
    }
    x <<= 1;
    if (!(x & 0x80)) {
      goto error;
    }
    x <<= 1;
    if (!(x & 0x80)) {
      if (i + 1 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    x <<= 1;
    if (!(x & 0x80)) {
      if (i + 2 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    x <<= 1;
    if (!(x & 0x80)) {
      if (i + 3 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    goto error;
  }

error:
  d_1[0] = i;
}
OIL_DEFINE_IMPL (utf8_validate_fast2, utf8_validate);

#ifdef HAVE_UNALIGNED_ACCESS
static void
utf8_validate_fast3 (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  uint8_t x;

  i=0;
  while (i<n) {
    if (i < n-3 && (*(uint32_t *)(s+i) & 0x80808080) == 0) {
      i+=4;
      continue;
    }
    x = s[i];
    if (!(x & 0x80)) {
      i++;
      continue;
    }
    if (!(x & 0x40)) {
      goto error;
    }
    if (!(x & 0x20)) {
      if (i + 1 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    if (!(x & 0x10)) {
      if (i + 2 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    if (!(x & 0x08)) {
      if (i + 3 >= n) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
      i++;
      continue;
    }
    goto error;
  }

error:
  d_1[0] = i;
}
OIL_DEFINE_IMPL (utf8_validate_fast3, utf8_validate);
#endif

static uint8_t utf8_table[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 8, 8, 8, 8, 8, 8
};
  
static void
utf8_validate_lookup (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  uint8_t x;

  i=0;
  while (i<n) {
    x = utf8_table[s[i]];
    if (x > 0) {
      if (x == 8 || i + x >= n) goto error;
      while (x>0) {
        i++;
        if ((s[i] & 0xc0) != 0x80) goto error;
        x--;
      }
    }
    i++;
  }

error:
  d_1[0] = i;
}
OIL_DEFINE_IMPL (utf8_validate_lookup, utf8_validate);

#if 0
static void
utf8_validate_asm1 (int32_t *d_1, uint8_t *s, int n)
{
  uint8_t *tmp = s;

  asm (
      "1:\n"
      "  movb (%%eax), %%bl\n"
      "  testb %%bl, %%bl\n"
      //"  jns 3f\n"
      "  js 2f\n"
      "3:\n"
      "  addl $1, %%eax\n"
      "  subl $1, %%ecx\n"
      "  jne 1b\n"
      "2:\n"
    : "+a" (tmp), "+c" (n)
    :
    : "ebx" );

  d_1[0] = tmp - s;
}
OIL_DEFINE_IMPL (utf8_validate_asm1, utf8_validate);

static void
utf8_validate_asm2 (int32_t *d_1, uint8_t *s, int n)
{
  uint8_t *tmp = s;

  asm (
      "1:\n"
      "  testl $0x80808080, (%%eax)\n"
      "  jne 2f\n"
      "  testl $0x80808080, 4(%%eax)\n"
      "  jne 2f\n"
      "  testl $0x80808080, 8(%%eax)\n"
      "  jne 2f\n"
      "  testl $0x80808080, 12(%%eax)\n"
      "  jne 2f\n"
      "  addl $16, %%eax\n"
      "  subl $16, %%ecx\n"
      "  jge 1b\n"
      "  jl 4f\n"
      "2:\n"
      "  movb (%%eax), %%bl\n"
      "  testb %%bl, %%bl\n"
      "  js 4f\n"
      "3:\n"
      "  addl $1, %%eax\n"
      "  subl $1, %%ecx\n"
      "  jne 1b\n"
      "4:\n"
    : "+a" (tmp), "+c" (n)
    :
    : "ebx" );

  d_1[0] = tmp - s;
}
OIL_DEFINE_IMPL (utf8_validate_asm2, utf8_validate);
#endif

