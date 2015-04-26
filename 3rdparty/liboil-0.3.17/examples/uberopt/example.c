/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/simdpack/simdpack.h>


UBER_START
static void
abs_u16_s16_i386asm_UBER_INDEX (uint16_t * dest, int dstr, int16_t * src,
    int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:                               \n"
      "    movswl	(%0), %%eax	\n" /* UBER 0:     */
      "    addl	$2, %0			\n" /* UBER 1: 0   */
      "    movl	%%eax, %%edx		\n" /* UBER 2: 0   */
      "    negl	%%edx			\n" /* UBER 3: 2   */
      "    cmpl	$-1, %%eax ; cmovle %%edx, %%eax \n" /* UBER 4: 3 */
      "    movw	%%ax, (%1)		\n" /* UBER 5: 4   */
      "    addl	$2, %1			\n" /* UBER 6: 5   */
      "    decl	%2			\n" /* UBER 7:     */
      "    testl	%2, %2		\n"
      "    jg	1b			\n"
      :"+r" (src), "+r" (dest), "+r" (n)
      ::"eax", "edx");
}
OIL_DEFINE_IMPL_ASM (abs_u16_s16_i386asm_UBER_INDEX, abs_u16_s16);
UBER_END

