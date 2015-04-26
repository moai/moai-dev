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
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS (trans8x8_u16);

static void
trans8x8_u16_c1 (uint16_t *dest, int dstr, const uint16_t *src, int sstr)
{
  int i;
  int j;
  uint16_t *d, *s;

  for(i=0;i<8;i++){
    d = OIL_OFFSET(dest, 2*i);
    s = OIL_OFFSET(src, sstr*i);
    for(j=0;j<8;j++){
      OIL_GET(d,dstr*j,uint16_t) = s[j];
    }
  }
}
OIL_DEFINE_IMPL (trans8x8_u16_c1, trans8x8_u16);

static void
trans8x8_u16_c2 (uint16_t *dest, int dstr, const uint16_t *src, int sstr)
{
  int i;
  uint16_t *d, *s;

  for(i=0;i<8;i++){
    d = OIL_OFFSET(dest, 2*i);
    s = OIL_OFFSET(src, sstr*i);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
    *d = *s; s++; d = OIL_OFFSET(d, dstr);
  }
}
OIL_DEFINE_IMPL (trans8x8_u16_c2, trans8x8_u16);

static void
trans8x8_u16_c3 (uint16_t *dest, int dstr, const uint16_t *src, int sstr)
{
  int i;
  uint16_t *d, *s;

  for(i=0;i<8;i++){
    d = OIL_OFFSET(dest, 2*i);
    s = OIL_OFFSET(src, sstr*i);
    OIL_GET(d,dstr*0,uint16_t) = s[0];
    OIL_GET(d,dstr*1,uint16_t) = s[1];
    OIL_GET(d,dstr*2,uint16_t) = s[2];
    OIL_GET(d,dstr*3,uint16_t) = s[3];
    OIL_GET(d,dstr*4,uint16_t) = s[4];
    OIL_GET(d,dstr*5,uint16_t) = s[5];
    OIL_GET(d,dstr*6,uint16_t) = s[6];
    OIL_GET(d,dstr*7,uint16_t) = s[7];
  }
}
OIL_DEFINE_IMPL (trans8x8_u16_c3, trans8x8_u16);

static void
trans8x8_u16_c4 (uint16_t *dest, int dstr, const uint16_t *src, int sstr)
{
  int i;
  uint16_t *d, *s;

  for(i=0;i<8;i++){
    d = OIL_OFFSET(dest, 2*i);
    s = OIL_OFFSET(src, sstr*i);
    OIL_GET(d,dstr*0,uint16_t) = *s++;
    OIL_GET(d,dstr*1,uint16_t) = *s++;
    OIL_GET(d,dstr*2,uint16_t) = *s++;
    OIL_GET(d,dstr*3,uint16_t) = *s++;
    OIL_GET(d,dstr*4,uint16_t) = *s++;
    OIL_GET(d,dstr*5,uint16_t) = *s++;
    OIL_GET(d,dstr*6,uint16_t) = *s++;
    OIL_GET(d,dstr*7,uint16_t) = *s++;
  }
}

OIL_DEFINE_IMPL (trans8x8_u16_c4, trans8x8_u16);


