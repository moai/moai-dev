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

/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2003                *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: fdct8x8theora.c,v 1.3 2005-12-16 17:30:40 ds Exp $

 ********************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboilfuncs.h>
#include <liboil/dct/dct.h>
#include <math.h>

static int32_t xC1S7 = 64277;
static int32_t xC2S6 = 60547;
static int32_t xC3S5 = 54491;
static int32_t xC4S4 = 46341;
static int32_t xC5S3 = 36410;
static int32_t xC6S2 = 25080;
static int32_t xC7S1 = 12785;

#define SIGNBITDUPPED(X) ((signed )(((X) & 0x80000000)) >> 31)
#define DOROUND(X) ( (SIGNBITDUPPED(X) & (0xffff)) + (X) )

/**
 * oil_fdct8x8theora:
 * @s_8x8:
 * @d_8x8:
 *
 * Calculates the FDCT transformation of @s_8x8 according to the Theora
 * specification and places the result in @d_8x8.
 *
 * Note that the source and destination arrays are reversed compared
 * to normal Liboil order.
 */
OIL_DEFINE_CLASS(fdct8x8theora, "int16_t *s_8x8, int16_t *d_8x8");

static void
fdct8x8theora_ref(const int16_t *src, int16_t *dest)
{
  int loop;

  int32_t  is07, is12, is34, is56;
  int32_t  is0734, is1256;
  int32_t  id07, id12, id34, id56;

  int32_t  irot_input_x, irot_input_y;
  int32_t  icommon_product1;   /* Re-used product  (c4s4 * (s12 - s56)). */
  int32_t  icommon_product2;   /* Re-used product  (c4s4 * (d12 + d56)). */

  int32_t  temp1, temp2;         /* intermediate variable for computation */

  int32_t  InterData[64];
  int32_t *ip = InterData;
  int16_t * op = dest;
  for (loop = 0; loop < 8; loop++){
    /* Pre calculate some common sums and differences. */
    is07 = src[0] + src[7];
    is12 = src[1] + src[2];
    is34 = src[3] + src[4];
    is56 = src[5] + src[6];

    id07 = src[0] - src[7];
    id12 = src[1] - src[2];
    id34 = src[3] - src[4];
    id56 = src[5] - src[6];

    is0734 = is07 + is34;
    is1256 = is12 + is56;

    /* Pre-Calculate some common product terms. */
    icommon_product1 = xC4S4*(is12 - is56);
    icommon_product1 = DOROUND(icommon_product1);
    icommon_product1>>=16;

    icommon_product2 = xC4S4*(id12 + id56);
    icommon_product2 = DOROUND(icommon_product2);
    icommon_product2>>=16;


    ip[0] = (xC4S4*(is0734 + is1256));
    ip[0] = DOROUND(ip[0]);
    ip[0] >>= 16;

    ip[4] = (xC4S4*(is0734 - is1256));
    ip[4] = DOROUND(ip[4]);
    ip[4] >>= 16;

    /* Define inputs to rotation for outputs 2 and 6 */
    irot_input_x = id12 - id56;
    irot_input_y = is07 - is34;

    /* Apply rotation for outputs 2 and 6.  */
    temp1=xC6S2*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC2S6*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[2] = temp1 + temp2;

    temp1=xC6S2*irot_input_y;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC2S6*irot_input_x ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[6] = temp1 -temp2 ;

    /* Define inputs to rotation for outputs 1 and 7  */
    irot_input_x = icommon_product1 + id07;
    irot_input_y = -( id34 + icommon_product2 );

    /* Apply rotation for outputs 1 and 7.  */

    temp1=xC1S7*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC7S1*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[1] = temp1 - temp2;

    temp1=xC7S1*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC1S7*irot_input_y ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[7] = temp1 + temp2 ;

    /* Define inputs to rotation for outputs 3 and 5 */
    irot_input_x = id07 - icommon_product1;
    irot_input_y = id34 - icommon_product2;

    /* Apply rotation for outputs 3 and 5. */
    temp1=xC3S5*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC5S3*irot_input_y ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[3] = temp1 - temp2 ;

    temp1=xC5S3*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC3S5*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    ip[5] = temp1 + temp2;

    /* Increment data pointer for next row. */
    src += 8 ;
    ip += 8; /* advance pointer to next row */

  }


  /* Performed DCT on rows, now transform the columns */
  ip = InterData;
  for (loop = 0; loop < 8; loop++){
    /* Pre calculate some common sums and differences.  */
    is07 = ip[0 * 8] + ip[7 * 8];
    is12 = ip[1 * 8] + ip[2 * 8];
    is34 = ip[3 * 8] + ip[4 * 8];
    is56 = ip[5 * 8] + ip[6 * 8];

    id07 = ip[0 * 8] - ip[7 * 8];
    id12 = ip[1 * 8] - ip[2 * 8];
    id34 = ip[3 * 8] - ip[4 * 8];
    id56 = ip[5 * 8] - ip[6 * 8];

    is0734 = is07 + is34;
    is1256 = is12 + is56;

    /* Pre-Calculate some common product terms. */
    icommon_product1 = xC4S4*(is12 - is56) ;
    icommon_product2 = xC4S4*(id12 + id56) ;
    icommon_product1 = DOROUND(icommon_product1);
    icommon_product2 = DOROUND(icommon_product2);
    icommon_product1>>=16;
    icommon_product2>>=16;


    temp1 = xC4S4*(is0734 + is1256) ;
    temp2 = xC4S4*(is0734 - is1256) ;
    temp1 = DOROUND(temp1);
    temp2 = DOROUND(temp2);
    temp1>>=16;
    temp2>>=16;
    op[0*8] = (int16_t) temp1;
    op[4*8] = (int16_t) temp2;

    /* Define inputs to rotation for outputs 2 and 6 */
    irot_input_x = id12 - id56;
    irot_input_y = is07 - is34;

    /* Apply rotation for outputs 2 and 6.  */
    temp1=xC6S2*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC2S6*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[2*8] = (int16_t) (temp1 + temp2);

    temp1=xC6S2*irot_input_y;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC2S6*irot_input_x ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[6*8] = (int16_t) (temp1 -temp2) ;

    /* Define inputs to rotation for outputs 1 and 7 */
    irot_input_x = icommon_product1 + id07;
    irot_input_y = -( id34 + icommon_product2 );

    /* Apply rotation for outputs 1 and 7. */
    temp1=xC1S7*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC7S1*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[1*8] = (int16_t) (temp1 - temp2);

    temp1=xC7S1*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC1S7*irot_input_y ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[7*8] = (int16_t) (temp1 + temp2);

    /* Define inputs to rotation for outputs 3 and 5 */
    irot_input_x = id07 - icommon_product1;
    irot_input_y = id34 - icommon_product2;

    /* Apply rotation for outputs 3 and 5. */
    temp1=xC3S5*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC5S3*irot_input_y ;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[3*8] = (int16_t) (temp1 - temp2) ;

    temp1=xC5S3*irot_input_x;
    temp1=DOROUND(temp1);
    temp1>>=16;
    temp2=xC3S5*irot_input_y;
    temp2=DOROUND(temp2);
    temp2>>=16;
    op[5*8] = (int16_t) (temp1 + temp2);

    /* Increment data pointer for next column.  */
    ip ++;
    op ++;
  }
}

OIL_DEFINE_IMPL_REF (fdct8x8theora_ref, fdct8x8theora);

