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

#ifndef __RESAMPLE_H__
#define __RESAMPLE_H__

#include "functable.h"

typedef enum {
  RESAMPLE_FORMAT_S16 = 0,
  RESAMPLE_FORMAT_S32,
  RESAMPLE_FORMAT_F32,
  RESAMPLE_FORMAT_F64
} ResampleFormat;

typedef void (*ResampleCallback) (void *);

typedef struct _ResampleState ResampleState;

struct _ResampleState {
  /* static parameters */
  int n_channels;
  ResampleFormat format;

  /* dynamic parameters */
  int filter_length;
  int ratio_n;
  int ratio_d;
  double sinc_scale;
  int oversample;

  /* internal parameters */

  void *last_buffer;
  int need_reinit;
  int sample_size;
  Functable *ft;

  void *forward_data;
  int forward_samples;

  /* filter state */

  int eos;

  void *buffer;
  int buffer_len;

  int offset;

};

void resample_init(void);

ResampleState *resample_new (ResampleFormat format, int n_channels);
void resample_free (ResampleState *state);

void resample_set_filter_length (ResampleState *r, int length);
void resample_set_ratio (ResampleState *r, int n, int d);
int resample_get_sample_size (ResampleState *r);

void resample_preload_data (ResampleState * r, void *data, int n_samples);
void resample_set_offset (ResampleState *r, int offset);
int resample_get_offset (ResampleState *r);

void resample_push_data (ResampleState * r, void *data, int n_samples);
void resample_push_eos (ResampleState *r);

int resample_get_output_size (ResampleState *r);
void resample_get_output_data (ResampleState *r, void *data, int size);

#endif /* __RESAMPLE_H__ */

