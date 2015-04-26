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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>


static void
swab_u16_libc (uint16_t *d, const uint16_t *s, int n)
{
  swab ((void *)s, (void *)d, n*2);
}
OIL_DEFINE_IMPL (swab_u16_libc, swab_u16);

static void
swab_u16_ptr (uint16_t *d, const uint16_t *s, int n)
{
  int i;

  for(i=0;i<n;i++){
    *d++ = (*s<<8) | (*s>>8);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u16_ptr, swab_u16);

static void
swab_u32_ptr (uint32_t *d, const uint32_t *s, int n)
{
  int i;

  for(i=0;i<n;i++){
    *d++ = (*s<<24) | ((*s&0x0000ff00)<<8) |
      ((*s&0x00ff0000)>>8) | (*s>>24);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u32_ptr, swab_u32);

static void
swab_u16_unroll2 (uint16_t *d, const uint16_t *s, int n)
{
  int i;

  if (n&1) {
    *d++ = (*s<<8) | (*s>>8);
    s++;
  }
  n>>=1;
  for(i=0;i<n;i++){
    *d++ = (*s<<8) | (*s>>8);
    s++;
    *d++ = (*s<<8) | (*s>>8);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u16_unroll2, swab_u16);

static void
swab_u16_unroll4 (uint16_t *d, const uint16_t *s, int n)
{
  int i;

  while (n&3) {
    *d++ = (*s<<8) | (*s>>8);
    s++;
    n--;
  }
  n>>=2;
  for(i=0;i<n;i++){
    *d++ = (*s<<8) | (*s>>8);
    s++;
    *d++ = (*s<<8) | (*s>>8);
    s++;
    *d++ = (*s<<8) | (*s>>8);
    s++;
    *d++ = (*s<<8) | (*s>>8);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u16_unroll4, swab_u16);

static void
swab_u32_unroll2 (uint32_t *d, const uint32_t *s, int n)
{
  int i;

  if (n&1) {
    *d++ = (*s<<24) | ((*s&0x0000ff00)<<8) |
      ((*s&0x00ff0000)>>8) | (*s>>24);
    s++;
  }
  n>>=1;
  for(i=0;i<n;i++){
    *d++ = (*s<<24) | ((*s&0x0000ff00)<<8) |
      ((*s&0x00ff0000)>>8) | (*s>>24);
    s++;
    *d++ = (*s<<24) | ((*s&0x0000ff00)<<8) |
      ((*s&0x00ff0000)>>8) | (*s>>24);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u32_unroll2, swab_u32);


static void
swab_u16_char (uint16_t *d, const uint16_t *s, int n)
{
  int i;
  uint8_t *bd = (uint8_t *)d;
  uint8_t *bs = (uint8_t *)s;
  int x;

  for(i=0;i<n;i++){
    x = bs[0];
    bd[0] = bs[1];
    bd[1] = x;
    bd += 2;
    bs += 2;
  }
}
OIL_DEFINE_IMPL (swab_u16_char, swab_u16);

static void
swab_u32_char (uint32_t *d, const uint32_t *s, int n)
{
  int i;

  for(i=0;i<n;i++){
    *d++ = (*s<<24) | ((*s&0x0000ff00)<<8) |
      ((*s&0x00ff0000)>>8) | (*s>>24);
    s++;
  }
}
OIL_DEFINE_IMPL (swab_u32_char, swab_u32);







#if 0
static void
swab_u32_asm (uint32_t *d, uint32_t *s, int n)
{
  asm volatile ("\n"
      "1:\n"
      "  mov 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  mov %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "  jne 1b\n"
      : "+r" (d), "+r" (s), "+r" (n)
      :
      : "eax");
}
OIL_DEFINE_IMPL (swab_u32_asm, swab_u32);
#endif

