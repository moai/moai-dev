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

char *
string_append (char *str, const char *append)
{
  char *ret;
  if (str) {
    size_t size = strlen (str) + 2 + strlen (append) + 1;
    ret = malloc (size);
    snprintf (ret, size, "%s %s", str, append);
    free (str);
  } else {
    ret = strdup (append);
  }
  return ret;
}

static char *
oil_cpu_flags_to_string (unsigned int flags)
{
  char *ret = NULL;
  
#if defined(__i386__) || defined(__amd64__)
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
  if (flags & OIL_IMPL_FLAG_SSE3) 
    ret = string_append (ret, "sse3");
#endif
#if defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
  if (flags & OIL_IMPL_FLAG_ALTIVEC) 
    ret = string_append (ret, "altivec");
#endif
#if defined(__arm__)
  if (flags & OIL_IMPL_FLAG_EDSP) 
    ret = string_append (ret, "edsp");
  if (flags & OIL_IMPL_FLAG_ARM6) 
    ret = string_append (ret, "arm6");
  if (flags & OIL_IMPL_FLAG_VFP) 
    ret = string_append (ret, "vfp");
#endif
  if (ret == NULL) {
    ret = strdup ("");
  }
  return ret;
}

int
main (int argc, char *argv[])
{
  char *flagstr;

  oil_init();

  flagstr = oil_cpu_flags_to_string(oil_cpu_get_flags());
  printf("CPU flags: %s\n", flagstr);
  free(flagstr);

  return 0;
}

