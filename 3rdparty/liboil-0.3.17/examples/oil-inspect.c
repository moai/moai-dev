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

int retval = EXIT_SUCCESS;
static void
printerr (char *format, ...)
{
  va_list var_args;

  va_start (var_args, format);
  vfprintf (stderr, format, var_args);
  va_end (var_args);
  retval = EXIT_FAILURE;
}
  
static char *
string_append (char *str, const char *append)
{
  char *ret;
  if (str) {
    size_t size = strlen (str) + 2 + strlen (append) + 1;
    ret = malloc (size);
    snprintf (ret, size, "%s, %s", str, append);
    free (str);
  } else {
    ret = strdup (append);
  }
  return ret;
}
static char *
oil_flags_to_string (unsigned int flags)
{
  char *ret = NULL;
  
  if (flags & OIL_IMPL_FLAG_REF) 
    ret = string_append (ret, "REF");
  if (flags & OIL_IMPL_FLAG_OPT) 
    ret = string_append (ret, "altopt");
  if (flags & OIL_IMPL_FLAG_ASM) 
    ret = string_append (ret, "asm");
  if (flags & OIL_IMPL_FLAG_CMOV) 
    ret = string_append (ret, "cmov");
  if (flags & OIL_IMPL_FLAG_MMX) 
    ret = string_append (ret, "mmx");
  if (flags & OIL_IMPL_FLAG_SSE) 
    ret = string_append (ret, "sse");
  if (flags & OIL_IMPL_FLAG_MMXEXT) 
    ret = string_append (ret, "mmxext");
  if (flags & OIL_IMPL_FLAG_SSE2) 
    ret = string_append (ret, "sse2");
  if (flags & OIL_IMPL_FLAG_3DNOW) 
    ret = string_append (ret, "3dnow");
  if (flags & OIL_IMPL_FLAG_3DNOWEXT) 
    ret = string_append (ret, "3dnowext");
  if (flags & OIL_IMPL_FLAG_ALTIVEC) 
    ret = string_append (ret, "altivec");
  return ret;
}

static void
oil_print_impl (OilFunctionImpl *impl, OilTest *test, char* prefix)
{
  char *c;
  unsigned int cpu_flags = oil_cpu_get_flags();

  if (oil_impl_is_runnable (impl)) {
    oil_test_check_impl (test, impl);
  }

  printf ("%s%s\n", prefix, impl->name);
  c = oil_flags_to_string (impl->flags);
  if (c) {
    printf ("%s  flags: %s\n", prefix, c);
    free (c);
  }
  if (impl->profile_ave) {
    printf ("%s  profile: %g ticks (std.dev. %g)\n", prefix, impl->profile_ave,
        impl->profile_std);
  }
  if (test && !(impl->flags & OIL_IMPL_FLAG_REF)) {
    printf ("%s  sum abs difference: %g (n=%d)\n", prefix,
        test->sum_abs_diff, test->n_points);
  }
  if ((impl->flags & OIL_CPU_FLAG_MASK) & (~cpu_flags)) {
    printf ("%s  disabled\n", prefix);
  }
}

static int
impl_compare (const void *a, const void *b)
{
  const OilFunctionImpl *ia = *(OilFunctionImpl **)a;
  const OilFunctionImpl *ib = *(OilFunctionImpl **)b;

  if (ia->profile_ave < ib->profile_ave) return -1;
  if (ia->profile_ave > ib->profile_ave) return 1;
  return 0;
}

static void
oil_print_class (OilFunctionClass *klass, int verbose)
{
  OilFunctionImpl *impl;
  OilFunctionImpl **list;
  int n;
  int i;
  OilTest *test;
  
  if (!verbose) {
    printf ("%s\n", klass->name);
    return;
  }
  printf ("%s (%s)\n", klass->name, klass->prototype);
  printf ("  all implementations: \n");

  n = 0;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    n++;
  }
  list = malloc(n * sizeof(OilFunctionImpl *));
  i = 0;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    list[i] = impl;
    i++;
  }

  qsort (list, n, sizeof(OilFunctionImpl *), impl_compare);

  test = oil_test_new (klass);

  for (i=0;i<n;i++){
    impl = list[i];
    if ((impl->flags & OIL_IMPL_FLAG_REF) &&
	klass->reference_impl != impl) {
      printerr ("warning: function %s is not reference implementation for class %s\n",
	  impl->name, klass->name);
    }
    oil_print_impl (impl, test, "    ");
    if (klass->chosen_impl == impl) {
      printf ("      currently chosen\n");
    }
  }

  oil_test_free (test);

  free(list);
}

static void
oil_print_all (void)
{
  int i;
  int n;

  n = oil_class_get_n_classes ();
  for (i = 0; i < n; i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);
    oil_print_class (klass, 0);
  }
}

int
main (int argc, char *argv[])
{
  oil_init();

  if (argc > 1) {
    int i;
    for (i = 1; i < argc; i++) {
      OilFunctionClass *klass = oil_class_get (argv[i]);
      if (!klass) {
	printerr ("error: no class \"%s\" found\n", argv[i]);
      } else {
	oil_print_class (klass, 1);
      }
    }
  } else {
    oil_print_all ();
  }

  return retval;
}

