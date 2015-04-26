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
#include <liboil/liboilcpu.h>
#include <liboil/liboiltest.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static int
is_poorly_strided (OilFunctionClass *klass)
{
  OilPrototype *proto;
  int i;
  int j;
  OilParameter *param;

  proto = oil_prototype_from_string (klass->prototype);
  for(i=0;i<proto->n_params;i++) {
    if (proto->params[i].is_stride) {
      for(j=0;j<proto->n_params;j++) {
        param = proto->params + j;
        if (param->parameter_type == (proto->params[i].parameter_type - 1)) {
          if (param->prestride_length == 1 && param->poststride_length == 0) {
            oil_prototype_free (proto);
            return 1;
          }
        }
      }
    }
  }
  oil_prototype_free (proto);
  return 0;
}

static double
get_speed_score (OilFunctionClass *klass)
{
  OilFunctionImpl *impl;
  double max;
  double x;

  max = 1.0;
  for(impl = klass->first_impl; impl; impl = impl->next) {
    x =  klass->reference_impl->profile_ave / impl->profile_ave;
    if (x > max) max = x;
  }
  return max;
}

static void
oil_suggest_class (OilFunctionClass *klass, int verbose)
{
  double x;

  if (is_poorly_strided (klass)) return;

  x = get_speed_score (klass);

  if (x < 20.0) {
    printf("%s %g\n", klass->name, x);
  }

}

static void
oil_suggest_all (void)
{
  int i;
  int n;

  n = oil_class_get_n_classes ();
  for (i = 0; i < n; i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);

    oil_suggest_class (klass, 0);
  }
}

static void
oil_suggest (const char *s)
{
  OilFunctionClass *klass = oil_class_get (s);

  if (klass) {
    oil_suggest_class (klass, 0);
  }
}

int
main (int argc, char *argv[])
{
  int i;

  oil_init();

  if (argc == 1) {
    oil_suggest_all ();
    return 0;
  }

  for(i=1;i<argc;i++){
    oil_suggest (argv[i]);
  }

  return 0;
}

