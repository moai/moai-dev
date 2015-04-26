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


#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <liboil/liboil.h>

#include "resample.h"

#define MAX_FILTER_LENGTH 64

#define OFFSET(ptr,offset) ((void *)((uint8_t *)(ptr) + (offset)) )

static int resample_format_size (ResampleFormat format);

void resample_scale_ref (ResampleState * r);
void resample_scale_functable (ResampleState * r);



void
resample_init (void)
{
  static int inited = 0;
  if (!inited) {
    oil_init ();
    inited = 1;
  }
}

ResampleState *
resample_new (ResampleFormat format, int n_channels)
{
  ResampleState *r;
  int size;

  r = malloc(sizeof(ResampleState));
  memset(r,0,sizeof(ResampleState));

  r->format = format;
  r->n_channels = n_channels;

  r->filter_length = 16;
  r->ratio_n = 1;
  r->ratio_d = 1;

  r->sinc_scale = 1.0;
  r->oversample = 10;

  r->sample_size = resample_format_size (r->format) * r->n_channels;
  size = MAX_FILTER_LENGTH * r->sample_size;
  r->last_buffer = malloc (size);
  memset (r->last_buffer, 0, size);

  r->ft = functable_new(r->filter_length, r->oversample,
      -0.5*r->filter_length, 1.0);

  functable_calculate (r->ft, functable_function_sinc, M_PI, 0);

  r->need_reinit = 1;

  return r;
}

static int
resample_format_size (ResampleFormat format)
{
  switch(format){
    case RESAMPLE_FORMAT_S16:
      return 2;
    case RESAMPLE_FORMAT_S32:
    case RESAMPLE_FORMAT_F32:
      return 4;
    case RESAMPLE_FORMAT_F64:
      return 8;
  }
  return 0;
}

void
resample_free (ResampleState * r)
{
  if (r->last_buffer) {
    free (r->last_buffer);
  }
  if (r->ft) {
    functable_free (r->ft);
  }

  free (r);
}


void
resample_set_filter_length (ResampleState *r, int length)
{
  r->filter_length = length;

  /* FIXME adjust the buffer */
}

void
resample_set_ratio (ResampleState *r, int n, int d)
{
  r->ratio_n = n;
  r->ratio_d = d;

  /* FIXME adjust offset */
}

int
resample_get_sample_size (ResampleState *r)
{
  return r->sample_size;
}

void
resample_preload_data (ResampleState *r, void *data, int n_samples)
{
  if (n_samples > r->filter_length) {
    data = OFFSET(data, r->sample_size * (n_samples - r->filter_length));
    n_samples = r->filter_length;
  }
  memcpy (OFFSET(r->buffer, r->sample_size * (r->filter_length - n_samples)),
      data, r->sample_size * n_samples);
}

void
resample_set_offset (ResampleState *r, int offset)
{
  r->offset = offset;
  /* FIXME */
}

int
resample_get_offset (ResampleState *r)
{
  return r->offset;
}



void
resample_push_data (ResampleState * r, void *data, int n_samples)
{
  r->forward_data = data;
  r->forward_samples = n_samples;
}

void
resample_push_eos (ResampleState *r)
{
  r->eos = 1;
}

int
resample_get_output_size (ResampleState *r)
{
  if (r->eos) {
    return 42;
  } else {
    return 43;
  }
}

void
resample_get_output_data (ResampleState *r, void *data, int size)
{
  /* FIXME do stuff */
}

