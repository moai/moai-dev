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
#include <liboil/liboildebug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if 0
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
#ifdef HAVE_CPU_I386
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
#endif
#ifdef HAVE_CPU_PPC
  if (flags & OIL_IMPL_FLAG_ALTIVEC) 
    ret = string_append (ret, "altivec");
#endif
  return ret;
}
#endif

#if 0
static void
oil_print_impl (OilFunctionImpl *impl, OilTest *test, char* prefix)
{
  char *c;
  unsigned int cpu_flags = oil_cpu_get_flags();

  oil_test_check_impl (test, impl);

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
#endif

#if 0
static int
impl_compare (const void *a, const void *b)
{
  const OilFunctionImpl *ia = *(OilFunctionImpl **)a;
  const OilFunctionImpl *ib = *(OilFunctionImpl **)b;

  if (ia->profile_ave < ib->profile_ave) return -1;
  if (ia->profile_ave > ib->profile_ave) return 1;
  return 0;
}
#endif

static int
get_n_impls(OilFunctionClass *klass)
{
  OilFunctionImpl *impl;
  int n;

  n = 0;
  for(impl=klass->first_impl;impl;impl=impl->next)n++;

  return n;
}

static void
oil_print_class (OilFunctionClass *klass, int verbose)
{
  int n_impls;

  n_impls = get_n_impls(klass);
  
  printf ("%-20s %d %-10g %-10g %-10.3g %s %s\n", klass->name,
      n_impls,
      klass->reference_impl->profile_ave,
      klass->chosen_impl->profile_ave,
      klass->reference_impl->profile_ave/klass->chosen_impl->profile_ave,
      (klass->chosen_impl->flags & OIL_IMPL_FLAG_OPT)?"*":" ",
      klass->chosen_impl->name
      );

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

static void
_debug_print_valist (int level, const char *file, const char *func,
        int line, const char *format, va_list args)
{
  static const char *level_names[] = { "NONE", "ERROR", "WARNING", "INFO",
    "DEBUG", "LOG" };
  const char *level_name = "unknown";

  if (level > 3) return;

  if(level>=OIL_DEBUG_NONE && level<=OIL_DEBUG_LOG){
    level_name = level_names[level];
  }

  fprintf (stdout, "OIL: %s %s %d: %s(): ", level_name, file, line, func);
  vfprintf (stdout, format, args);
  fprintf (stdout, "\n");
}

int
main (int argc, char *argv[])
{
  oil_debug_set_print_function (_debug_print_valist);

  oil_init();

  oil_print_all ();

  return 0;
}

