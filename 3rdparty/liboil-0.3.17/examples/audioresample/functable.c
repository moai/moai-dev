/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2006 David A. Schleef <ds@schleef.org>
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
#include <config.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboildebug.h>

#include "functable.h"



void
functable_function_sinc(double *fx, double *dfx, double x,
    double param1, double param2)
{
  double y;
  if(x==0){
    *fx = 1;
    *dfx = 0;
    return;
  }

  y = x * param1;
  *fx = sin(y)/y;
  *dfx = (cos(y) - sin(y)/y)/x;
}

void
functable_function_boxcar(double *fx, double *dfx, double x,
    double param1, double param2)
{
  double width = param1;

  if (x < width && x > -width) {
    *fx = 1;
  } else {
    *fx = 0;
  }
  *dfx = 0;
}

void
functable_function_hanning(double *fx, double *dfx, double x,
    double param1, double param2)
{
  double width = param1;

  if (x < width && x > -width) {
    x /= width;
    *fx = (1-x*x)*(1-x*x);
    *dfx = -2*2*x/width*(1-x*x);
  } else {
    *fx = 0;
    *dfx = 0;
  }
}


Functable *
functable_new (int length, int oversample, double offset, double multiplier)
{
  Functable *ft;

  ft = malloc (sizeof(Functable));
  memset (ft, 0, sizeof(Functable));

  ft->length = length;
  ft->offset = offset;
  ft->multiplier = multiplier;
  ft->oversample = oversample;

  ft->inv_multiplier = 1.0 / ft->multiplier;
  ft->submultiplier = ft->multiplier / ft->oversample;
  ft->inv_submultiplier = 1.0 / ft->submultiplier;

  ft->fx = malloc(sizeof(double)*ft->length*(ft->oversample + 1));
  ft->dfx = malloc(sizeof(double)*ft->length*(ft->oversample + 1));

  memset (ft->fx, 0, sizeof(double)*ft->length*(ft->oversample + 1));
  memset (ft->dfx, 0, sizeof(double)*ft->length*(ft->oversample + 1));

  return ft;
}

void
functable_free (Functable *ft)
{
  free (ft->fx);
  free (ft->dfx);
  free (ft);
}

void
functable_calculate (Functable *t, FunctableFunc func, double param1,
    double param2)
{
  int i, j;
  double x;
  double *fx;
  double *dfx;

  for(j=0;j<t->oversample + 1;j++){
    fx = t->fx + t->length*j;
    dfx = t->dfx + t->length*j;
    for(i=0;i<t->length;i++){
      x = t->offset + t->submultiplier * (i * t->oversample + j);

      func (&fx[i], &dfx[i], x, param1, param2);
      dfx[i] *= t->submultiplier;
    }
  }
}

void
functable_calculate_multiply (Functable *t, FunctableFunc func,
    double param1, double param2)
{
  int i;
  int j;
  double x;
  double *fx;
  double *dfx;
  double afx, dafx, bfx, dbfx;

  for(j=0;j<t->oversample + 1;j++){
    fx = t->fx + t->length*j;
    dfx = t->dfx + t->length*j;
    for(i=0;i<t->length;i++){
      x = t->offset + t->submultiplier * (i * t->oversample + j);

      afx = fx[i];
      dafx = dfx[i];
      func (&bfx, &dbfx, x, param1, param2);
      fx[i] = afx * bfx;
      dfx[i] = afx * dbfx + dafx * bfx;
    }
  }
}

double
functable_evaluate (Functable *t, double x)
{
  int i, j;
  double f0, f1, w0, w1;
  double x2, x3;
  double w;
  double *fx1, *fx2;
  double *dfx1, *dfx2;
  int xi;

  if(x<t->offset || x>(t->offset+t->length*t->multiplier)){
    OIL_DEBUG ("x out of range %g",x);
    return 0;
  }

  x -= t->offset;
  x *= t->inv_submultiplier;
  xi = floor(x);
  j = xi % t->oversample;
  i = xi / t->oversample;
  x -= xi;

  fx1 = t->fx + j*t->length;
  fx2 = t->fx + (j+1)*t->length;
  dfx1 = t->dfx + j*t->length;
  dfx2 = t->dfx + (j+1)*t->length;

  x2 = x * x;
  x3 = x2 * x;

  f1 = 3 * x2 - 2 * x3;
  f0 = 1 - f1;
  w0 = x - 2 * x2 + x3;
  w1 = -x2 + x3;

  w = fx1[i] * f0 + fx2[i] * f1 + dfx1[i] * w0 + dfx2[i] * w1;

  return w;
}

double
functable_mult_and_sum (Functable *t, double x, double *data, int n)
{
  int i, j;
  double f0, f1, w0, w1;
  double x2, x3;
  double w;
  double *fx1, *fx2;
  double *dfx1, *dfx2;
  int xi;
  double sum;

  if(x<t->offset || x>(t->offset+t->length*t->multiplier)){
    OIL_DEBUG ("x out of range %g",x);
    return 0;
  }

  x -= t->offset;
  x *= t->inv_submultiplier;
  xi = floor(x);
  j = xi % t->oversample;
  i = xi / t->oversample;
  x -= xi;

  fx1 = t->fx + j*t->length + i;
  fx2 = t->fx + (j+1)*t->length + i;
  dfx1 = t->dfx + j*t->length + i;
  dfx2 = t->dfx + (j+1)*t->length + i;

  x2 = x * x;
  x3 = x2 * x;

  f1 = 3 * x2 - 2 * x3;
  f0 = 1 - f1;
  w0 = x - 2 * x2 + x3;
  w1 = -x2 + x3;

  sum = 0;
  for(i=0;i<n;i++){
    w = fx1[i] * f0 + fx2[i] * f1 + dfx1[i] * w0 + dfx2[i] * w1;
    sum += w * data[i];
  }

  return sum;
}


