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


#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <stdio.h>
#include <stdlib.h>

void test(void)
{
  int i;
  float *src;
  float *dest;

  src = malloc(100*sizeof(float));
  dest = malloc(100*sizeof(float));

  for(i=0;i<100;i++){
    src[i] = (i-50)*100;
  }

  oil_abs_f32_f32 (dest, 4, src, 4, 100);

  for (i=0; i<100; i++) {
    printf("%d %g %g\n", i, src[i], dest[i]);
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("abs_f32_f32");
  oil_class_optimize(klass);

  printf("class=%s\n", klass->name);
  for (impl = klass->first_impl; impl; impl=impl->next) {
    printf("impl=%p\n", impl);
    printf("  func=%p\n", impl->func);
    printf("  name=%s\n", impl->name);
    printf("  flags=%08x\n", impl->flags);

  }
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);

  test();

  return 0;
}

