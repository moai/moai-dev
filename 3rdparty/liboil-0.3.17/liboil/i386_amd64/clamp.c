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
#include <liboil/liboilclasses.h>

static void
clamp_s8_asm_mmx (int8_t *dest, int8_t *src, int n, int8_t *low, int8_t *high)
{
  int32_t l;
  int32_t h;

  while(n&7) {
    oil_type_s8 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  l = *low + 128;
  l |= (l<<8);
  l |= (l<<16);
  h = 127 - *high;
  h |= (h<<8);
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklbw %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklbw %%mm5, %%mm5\n"
      "  mov $0x80808080, %3\n"
      "  movd %3, %%mm7\n"
      "  punpcklbw %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  psubusb %%mm6, %%mm0\n"
      "  paddusb %%mm6, %%mm0\n"
      "  paddusb %%mm5, %%mm0\n"
      "  psubusb %%mm5, %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_s8_asm_mmx, clamp_s8, OIL_IMPL_FLAG_MMX);

static void
clamp_s8_asm_mmxext (int8_t *dest, int8_t *src, int n, int8_t *low, int8_t *high)
{
  int32_t l;
  int32_t h;

  while(n&7) {
    oil_type_s8 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  l = *low + 128;
  l |= (l<<8);
  l |= (l<<16);
  h = *high + 128;
  h |= (h<<8);
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklbw %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklbw %%mm5, %%mm5\n"
      "  mov $0x80808080, %3\n"
      "  movd %3, %%mm7\n"
      "  punpcklbw %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  pmaxub %%mm6, %%mm0\n"
      "  pminub %%mm5, %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_s8_asm_mmxext, clamp_s8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
clamp_u8_asm_mmx (uint8_t *dest, uint8_t *src, int n, uint8_t *low, uint8_t *high)
{
  uint32_t l;
  uint32_t h;

  while(n&7) {
    oil_type_u8 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  l = *low;
  l |= (l<<8);
  l |= (l<<16);
  h = 255 - *high;
  h |= (h<<8);
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklbw %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklbw %%mm5, %%mm5\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  psubusb %%mm6, %%mm0\n"
      "  paddusb %%mm6, %%mm0\n"
      "  paddusb %%mm5, %%mm0\n"
      "  psubusb %%mm5, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_u8_asm_mmx, clamp_u8, OIL_IMPL_FLAG_MMX);

static void
clamp_u8_asm_mmxext (uint8_t *dest, uint8_t *src, int n, uint8_t *low, uint8_t *high)
{
  int32_t l;
  int32_t h;

  while(n&7) {
    oil_type_u8 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  l = *low;
  l |= (l<<8);
  l |= (l<<16);
  h = *high;
  h |= (h<<8);
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklbw %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklbw %%mm5, %%mm5\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pmaxub %%mm6, %%mm0\n"
      "  pminub %%mm5, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_u8_asm_mmxext, clamp_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);







static void
clamp_s16_asm_mmx (int16_t *dest, int16_t *src, int n, int16_t *low, int16_t *high)
{
  int32_t l;
  int32_t h;

  while(n&3) {
    oil_type_s16 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  l = *low + 32768;
  l |= (l<<16);
  h = 32767 - *high;
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklwd %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklwd %%mm5, %%mm5\n"
      "  mov $0x80008000, %3\n"
      "  movd %3, %%mm7\n"
      "  punpcklwd %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  psubusw %%mm6, %%mm0\n"
      "  paddusw %%mm6, %%mm0\n"
      "  paddusw %%mm5, %%mm0\n"
      "  psubusw %%mm5, %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_s16_asm_mmx, clamp_s16, OIL_IMPL_FLAG_MMX);

static void
clamp_s16_asm_mmxext (int16_t *dest, int16_t *src, int n, int16_t *low, int16_t *high)
{
  int32_t l;
  int32_t h;

  while(n&3) {
    oil_type_s16 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  l = *low & 0xffff;
  l |= (l<<16);
  h = *high & 0xffff;
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklwd %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklwd %%mm5, %%mm5\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pmaxsw %%mm6, %%mm0\n"
      "  pminsw %%mm5, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_s16_asm_mmxext, clamp_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
clamp_u16_asm_mmx (uint16_t *dest, uint16_t *src, int n, uint16_t *low, uint16_t *high)
{
  uint32_t l;
  uint32_t h;

  while(n&3) {
    oil_type_u16 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  l = *low;
  l |= (l<<16);
  h = 65535 - *high;
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklwd %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklwd %%mm5, %%mm5\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  psubusw %%mm6, %%mm0\n"
      "  paddusw %%mm6, %%mm0\n"
      "  paddusw %%mm5, %%mm0\n"
      "  psubusw %%mm5, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_u16_asm_mmx, clamp_u16, OIL_IMPL_FLAG_MMX);

static void
clamp_u16_asm_mmxext (uint16_t *dest, uint16_t *src, int n, uint16_t *low, uint16_t *high)
{
  int32_t l;
  int32_t h;

  while(n&3) {
    oil_type_u16 x = *src;
    if (x < *low) x = *low;
    if (x > *high) x = *high;
    *dest = x;
    dest++;
    src++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  l = *low ^ 0x8000;
  l |= (l<<16);
  h = *high ^ 0x8000;
  h |= (h<<16);
  asm volatile ("\n"
      "  movd %3, %%mm6\n"
      "  punpcklwd %%mm6, %%mm6\n"
      "  movd %4, %%mm5\n"
      "  punpcklwd %%mm5, %%mm5\n"
      "  mov $0x80008000, %3\n"
      "  movd %3, %%mm7\n"
      "  punpcklwd %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  pmaxsw %%mm6, %%mm0\n"
      "  pminsw %%mm5, %%mm0\n"
      "  pxor %%mm7, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n), "+r" (l), "+r" (h)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (clamp_u16_asm_mmxext, clamp_u16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

