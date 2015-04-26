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

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f,w,x,y,z,in,s) \
  (w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)

OIL_DECLARE_CLASS (md5);

#ifdef ENABLE_BROKEN_IMPLS

static void
md5_asm1 (uint32_t *state, uint32_t *src)
{
  uint32_t a,b,c,d;

  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];


  asm (
#define a "%0"
#define b "%1"
#define c "%2"
#define d "%3"
#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "  and r22, " r3 ", " r2 "\n" \
      "  andc r21, " r4 ", " r2 "\n" \
      "  or r22, r22, r21\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   li r20, " #offset "*4\n " \
      "   lwbrx r20, r20, %4\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP1 (a, b, c, d, 0, 0xd76aa478, 7)
      STEP1 (d, a, b, c, 1, 0xe8c7b756, 12)
      STEP1 (c, d, a, b, 2, 0x242070db, 17)
      STEP1 (b, c, d, a, 3, 0xc1bdceee, 22)
      STEP1 (a, b, c, d, 4, 0xf57c0faf, 7)
      STEP1 (d, a, b, c, 5, 0x4787c62a, 12)
      STEP1 (c, d, a, b, 6, 0xa8304613, 17)
      STEP1 (b, c, d, a, 7, 0xfd469501, 22)
      STEP1 (a, b, c, d, 8, 0x698098d8, 7)
      STEP1 (d, a, b, c, 9, 0x8b44f7af, 12)
      STEP1 (c, d, a, b, 10, 0xffff5bb1, 17)
      STEP1 (b, c, d, a, 11, 0x895cd7be, 22)
      STEP1 (a, b, c, d, 12, 0x6b901122, 7)
      STEP1 (d, a, b, c, 13, 0xfd987193, 12)
      STEP1 (c, d, a, b, 14, 0xa679438e, 17)
      STEP1 (b, c, d, a, 15, 0x49b40821, 22)
#undef STEP1
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "  and r22, " r2 ", " r4 "\n" \
      "  andc r21, " r3 ", " r4 "\n" \
      "  or r22, r22, r21\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   li r20, " #offset "*4\n " \
      "   lwbrx r20, r20, %4\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP2(a, b, c, d, 1, 0xf61e2562, 5)
      STEP2(d, a, b, c, 6, 0xc040b340, 9)
      STEP2(c, d, a, b, 11, 0x265e5a51, 14)
      STEP2(b, c, d, a, 0, 0xe9b6c7aa, 20)
      STEP2(a, b, c, d, 5, 0xd62f105d, 5)
      STEP2(d, a, b, c, 10, 0x02441453, 9)
      STEP2(c, d, a, b, 15, 0xd8a1e681, 14)
      STEP2(b, c, d, a, 4, 0xe7d3fbc8, 20)
      STEP2(a, b, c, d, 9, 0x21e1cde6, 5)
      STEP2(d, a, b, c, 14, 0xc33707d6, 9)
      STEP2(c, d, a, b, 3, 0xf4d50d87, 14)
      STEP2(b, c, d, a, 8, 0x455a14ed, 20)
      STEP2(a, b, c, d, 13, 0xa9e3e905, 5)
      STEP2(d, a, b, c, 2, 0xfcefa3f8, 9)
      STEP2(c, d, a, b, 7, 0x676f02d9, 14)
      STEP2(b, c, d, a, 12, 0x8d2a4c8a, 20)
#undef STEP2

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "  xor r22, " r2 ", " r3 "\n" \
      "  xor r22, r22, " r4 "\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   li r20, " #offset "*4\n " \
      "   lwbrx r20, r20, %4\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP3 (a, b, c, d, 5, 0xfffa3942, 4)
      STEP3 (d, a, b, c, 8, 0x8771f681, 11)
      STEP3 (c, d, a, b, 11, 0x6d9d6122, 16)
      STEP3 (b, c, d, a, 14, 0xfde5380c, 23)
      STEP3 (a, b, c, d, 1, 0xa4beea44, 4)
      STEP3 (d, a, b, c, 4, 0x4bdecfa9, 11)
      STEP3 (c, d, a, b, 7, 0xf6bb4b60, 16)
      STEP3 (b, c, d, a, 10, 0xbebfbc70, 23)
      STEP3 (a, b, c, d, 13, 0x289b7ec6, 4)
      STEP3 (d, a, b, c, 0, 0xeaa127fa, 11)
      STEP3 (c, d, a, b, 3, 0xd4ef3085, 16)
      STEP3 (b, c, d, a, 6, 0x04881d05, 23)
      STEP3 (a, b, c, d, 9, 0xd9d4d039, 4)
      STEP3 (d, a, b, c, 12, 0xe6db99e5, 11)
      STEP3 (c, d, a, b, 15, 0x1fa27cf8, 16)
      STEP3 (b, c, d, a, 2, 0xc4ac5665, 23)
#undef STEP3

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "  orc r22, " r2 ", " r4 "\n" \
      "  xor r22, r22, " r3 "\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   li r20, " #offset "*4\n " \
      "   lwbrx r20, r20, %4\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP4 (a, b, c, d, 0, 0xf4292244, 6)
      STEP4 (d, a, b, c, 7, 0x432aff97, 10)
      STEP4 (c, d, a, b, 14, 0xab9423a7, 15)
      STEP4 (b, c, d, a, 5, 0xfc93a039, 21)
      STEP4 (a, b, c, d, 12, 0x655b59c3, 6)
      STEP4 (d, a, b, c, 3, 0x8f0ccc92, 10)
      STEP4 (c, d, a, b, 10, 0xffeff47d, 15)
      STEP4 (b, c, d, a, 1, 0x85845dd1, 21)
      STEP4 (a, b, c, d, 8, 0x6fa87e4f, 6)
      STEP4 (d, a, b, c, 15, 0xfe2ce6e0, 10)
      STEP4 (c, d, a, b, 6, 0xa3014314, 15)
      STEP4 (b, c, d, a, 13, 0x4e0811a1, 21)
      STEP4 (a, b, c, d, 4, 0xf7537e82, 6)
      STEP4 (d, a, b, c, 11, 0xbd3af235, 10)
      STEP4 (c, d, a, b, 2, 0x2ad7d2bb, 15)
      STEP4 (b, c, d, a, 9, 0xeb86d391, 21)
#undef STEP4
#undef a
#undef b
#undef c
#undef d

      : "+b" (a), "+b" (b), "+b" (c), "+b" (d)
      : "b" (src)
      : "20", "21", "22");

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}


