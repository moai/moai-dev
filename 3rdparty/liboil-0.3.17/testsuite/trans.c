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
#include <stdio.h>
#include <string.h>
#include <math.h>

void test(void)
{
  int16_t src2[80];
  int16_t src1[80];
  int16_t dest[80];
  int i;
  int j;

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      dest[i*10+j] = 0;
      //src1[i*10+j] = i*8+j;
      //src1[i*10+j] = rint(100*cos((j+0.5)*i*M_PI/8));
      src1[i*10+j] = 100*(i+j);
      src2[i*10+j] = 2;
    }
  }

  oil_fdct8x8s_s16 (dest, 20, src1, 20);

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      printf("%4d ",src1[i*10+j]);
    }
    printf("\n");
  }
  printf("\n");
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      printf("%4d ",dest[i*10+j]);
    }
    printf("\n");
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  //OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("fdct8x8s_s16");

#if 0
  /* FIXME this doesn't check for CPU validity */
  oil_class_choose_by_name (klass, "fdct8x8s_s16_mmx");
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);
  test();
#endif

  return 0;
}

