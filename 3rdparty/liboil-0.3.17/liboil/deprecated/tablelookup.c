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

/**
 * oil_tablelookup_u8:
 * @d:
 * @ds:
 * @s1:
 * @ss1:
 * @s2_256:
 * @ss2:
 * @n:
 *
 * Looks up each element in @s1 using the lookup table in @s2_256,
 * and places the table value in @d.
 */
OIL_DEFINE_CLASS (tablelookup_u8,
    "uint8_t *d, int ds, uint8_t *s1, int ss1, "
    "uint8_t *s2_256, int ss2, int n");

static void tablelookup_u8_ref (uint8_t *dest, int dstr, uint8_t *src,
    int sstr, uint8_t *table, int tablestride, int n)
{
  int i;
  for(i=0;i<n;i++){
    *dest = OIL_GET(table,tablestride*(int)(*src), uint8_t);
    OIL_INCREMENT(dest, dstr);
    OIL_INCREMENT(src, sstr);
  }
}

OIL_DEFINE_IMPL_REF (tablelookup_u8_ref, tablelookup_u8);


