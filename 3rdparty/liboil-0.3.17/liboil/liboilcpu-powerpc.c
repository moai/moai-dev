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

#if defined(__linux__)
#include <linux/auxvec.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#ifndef PPC_FEATURE_HAS_ALTIVEC
/* From linux-2.6/include/asm-powerpc/cputable.h */
#define PPC_FEATURE_HAS_ALTIVEC 0x10000000
#endif

#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__OpenBSD__)
#include <sys/param.h>
#include <sys/sysctl.h>
#include <machine/cpu.h>
#endif

/***** powerpc *****/

static unsigned long
oil_profile_stamp_tb(void)
{
  unsigned long ts;
  __asm__ __volatile__("mftb %0\n" : "=r" (ts));
  return ts;
}

#if !defined(__FreeBSD__) && !defined(__FreeBSD_kernel__) && !defined(__OpenBSD__) && !defined(__APPLE__) && !defined(__linux__)
static void
test_altivec (void * ignored)
{
  asm volatile ("vor v0, v0, v0\n");
}
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
static void
oil_check_altivec_sysctl_freebsd (void)
{
  int ret, av;
  size_t len;

  len = sizeof(av);
  ret = sysctlbyname("hw.altivec", &av, &len, NULL, 0);
  if (!ret && av) {
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
}
#endif

#if defined(__OpenBSD__)
static void
oil_check_altivec_sysctl_openbsd (void)
{
  int mib[2], av, ret;
  size_t len;

  mib[0] = CTL_MACHDEP;
  mib[1] = CPU_ALTIVEC;

  len = sizeof(av);
  ret = sysctl(mib, 2, &av, &len, NULL, 0);
  if (!ret && av) {
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
}
#endif

#if defined(__APPLE__)
static void
oil_check_altivec_sysctl_darwin (void)
{
  int ret, vu;
  size_t len;

  len = sizeof(vu);
  ret = sysctlbyname("hw.vectorunit", &vu, &len, NULL, 0);
  if (!ret && vu) {
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
}
#endif

#if defined(__linux__)
static void
oil_check_altivec_proc_auxv (void)
{
  static int available = -1;
  int new_avail = 0;
  unsigned long buf[64];
  ssize_t count;
  int fd, i;

  /* Flags already set */
  if (available != -1) {
    return;
  }

  fd = open("/proc/self/auxv", O_RDONLY);
  if (fd < 0) {
    goto out;
  }

more:
  count = read(fd, buf, sizeof(buf));
  if (count < 0) {
    goto out_close;
  }

  for (i=0; i < (count / sizeof(unsigned long)); i += 2) {
    if (buf[i] == AT_HWCAP) {
      new_avail = !!(buf[i+1] & PPC_FEATURE_HAS_ALTIVEC);
      goto out_close;
    } else if (buf[i] == AT_NULL) {
      goto out_close;
    }
  }

  if (count == sizeof(buf)) {
    goto more;
  }

out_close:
  close(fd);

out:
  available = new_avail;
  if (available) {
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
}
#endif

#if !defined(__FreeBSD__) && !defined(__FreeBSD_kernel__) && !defined(__OpenBSD__) && !defined(__APPLE__) && !defined(__linux__)
static void
oil_check_altivec_fault (void)
{
  oil_fault_check_enable ();
  if (oil_fault_check_try(test_altivec, NULL)) {
    OIL_DEBUG ("cpu flag altivec");
    oil_cpu_flags |= OIL_IMPL_FLAG_ALTIVEC;
  }
  oil_fault_check_disable ();
}
#endif

void
oil_cpu_detect_arch(void)
{
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
  oil_check_altivec_sysctl_freebsd();
#elif defined(__OpenBSD__)
  oil_check_altivec_sysctl_openbsd();
#elif defined(__APPLE__)
  oil_check_altivec_sysctl_darwin();
#elif defined(__linux__)
  oil_check_altivec_proc_auxv();
#else
  oil_check_altivec_fault();
#endif

  _oil_profile_stamp = oil_profile_stamp_tb;
}



