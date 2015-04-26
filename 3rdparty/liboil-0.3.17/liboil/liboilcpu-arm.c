/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilfunction.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilcpu.h>
#include <liboil/liboilfault.h>
#include <liboil/liboilutils.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

/***** arm *****/

#ifdef __arm__
#if 0
static unsigned long
oil_profile_stamp_xscale(void)
{
  unsigned int ts;
  __asm__ __volatile__ (
      "  mrc p14, 0, %0, c1, c0, 0 \n"
      : "=r" (ts));
  return ts;
}
#endif

static void
oil_cpu_arm_getflags_cpuinfo (char *cpuinfo)
{
  char *cpuinfo_flags;
  char **flags;
  char **f;

  cpuinfo_flags = get_cpuinfo_line(cpuinfo, "Features");
  if (cpuinfo_flags == NULL) return;

  flags = strsplit(cpuinfo_flags);
  for (f = flags; *f; f++) {
    if (strcmp (*f, "edsp") == 0) {
      OIL_DEBUG ("cpu feature %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_EDSP;
    }
    if (strcmp (*f, "vfp") == 0) {
      OIL_DEBUG ("cpu feature %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_VFP;
    }

    free (*f);
  }
  free (flags);
  free (cpuinfo_flags);
}

static char *
get_proc_cpuinfo (void)
{
  char *cpuinfo;
  int fd;
  int n;

  cpuinfo = malloc(4096);
  if (cpuinfo == NULL) return NULL;

  fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd < 0) {
    free (cpuinfo);
    return NULL;
  }

  n = read(fd, cpuinfo, 4095);
  if (n < 0) {
    free (cpuinfo);
    close (fd);
    return NULL;
  }
  cpuinfo[n] = 0;

  close (fd);

  return cpuinfo;
}

void
oil_cpu_detect_arch(void)
{
#ifdef __linux__
  int arm_implementer = 0;
  int arm_arch;
  char *cpuinfo;
  char *s;

  cpuinfo = get_proc_cpuinfo();
  if (cpuinfo == NULL) return;

  s = get_cpuinfo_line(cpuinfo, "CPU implementer");
  if (s) {
    arm_implementer = strtoul (s, NULL, 0);
    free(s);
  }

  switch(arm_implementer) {
    case 0x69: /* Intel */
    case 0x41: /* ARM */
      /* ARM chips are known to not have timestamping available from 
       * user space */
      break;
    default:
      break;
  }

  s = get_cpuinfo_line(cpuinfo, "CPU architecture");
  if (s) {
    arm_arch = strtoul (s, NULL, 0);
    if (arm_arch >= 6)
      oil_cpu_flags |= OIL_IMPL_FLAG_ARM6;
    free(s);
  }

  oil_cpu_arm_getflags_cpuinfo (cpuinfo);
  free (cpuinfo);
#endif
}
#endif