OIL_DEFINE_IMPL_ASM (md5_asm1, md5);

static void
md5_asm2 (uint32_t *state, uint32_t *src)
{
  uint32_t a,b,c,d;

  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];


  asm (
#define a "%0"
#define b "%1"
#define c "%2"
#define d "%3"
#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  and r22, " r3 ", " r2 "\n" \
      "  andc r21, " r4 ", " r2 "\n" \
      "  or r22, r22, r21\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP1 (a, b, c, d, 0, 0xd76aa478, 7)
      STEP1 (d, a, b, c, 1, 0xe8c7b756, 12)
      STEP1 (c, d, a, b, 2, 0x242070db, 17)
      STEP1 (b, c, d, a, 3, 0xc1bdceee, 22)
      STEP1 (a, b, c, d, 4, 0xf57c0faf, 7)
      STEP1 (d, a, b, c, 5, 0x4787c62a, 12)
      STEP1 (c, d, a, b, 6, 0xa8304613, 17)
      STEP1 (b, c, d, a, 7, 0xfd469501, 22)
      STEP1 (a, b, c, d, 8, 0x698098d8, 7)
      STEP1 (d, a, b, c, 9, 0x8b44f7af, 12)
      STEP1 (c, d, a, b, 10, 0xffff5bb1, 17)
      STEP1 (b, c, d, a, 11, 0x895cd7be, 22)
      STEP1 (a, b, c, d, 12, 0x6b901122, 7)
      STEP1 (d, a, b, c, 13, 0xfd987193, 12)
      STEP1 (c, d, a, b, 14, 0xa679438e, 17)
      STEP1 (b, c, d, a, 15, 0x49b40821, 22)
#undef STEP1
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  and r22, " r2 ", " r4 "\n" \
      "  andc r21, " r3 ", " r4 "\n" \
      "  or r22, r22, r21\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP2(a, b, c, d, 1, 0xf61e2562, 5)
      STEP2(d, a, b, c, 6, 0xc040b340, 9)
      STEP2(c, d, a, b, 11, 0x265e5a51, 14)
      STEP2(b, c, d, a, 0, 0xe9b6c7aa, 20)
      STEP2(a, b, c, d, 5, 0xd62f105d, 5)
      STEP2(d, a, b, c, 10, 0x02441453, 9)
      STEP2(c, d, a, b, 15, 0xd8a1e681, 14)
      STEP2(b, c, d, a, 4, 0xe7d3fbc8, 20)
      STEP2(a, b, c, d, 9, 0x21e1cde6, 5)
      STEP2(d, a, b, c, 14, 0xc33707d6, 9)
      STEP2(c, d, a, b, 3, 0xf4d50d87, 14)
      STEP2(b, c, d, a, 8, 0x455a14ed, 20)
      STEP2(a, b, c, d, 13, 0xa9e3e905, 5)
      STEP2(d, a, b, c, 2, 0xfcefa3f8, 9)
      STEP2(c, d, a, b, 7, 0x676f02d9, 14)
      STEP2(b, c, d, a, 12, 0x8d2a4c8a, 20)
#undef STEP2

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  xor r22, " r2 ", " r3 "\n" \
      "  xor r22, r22, " r4 "\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP3 (a, b, c, d, 5, 0xfffa3942, 4)
      STEP3 (d, a, b, c, 8, 0x8771f681, 11)
      STEP3 (c, d, a, b, 11, 0x6d9d6122, 16)
      STEP3 (b, c, d, a, 14, 0xfde5380c, 23)
      STEP3 (a, b, c, d, 1, 0xa4beea44, 4)
      STEP3 (d, a, b, c, 4, 0x4bdecfa9, 11)
      STEP3 (c, d, a, b, 7, 0xf6bb4b60, 16)
      STEP3 (b, c, d, a, 10, 0xbebfbc70, 23)
      STEP3 (a, b, c, d, 13, 0x289b7ec6, 4)
      STEP3 (d, a, b, c, 0, 0xeaa127fa, 11)
      STEP3 (c, d, a, b, 3, 0xd4ef3085, 16)
      STEP3 (b, c, d, a, 6, 0x04881d05, 23)
      STEP3 (a, b, c, d, 9, 0xd9d4d039, 4)
      STEP3 (d, a, b, c, 12, 0xe6db99e5, 11)
      STEP3 (c, d, a, b, 15, 0x1fa27cf8, 16)
      STEP3 (b, c, d, a, 2, 0xc4ac5665, 23)
#undef STEP3

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  orc r22, " r2 ", " r4 "\n" \
      "  xor r22, r22, " r3 "\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addis " r1 ", " r1 ", " #constant ">>16 + (" #constant ">>15)&1\n" \
      "  addi " r1 ", " r1 ", (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP4 (a, b, c, d, 0, 0xf4292244, 6)
      STEP4 (d, a, b, c, 7, 0x432aff97, 10)
      STEP4 (c, d, a, b, 14, 0xab9423a7, 15)
      STEP4 (b, c, d, a, 5, 0xfc93a039, 21)
      STEP4 (a, b, c, d, 12, 0x655b59c3, 6)
      STEP4 (d, a, b, c, 3, 0x8f0ccc92, 10)
      STEP4 (c, d, a, b, 10, 0xffeff47d, 15)
      STEP4 (b, c, d, a, 1, 0x85845dd1, 21)
      STEP4 (a, b, c, d, 8, 0x6fa87e4f, 6)
      STEP4 (d, a, b, c, 15, 0xfe2ce6e0, 10)
      STEP4 (c, d, a, b, 6, 0xa3014314, 15)
      STEP4 (b, c, d, a, 13, 0x4e0811a1, 21)
      STEP4 (a, b, c, d, 4, 0xf7537e82, 6)
      STEP4 (d, a, b, c, 11, 0xbd3af235, 10)
      STEP4 (c, d, a, b, 2, 0x2ad7d2bb, 15)
      STEP4 (b, c, d, a, 9, 0xeb86d391, 21)
#undef STEP4
#undef a
#undef b
#undef c
#undef d

      : "+b" (a), "+b" (b), "+b" (c), "+b" (d)
      : "b" (src)
      : "20", "21", "22");

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}


