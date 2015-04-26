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

#include <liboil/liboilfunction.h>
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(splat_u8_ns);
OIL_DECLARE_CLASS(splat_u32_ns);

static void
splat_u8_ns_altivec (uint8_t *dest, uint8_t *src, int n)
{
  while(((unsigned long)dest & 0xf) && n>0) {
    *dest++ = *src;
    n--;
  }

  if (n/16) {
    asm volatile (
          "  mtctr %2\n"
          "  li r11, 0\n"
          "  lvsl v2, 0, %1\n"
          "  lvx v0, 0, %1\n"
          "  vperm v0, v0, v0, v2\n"
          "  vspltb v0, v0, 0\n"
          "1:\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  bdnz+ 1b\n"
          "  add %0, %0, r11\n"
        : "+b" (dest), "+b" (src)
        : "r" (n/16));
  }

  n &= 0xf;
  while(n) {
    *dest++ = *src;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u8_ns_altivec, splat_u8_ns, OIL_IMPL_FLAG_ALTIVEC);

static void
splat_u8_ns_altivec2 (uint8_t *dest, uint8_t *src, int n)
{
  while(((unsigned long)dest & 0xf) && n>0) {
    *dest++ = *src;
    n--;
  }

  if (n/64) {
    asm volatile (
          "  mtctr %2\n"
          "  li r11, 0\n"
          "  lvsl v2, 0, %1\n"
          "  lvx v0, 0, %1\n"
          "  vperm v0, v0, v0, v2\n"
          "  vspltb v0, v0, 0\n"
          "1:\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  bdnz+ 1b\n"
          "  add %0, %0, r11\n"
        : "+b" (dest), "+b" (src)
        : "r" (n/64));
  }

  n &= 0x3f;
  while(n) {
    *dest++ = *src;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u8_ns_altivec2, splat_u8_ns, OIL_IMPL_FLAG_ALTIVEC);

/* With a little love, this could work with 4-byte unaligned dest,
 * but I'm not feeling loving today. */
static void
splat_u32_ns_altivec (uint32_t *dest, uint32_t *src, int n)
{
  while(((unsigned long)dest & 0xc) && n>0) {
    *dest++ = *src;
    n--;
  }

  if (n/4) {
    asm volatile (
          "  mtctr %2\n"
          "  li r11, 0\n"
          "  lvsl v2, 0, %1\n"
          "  lvx v0, 0, %1\n"
          "  vperm v0, v0, v0, v2\n"
          "  vspltw v0, v0, 0\n"
          "1:\n"
          "  stvx v0, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  bdnz+ 1b\n"
          "  add %0, %0, r11\n"
        : "+b" (dest), "+b" (src)
        : "r" (n/4));
  }

  n &= 0x3;
  while(n) {
    *dest++ = *src;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u32_ns_altivec, splat_u32_ns, OIL_IMPL_FLAG_ALTIVEC);

