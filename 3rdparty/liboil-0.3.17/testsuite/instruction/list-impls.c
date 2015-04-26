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

int
main (int argc, char *argv[])
{
  int n;
  int i;

  oil_init();

  n = oil_class_get_n_classes ();
  for (i = 0; i < n; i++){
    OilFunctionClass *klass = oil_class_get_by_index (i);
    OilFunctionImpl *impl;
    
    for (impl = klass->first_impl; impl; impl = impl->next) {
      unsigned int flags = impl->flags;

      printf("%s:", impl->name);
      if (flags & OIL_IMPL_FLAG_SSE3) {
        flags |= OIL_IMPL_FLAG_SSE2;
      }
      if (flags & OIL_IMPL_FLAG_SSE2) {
        flags |= OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_MMXEXT;
      }
      if (flags & OIL_IMPL_FLAG_3DNOW) printf(" 3dnow");
      if (flags & OIL_IMPL_FLAG_3DNOWEXT) printf(" 3dnowext");
      if (flags & OIL_IMPL_FLAG_CMOV) printf(" cmov");
      if (flags & OIL_IMPL_FLAG_MMX) printf(" mmx");
      if (flags & OIL_IMPL_FLAG_MMXEXT) printf(" mmxext");
      if (flags & OIL_IMPL_FLAG_SSE) printf(" sse");
      if (flags & OIL_IMPL_FLAG_SSE2) printf(" sse2");
      if (flags & OIL_IMPL_FLAG_SSE3) printf(" sse3");
      if (flags & OIL_IMPL_FLAG_SSSE3) printf(" ssse3");
      if (flags & OIL_IMPL_FLAG_ALTIVEC) printf(" altivec");
      printf("\n");
    }
  }

  return 0;
}

