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
#include <liboil/liboilclasses.h>


#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f,w,x,y,z,in,s) \
  (w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)


static void
md5_asm1(uint32_t *state, uint32_t *src)
{
  struct {
    void *state;
    void *src;
    void *ebp;
    void *ebx;
  }tmp;

  tmp.state = state;
  tmp.src = src;

  __asm__ __volatile__ (
      "  mov %%ebp, 0x8(%%eax)\n"
      "  mov %%ebx, 0xc(%%eax)\n"
      "  mov %%eax, %%ebp\n"
      "  mov (%%ebp), %%esi\n"
      "  mov (%%esi), %%eax\n"
      "  mov 0x4(%%esi), %%ebx\n"
      "  mov 0x8(%%esi), %%ecx\n"
      "  mov 0xc(%%esi), %%edx\n"
      "  mov 0x4(%%ebp), %%esi\n"

#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  and %%e" #r2 "x, %%edi\n" \
      "  xor %%e" #r4 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  add (" #offset "* 4)(%%esi), %%e" #r1 "x\n" \
      "  add $" #constant ", %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r3 "x, %%edi\n" \
      "  xor %%e" #r2 "x, %%edi\n" \
      "  and %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  add (" #offset "*4)(%%esi), %%e" #r1 "x\n" \
      "  add $" #constant ", %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  xor %%e" #r2 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  add (" #offset "*4)(%%esi), %%e" #r1 "x\n" \
      "  add $" #constant ", %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  not %%edi\n" \
      "  or %%e" #r2 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  add (" #offset "*4)(%%esi), %%e" #r1 "x\n" \
      "  add $" #constant ", %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

      "  mov (%%ebp), %%edi\n"
      "  add %%eax, 0x0(%%edi)\n"
      "  add %%ebx, 0x4(%%edi)\n"
      "  add %%ecx, 0x8(%%edi)\n"
      "  add %%edx, 0xc(%%edi)\n"
      "  mov 0xc(%%ebp), %%ebx\n"
      "  mov 0x8(%%ebp), %%ebp\n"
      :
      : "a" (&tmp)
      : "esi", "ecx", "edx", "edi", "memory");
}
#undef STEP1
#undef STEP2
#undef STEP3
#undef STEP4
OIL_DEFINE_IMPL (md5_asm1, md5);

static void
md5_asm2(uint32_t *state, uint32_t *src)
{
  struct {
    void *state;
    void *src;
    void *ebp;
    void *ebx;
  }tmp;

  tmp.state = state;
  tmp.src = src;

  asm (
      "  mov %%ebp, 0x8(%%eax)\n"
      "  mov %%ebx, 0xc(%%eax)\n"
      "  mov %%eax, %%ebp\n"
      "  mov (%%ebp), %%esi\n"
      "  mov (%%esi), %%eax\n"
      "  mov 0x4(%%esi), %%ebx\n"
      "  mov 0x8(%%esi), %%ecx\n"
      "  mov 0xc(%%esi), %%edx\n"
      "  mov 0x4(%%ebp), %%esi\n"

#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  and %%e" #r2 "x, %%edi\n" \
      "  xor %%e" #r4 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r3 "x, %%edi\n" \
      "  xor %%e" #r2 "x, %%edi\n" \
      "  and %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  xor %%e" #r2 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  not %%edi\n" \
      "  or %%e" #r2 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

      "  mov (%%ebp), %%edi\n"
      "  add %%eax, 0x0(%%edi)\n"
      "  add %%ebx, 0x4(%%edi)\n"
      "  add %%ecx, 0x8(%%edi)\n"
      "  add %%edx, 0xc(%%edi)\n"
      "  mov 0xc(%%ebp), %%ebx\n"
      "  mov 0x8(%%ebp), %%ebp\n"
      :
      : "a" (&tmp)
      : "esi", "ecx", "edx", "edi", "memory");
#undef STEP1
#undef STEP2
#undef STEP3
#undef STEP4
}
OIL_DEFINE_IMPL_ASM (md5_asm2, md5);



#ifdef ENABLE_BROKEN_IMPLS
/* FIXME this is way too clever.  Using %esp as a general purpose
 * register?  NOT a brilliant idea. */
static void
md5_asm3(uint32_t *state, uint32_t *src)
{
  struct {
    void *state;
    void *src;
    void *ebp;
    void *esp;
    void *ebx;
  }tmp;

  tmp.state = state;
  tmp.src = src;

  asm (
      "  mov %%ebp, 0x8(%%eax)\n"
      "  mov %%esp, 0xc(%%eax)\n"
      "  mov %%ebx, 0x10(%%eax)\n"
      "  mov %%eax, %%ebp\n"
      "  mov (%%ebp), %%esi\n"
      "  mov (%%esi), %%eax\n"
      "  mov 0x4(%%esi), %%ebx\n"
      "  mov 0x8(%%esi), %%ecx\n"
      "  mov 0xc(%%esi), %%edx\n"
      "  mov 0x4(%%ebp), %%esi\n"

#define STEP1(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r2 "x, %%edi\n" \
      "   mov %%e" #r2 "x, %%esp\n" \
      "   not %%esp\n" \
      "  and %%e" #r3 "x, %%edi\n" \
      "   and %%e" #r4 "x, %%esp\n" \
      "  or %%esp, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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
      
#define STEP2(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "   mov %%e" #r4 "x, %%esp\n" \
      "   not %%esp\n" \
      "  and %%e" #r2 "x, %%edi\n" \
      "   and %%e" #r3 "x, %%esp\n" \
      "  or %%esp, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP3(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  xor %%e" #r2 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

#define STEP4(r1,r2,r3,r4,offset,constant,rotate) \
      "  mov %%e" #r4 "x, %%edi\n" \
      "  not %%edi\n" \
      "  or %%e" #r2 "x, %%edi\n" \
      "  xor %%e" #r3 "x, %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  mov (" #offset "* 4)(%%esi), %%edi\n" \
      "  add $" #constant ", %%edi\n" \
      "  add %%edi, %%e" #r1 "x\n" \
      "  rol $" #rotate ", %%e" #r1 "x\n" \
      "  add %%e" #r2 "x, %%e" #r1 "x\n"

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

      "  mov (%%ebp), %%edi\n"
      "  add %%eax, 0x0(%%edi)\n"
      "  add %%ebx, 0x4(%%edi)\n"
      "  add %%ecx, 0x8(%%edi)\n"
      "  add %%edx, 0xc(%%edi)\n"
      "  mov 0x10(%%ebp), %%ebx\n"
      "  mov 0xc(%%ebp), %%esp\n"
      "  mov 0x8(%%ebp), %%ebp\n"
      :
      : "a" (&tmp)
      : "esi", "ecx", "edx", "edi");
#undef STEP1
#undef STEP2
#undef STEP3
#undef STEP4
}


OIL_DEFINE_IMPL_ASM (md5_asm3, md5);
#endif

