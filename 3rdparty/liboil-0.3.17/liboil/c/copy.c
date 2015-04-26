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


static void
testzero_u8_1 (uint32_t *dest, uint8_t *src1, int n)
{
  uint8_t *s = src1;

  while(n>0 && (((unsigned long)s)&0x3)) {
    if (*s == 0) {
      goto foundzero;
    }
    s++;
    n--;
  }
  while (n>3) {
    uint32_t x;
    x = *(uint32_t *)s;
    if ((x ^ (x - 0x01010101))& 0x80808080) {
      int i;
      /* there's either a 0x00 or 0x80 byte */
      for(i=0;i<4;i++){
        if (s[i] == 0) {
          s += i;
          goto foundzero;
        }
      }
    }
    s += 4;
    n -= 4;
  }
  while(n>0) {
    if (*s == 0) {
      goto foundzero;
    }
    s++;
    n--;
  }

  dest[0] = n;
foundzero:
  dest[0] = s - src1;
}
OIL_DEFINE_IMPL (testzero_u8_1, testzero_u8);

#ifdef HAVE_UNALIGNED_ACCESS
static void
testzero_u8_2 (uint32_t *dest, uint8_t *src1, int n)
{
  int i;

  for(i=0;i<n-3;i+=4) {
    uint32_t x;
    x = *(uint32_t *)(src1 + i);
    if ((x ^ (x - 0x01010101))& 0x80808080) {
      int j;
      /* there's either a 0x00 or 0x80 byte */
      for(j=0;j<4;j++){
        if (src1[i + j] == 0) {
          dest[0] = i + j;
          return ;
        }
      }
    }
  }
  for(;i<n;i++){
    if (src1[i] == 0) {
      dest[0] = i;
      return;
    }
  }
  dest[0] = n;
}
OIL_DEFINE_IMPL (testzero_u8_2, testzero_u8);
#endif

