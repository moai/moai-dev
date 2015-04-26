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
#include <time.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifndef PRIx8
#define PRIx8  "x"
#define PRIx16 "x"
#define PRIx32 "x"
#define PRIx64 "llx"
#define PRId8  "d"
#define PRId16 "d"
#define PRId32 "d"
#define PRId64 "lld"
#define PRIu8  "u"
#define PRIu16 "u"
#define PRIu32 "u"
#define PRIu64 "llu"
#endif

int hex;

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

void
dump_array (void *data, void *ref_data, OilType type, int pre_n, int stride,
    int post_n)
{
  int i, j;
  int s2 = oil_type_sizeof (type);
  double x;

#define DUMP(type, format) do { \
  for(i=0;i<post_n;i++){ \
    printf("    "); \
    for(j=0;j<pre_n;j++){ \
      x = fabs(OIL_GET(data, i*stride + j*s2, type) - \
          OIL_GET(ref_data, i*stride + j*s2, type)); \
      if (x >= 0.00001) { \
        printf("*" format "* ", OIL_GET(data, i*stride + j*s2, type)); \
      } else { \
        printf(format " ", OIL_GET(data, i*stride + j*s2, type)); \
      } \
    } \
    printf("\n"); \
  } \
} while(0)

  if (hex) {
    switch(type) {
      case OIL_TYPE_s8p:
      case OIL_TYPE_u8p:
        DUMP(uint8_t, "%02" PRIx8);
        break;
      case OIL_TYPE_s16p:
      case OIL_TYPE_u16p:
        DUMP(uint16_t, "%04" PRIx16);
        break;
      case OIL_TYPE_s32p:
      case OIL_TYPE_u32p:
      case OIL_TYPE_f32p:
        DUMP(uint32_t, "%08" PRIx32);
        break;
      case OIL_TYPE_s64p:
      case OIL_TYPE_u64p:
      case OIL_TYPE_f64p:
        DUMP(uint64_t, "%016" PRIx64);
        break;
      default:
        break;
    }
  } else {
    switch(type) {
      case OIL_TYPE_s8p:
        DUMP(int8_t, "%" PRId8);
        break;
      case OIL_TYPE_u8p:
        DUMP(uint8_t, "%" PRIu8);
        break;
      case OIL_TYPE_s16p:
        DUMP(int16_t, "%" PRId16);
        break;
      case OIL_TYPE_u16p:
        DUMP(uint16_t, "%" PRIu16);
        break;
      case OIL_TYPE_s32p:
        DUMP(int32_t, "%" PRId32);
        break;
      case OIL_TYPE_u32p:
        DUMP(uint32_t, "%" PRIu32);
        break;
      case OIL_TYPE_s64p:
        DUMP(int64_t, "%" PRId64);
        break;
      case OIL_TYPE_u64p:
        DUMP(uint64_t, "%" PRIu64);
        break;
      case OIL_TYPE_f32p:
        DUMP(float, "%g");
        break;
      case OIL_TYPE_f64p:
        DUMP(double, "%g");
        break;
      default:
        break;
    }
  }
}

void
dump_test (OilTest *test)
{
  int i;
  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];
    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 'd') {
        printf ("  %s:\n", p->parameter_name);
        dump_array (p->test_data + OIL_TEST_HEADER,
            p->ref_data + OIL_TEST_HEADER,
            p->type, p->pre_n, p->stride, p->post_n);
      }
    }
  }
}

void
dump_source (OilTest *test)
{
  int i;
  for(i=0;i<OIL_ARG_LAST;i++){
    OilParameter *p = &test->params[i];
    if (p->is_pointer) {
      if (p->direction == 'i' || p->direction == 's') {
        printf ("  %s:\n", p->parameter_name);
        dump_array (p->src_data + OIL_TEST_HEADER,
            p->src_data + OIL_TEST_HEADER,
            p->type, p->pre_n, p->stride, p->post_n);
      }
    }
  }
}

void
help (void)
{
  printf("oil-test [-x] <class_name>\n");
  exit(0);
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  OilTest *test;
  double ave, std;
  char *class_name = NULL;
  int i;
  int n = 10;

  srand(time(NULL));
  oil_init ();

  for (i=1;i<argc;i++){
    if (!strcmp(argv[i],"-x")) {
      hex = 1;
    } else if (!strcmp(argv[i],"-n")) {
      if (i + 1 < argc) {
        n = strtol (argv[i+1], NULL, 0);
        i++;
      }
    } else {
      if (class_name != NULL) {
        help();
      }
      class_name = argv[i];
    }
  }
  if (class_name == NULL) {
    help();
  }

  klass = oil_class_get (class_name);
  if (klass == NULL) {
    printf("class not found: %s\n", class_name);
    exit(0);
  }
  oil_class_optimize (klass);

  test = oil_test_new(klass);
  oil_test_set_iterations(test, 1);
  test->n = n;
  test->m = n;

  impl = klass->reference_impl;
  ave = impl->profile_ave;
  std = impl->profile_std;
  oil_test_check_impl (test, impl);
  printf ("source array\n");
  dump_source(test);
  printf ("reference impl %s\n", impl->name);
  printf("  ave=%g std=%g\n", ave, std);
  printf("  (this test) ave=%g std=%g\n", test->profile_ave, test->profile_std);
  dump_test(test);

  for (impl = klass->first_impl; impl; impl = impl->next) {
    if (impl == klass->reference_impl) continue;
    printf ("impl %s\n", impl->name);
    if (oil_impl_is_runnable (impl)) {
      printf("  ave=%g std=%g\n", impl->profile_ave, impl->profile_std);
      oil_test_check_impl (test, impl);
      printf("  (this test) ave=%g std=%g\n", test->profile_ave, test->profile_std);
      printf("  abs diff=%g\n", test->sum_abs_diff);
      dump_test(test);
    }
  }

  return 0;
}

