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

#ifndef __FUNCTABLE_H__
#define __FUNCTABLE_H__

typedef void FunctableFunc (double *fx, double *dfx, double x, double param1,
    double param2);

typedef struct _Functable Functable;
struct _Functable {
  int length;
  int oversample;

  double offset;
  double multiplier;

  double submultiplier;
  double inv_multiplier;
  double inv_submultiplier;

  double *fx;
  double *dfx;
};

Functable *functable_new (int length, int oversample, double offset, double multiplier);
void functable_free (Functable *t);

void functable_calculate (Functable *t, FunctableFunc func, double param1, double param2);
void functable_calculate_multiply (Functable *t, FunctableFunc func, double param1, double param2);

double functable_evaluate (Functable *t, double x);
double functable_mult_and_sum (Functable *t, double x0, double *data, int n);

FunctableFunc functable_function_sinc;
FunctableFunc functable_function_boxcar;
FunctableFunc functable_function_hanning;

#endif /* __FUNCTABLE_H__ */

