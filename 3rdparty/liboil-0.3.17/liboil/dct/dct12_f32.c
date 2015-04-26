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
#include <liboil/dct/dct.h>
#include <math.h>


/**
 * oil_mdct12_f64:
 * @d_6:
 * @s_12:
 *
 * Performs a Modified Discrete Cosine Transform (MDCT)
 * on the source array @s_12 and places the result in @d_6.
 */
OIL_DEFINE_CLASS (mdct12_f64, "double *d_6, double *s_12");
/**
 * oil_imdct12_f64:
 * @d_12:
 * @s_6:
 *
 * Performs a Inverse Modified Discrete Cosine Transform (IMDCT)
 * on the source array @s_6 and places the result in @d_12.
 */
OIL_DEFINE_CLASS (imdct12_f64, "double *d_12, double *s_6");
/**
 * oil_mdct36_f64:
 * @d_18:
 * @s_36:
 *
 * Performs a Modified Discrete Cosine Transform (MDCT)
 * on the source array @s_36 and places the result in @d_18.
 */
OIL_DEFINE_CLASS (mdct36_f64, "double *d_18, double *s_36");
/**
 * oil_imdct36_f64:
 * @d_36:
 * @s_18:
 *
 * Performs a Inverse Modified Discrete Cosine Transform (IMDCT)
 * on the source array @s_18 and places the result in @d_36.
 */
OIL_DEFINE_CLASS (imdct36_f64, "double *d_36, double *s_18");

static void
mdct12_f64_ref(double *dest, const double *src)
{
  int j, k;
  double x;
  int N = 6;

  for(j=0;j<N;j++){
    x = 0;
    for(k=0;k<2*N;k++){
      x += src[k] * cos (M_PI/N * (j+0.5) * (k + 0.5 * (N+1)));
    }
    dest[j] = x;
  }
}
OIL_DEFINE_IMPL_REF (mdct12_f64_ref, mdct12_f64);


static void
imdct12_f64_ref(double *dest, const double *src)
{
  int j,k;
  double x;
  int N = 6;

  for(k=0;k<2*N;k++){
    x = 0;
    for(j=0;j<N;j++){
      x += src[j] * cos (M_PI/N * (j+0.5) * (k+0.5*(N + 1)));
    }
    dest[k] = x;
  }
}
OIL_DEFINE_IMPL_REF (imdct12_f64_ref, imdct12_f64);

static void
mdct36_f64_ref(double *dest, const double *src)
{
  int j, k;
  double x;
  int N = 18;

  for(j=0;j<N;j++){
    x = 0;
    for(k=0;k<2*N;k++){
      x += src[k] * cos (M_PI/N * (j+0.5) * (k + 0.5 * (N+1)));
    }
    dest[j] = x;
  }
}
OIL_DEFINE_IMPL_REF (mdct36_f64_ref, mdct36_f64);


static void
imdct36_f64_ref(double *dest, const double *src)
{
  int j,k;
  double x;
  int N = 18;

  for(k=0;k<2*N;k++){
    x = 0;
    for(j=0;j<N;j++){
      x += src[j] * cos (M_PI/N * (j+0.5) * (k+0.5*(N + 1)));
    }
    dest[k] = x;
  }
}
OIL_DEFINE_IMPL_REF (imdct36_f64_ref, imdct36_f64);



