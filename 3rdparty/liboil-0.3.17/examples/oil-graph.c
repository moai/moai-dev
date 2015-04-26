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
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include <liboil/liboilcpu.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

void register_impls(void);

void test(void)
{
  int32_t dest[1];
  uint8_t src[100];
  int i;

  for(i=0;i<100;i++){
    src[i] = oil_rand_u8() & 0x7f;
  }
  dest[0] = 0;

  oil_utf8_validate (dest, src, 100);

#if 0
  for(i=0;i<100;i++){
    printf("%d %d\n",dest[i],src[i]);
  }
#endif
  printf("%d\n", dest[0]);

}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilTest *test;
  double ave, std;
  int n;

  oil_init ();

  if (argc < 2) {
    printf("oil-test <class_name>\n");
    exit(0);
  }

  klass = oil_class_get (argv[1]);
  if (klass == NULL) {
    printf("class not found: %s\n", argv[1]);
    exit(0);
  }
  oil_class_optimize (klass);

  for (n = 0; n < 200; n+=1) {
    printf("%d", n);

    test = oil_test_new(klass);
    oil_test_set_iterations(test, 10);
    test->n = n;
    test->m = 10;

    impl = klass->reference_impl;
    ave = impl->profile_ave;
    std = impl->profile_std;
    oil_test_check_impl (test, impl);

    for (impl = klass->first_impl; impl; impl = impl->next) {
      if (oil_impl_is_usable (impl)) {
        oil_test_check_impl (test, impl);
        //printf(" %g %g", test->profile_ave, test->profile_std);
        printf(" %g", test->profile_ave);
      }
    }
    printf("\n");
  }

  return 0;
}

