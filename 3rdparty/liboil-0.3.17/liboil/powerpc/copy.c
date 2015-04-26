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

OIL_DECLARE_CLASS(copy_u8);

static void
copy_u8_altivec (uint8_t *dest, uint8_t *src, int n)
{
  while(((unsigned long)dest & 0xf) && n>0) {
    *dest++ = *src++;
    n--;
  }

  if (n/16) {
    if ((unsigned long)src & 0xf) {
      asm volatile (
          "  mtctr %2\n"
          "  li r11, 0\n"
          "  lvsl v2, 0, %1\n"
          "  lvx v0, 0, %1\n"
          "  addi %1, %1, 16\n"
          "1:\n"
          "  lvx v1, r11, %1\n"
          "  vperm v0, v0, v1, v2\n"
          "  stvx v0, r11, %0\n"
          "  vor v0, v1, v1\n"
          "  addi r11, r11, 16\n"
          "  bdnz+ 1b\n"
          "  add %0, %0, r11\n"
          "  add %1, %1, r11\n"
          "  addi %1, %1, -16\n"
        : "+b" (dest), "+b" (src)
        : "r" (n/16));
    } else {
      asm volatile (
          "  mtctr %2\n"
          "  li r11, 0\n"
          "1:\n"
          "  lvx v1, r11, %1\n"
          "  stvx v1, r11, %0\n"
          "  addi r11, r11, 16\n"
          "  bdnz+ 1b\n"
          "  add %0, %0, r11\n"
          "  add %1, %1, r11\n"
        : "+b" (dest), "+b" (src)
        : "r" (n/16));
    }
  }

  n &= 0xf;
  while(n) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_altivec, copy_u8, OIL_IMPL_FLAG_ALTIVEC);

static void
copy_u8_altivec2 (uint8_t *dest, uint8_t *src, int n)
{
  while((unsigned long)dest & 0xf && n>0) {
    *dest++ = *src++;
    n--;
  }

  if (n/64) {
    asm volatile (
        "  mtctr %2\n"
        "  li r11, 0\n"
        "  lvsl v5, 0, %1\n"
        "  lvx v0, 0, %1\n"
        "  addi %1, %1, 16\n"
        "1:\n"
        "  lvx v1, r11, %1\n"
        "  addi r11, r11, 16\n"
        "  lvx v2, r11, %1\n"
        "  addi r11, r11, 16\n"
        "  lvx v3, r11, %1\n"
        "  addi r11, r11, 16\n"
        "  lvx v4, r11, %1\n"
        "  addi r11, r11, -48\n"

        "  vperm v0, v0, v1, v5\n"
        "  vperm v1, v1, v2, v5\n"
        "  vperm v2, v2, v3, v5\n"
        "  vperm v3, v3, v4, v5\n"

        "  stvx v0, r11, %0\n"
        "  addi r11, r11, 16\n"
        "  stvx v1, r11, %0\n"
        "  addi r11, r11, 16\n"
        "  stvx v2, r11, %0\n"
        "  addi r11, r11, 16\n"
        "  stvx v3, r11, %0\n"
        "  addi r11, r11, 16\n"
        "  vor v0, v4, v4\n"

        "  bdnz+ 1b\n"
        "  add %0, %0, r11\n"
        "  add %1, %1, r11\n"
        "  addi %1, %1, -16\n"
      : "+b" (dest), "+b" (src)
      : "r" (n/64));
  }

  n &= 0x3f;
  while(n) {
    *dest++ = *src++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_altivec2, copy_u8, OIL_IMPL_FLAG_ALTIVEC);