OIL_DEFINE_IMPL_ASM (md5_asm2, md5);

static void
md5_asm3 (uint32_t *state, uint32_t *src)
{
  uint32_t a,b,c,d;

  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];


  asm (
#define a "%0"
#define b "%1"
#define c "%2"
#define d "%3"
#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  and r22, " r3 ", " r2 "\n" \
      "  andc r21, " r4 ", " r2 "\n" \
      "  or r22, r22, r21\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "   lis r22, " #constant ">>16 + (" #constant ">>15)&1\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addi r22, r22, (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP1 (a, b, c, d, 0, 0xd76aa478, 7)
      STEP1 (d, a, b, c, 1, 0xe8c7b756, 12)
      STEP1 (c, d, a, b, 2, 0x242070db, 17)
      STEP1 (b, c, d, a, 3, 0xc1bdceee, 22)
      STEP1 (a, b, c, d, 4, 0xf57c0faf, 7)
      STEP1 (d, a, b, c, 5, 0x4787c62a, 12)
      STEP1 (c, d, a, b, 6, 0xa8304613, 17)
      STEP1 (b, c, d, a, 7, 0xfd469501, 22)
      STEP1 (a, b, c, d, 8, 0x698098d8, 7)
      STEP1 (d, a, b, c, 9, 0x8b44f7af, 12)
      STEP1 (c, d, a, b, 10, 0xffff5bb1, 17)
      STEP1 (b, c, d, a, 11, 0x895cd7be, 22)
      STEP1 (a, b, c, d, 12, 0x6b901122, 7)
      STEP1 (d, a, b, c, 13, 0xfd987193, 12)
      STEP1 (c, d, a, b, 14, 0xa679438e, 17)
      STEP1 (b, c, d, a, 15, 0x49b40821, 22)
#undef STEP1
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  and r22, " r2 ", " r4 "\n" \
      "  andc r21, " r3 ", " r4 "\n" \
      "  or r22, r22, r21\n" \
      "   lwbrx r20, r20, %4\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "  lis r22, " #constant ">>16 + (" #constant ">>15)&1\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addi r22, r22, (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP2(a, b, c, d, 1, 0xf61e2562, 5)
      STEP2(d, a, b, c, 6, 0xc040b340, 9)
      STEP2(c, d, a, b, 11, 0x265e5a51, 14)
      STEP2(b, c, d, a, 0, 0xe9b6c7aa, 20)
      STEP2(a, b, c, d, 5, 0xd62f105d, 5)
      STEP2(d, a, b, c, 10, 0x02441453, 9)
      STEP2(c, d, a, b, 15, 0xd8a1e681, 14)
      STEP2(b, c, d, a, 4, 0xe7d3fbc8, 20)
      STEP2(a, b, c, d, 9, 0x21e1cde6, 5)
      STEP2(d, a, b, c, 14, 0xc33707d6, 9)
      STEP2(c, d, a, b, 3, 0xf4d50d87, 14)
      STEP2(b, c, d, a, 8, 0x455a14ed, 20)
      STEP2(a, b, c, d, 13, 0xa9e3e905, 5)
      STEP2(d, a, b, c, 2, 0xfcefa3f8, 9)
      STEP2(c, d, a, b, 7, 0x676f02d9, 14)
      STEP2(b, c, d, a, 12, 0x8d2a4c8a, 20)
#undef STEP2

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  xor r22, " r2 ", " r3 "\n" \
      "   lwbrx r20, r20, %4\n" \
      "  xor r22, r22, " r4 "\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "  lis r21, " #constant ">>16 + (" #constant ">>15)&1\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addi r21, r21, (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "   add " r1 ", " r1 ", r21\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP3 (a, b, c, d, 5, 0xfffa3942, 4)
      STEP3 (d, a, b, c, 8, 0x8771f681, 11)
      STEP3 (c, d, a, b, 11, 0x6d9d6122, 16)
      STEP3 (b, c, d, a, 14, 0xfde5380c, 23)
      STEP3 (a, b, c, d, 1, 0xa4beea44, 4)
      STEP3 (d, a, b, c, 4, 0x4bdecfa9, 11)
      STEP3 (c, d, a, b, 7, 0xf6bb4b60, 16)
      STEP3 (b, c, d, a, 10, 0xbebfbc70, 23)
      STEP3 (a, b, c, d, 13, 0x289b7ec6, 4)
      STEP3 (d, a, b, c, 0, 0xeaa127fa, 11)
      STEP3 (c, d, a, b, 3, 0xd4ef3085, 16)
      STEP3 (b, c, d, a, 6, 0x04881d05, 23)
      STEP3 (a, b, c, d, 9, 0xd9d4d039, 4)
      STEP3 (d, a, b, c, 12, 0xe6db99e5, 11)
      STEP3 (c, d, a, b, 15, 0x1fa27cf8, 16)
      STEP3 (b, c, d, a, 2, 0xc4ac5665, 23)
#undef STEP3

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "   li r20, " #offset "*4\n " \
      "  orc r22, " r2 ", " r4 "\n" \
      "   lwbrx r20, r20, %4\n" \
      "  xor r22, r22, " r3 "\n" \
      "  add " r1 ", " r1 ", r22\n" \
      "  lis r21, " #constant ">>16 + (" #constant ">>15)&1\n" \
      "   add " r1 ", " r1 ", r20\n" \
      "  addi r21, r21, (" #constant "&0x7fff) - (" #constant"&0x8000)\n" \
      "   add " r1 ", " r1 ", r21\n" \
      "  rlwinm " r1 ", " r1 ", " #rotate ", 0, 31\n" \
      "  add " r1 ", " r1 ", " r2 "\n"

      STEP4 (a, b, c, d, 0, 0xf4292244, 6)
      STEP4 (d, a, b, c, 7, 0x432aff97, 10)
      STEP4 (c, d, a, b, 14, 0xab9423a7, 15)
      STEP4 (b, c, d, a, 5, 0xfc93a039, 21)
      STEP4 (a, b, c, d, 12, 0x655b59c3, 6)
      STEP4 (d, a, b, c, 3, 0x8f0ccc92, 10)
      STEP4 (c, d, a, b, 10, 0xffeff47d, 15)
      STEP4 (b, c, d, a, 1, 0x85845dd1, 21)
      STEP4 (a, b, c, d, 8, 0x6fa87e4f, 6)
      STEP4 (d, a, b, c, 15, 0xfe2ce6e0, 10)
      STEP4 (c, d, a, b, 6, 0xa3014314, 15)
      STEP4 (b, c, d, a, 13, 0x4e0811a1, 21)
      STEP4 (a, b, c, d, 4, 0xf7537e82, 6)
      STEP4 (d, a, b, c, 11, 0xbd3af235, 10)
      STEP4 (c, d, a, b, 2, 0x2ad7d2bb, 15)
      STEP4 (b, c, d, a, 9, 0xeb86d391, 21)
#undef STEP4
#undef a
#undef b
#undef c
#undef d

      : "+b" (a), "+b" (b), "+b" (c), "+b" (d)
      : "b" (src)
      : "20", "21", "22");

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}


OIL_DEFINE_IMPL_ASM (md5_asm3, md5);

#endif

