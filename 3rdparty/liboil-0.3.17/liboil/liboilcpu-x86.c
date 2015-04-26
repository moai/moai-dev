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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <time.h>

#if defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef __sun
#include <sys/auxv.h>
#endif

/***** i386, amd64 *****/

#if defined(__sun)
#define USE_I386_GETISAX
#else
#define USE_I386_CPUID
#endif


#ifdef USE_I386_CPUINFO
static void
oil_cpu_i386_getflags_cpuinfo (char *cpuinfo)
{
  char *cpuinfo_flags;
  char **flags;
  char **f;

  cpuinfo_flags = get_tag_value (cpuinfo, "flags");
  if (cpuinfo_flags == NULL) {
    free (cpuinfo);
    return;
  }

  flags = strsplit(cpuinfo_flags);
  for (f = flags; *f; f++) {
    if (strcmp (*f, "cmov") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_CMOV;
    }
    if (strcmp (*f, "mmx") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_MMX;
    }
    if (strcmp (*f, "sse") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE;
    }
    if (strcmp (*f, "mmxext") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (strcmp (*f, "sse2") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (strcmp (*f, "3dnow") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOW;
    }
    if (strcmp (*f, "3dnowext") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOWEXT;
    }
    if (strcmp (*f, "sse3") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE3;
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (strcmp (*f, "ssse3") == 0) {
      OIL_DEBUG ("cpu flag %s", *f);
      oil_cpu_flags |= OIL_IMPL_FLAG_SSSE3;
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE3;
      oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }

    free (*f);
  }
  free (flags);
  free (cpuinfo);
  free (cpuinfo_flags);
}
#endif

#ifdef HAVE_GCC_ASM
static unsigned long
oil_profile_stamp_rdtsc(void)
{
	unsigned long ts;
	__asm__ __volatile__("rdtsc\n" : "=a" (ts) : : "edx");
	return ts;
}
#endif

#ifdef USE_I386_CPUID
#ifdef __i386__
static void
get_cpuid (uint32_t op, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
  __asm__ (
      "  pushl %%ebx\n"
      "  cpuid\n"
      "  mov %%ebx, %%esi\n"
      "  popl %%ebx\n"
      : "=a" (*a), "=S" (*b), "=c" (*c), "=d" (*d)
      : "0" (op));
}
#endif

#ifdef __amd64__
static void
get_cpuid (uint32_t op, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
  __asm__ (
      "  pushq %%rbx\n"
      "  cpuid\n"
      "  mov %%ebx, %%esi\n"
      "  popq %%rbx\n"
      : "=a" (*a), "=S" (*b), "=c" (*c), "=d" (*d)
      : "0" (op));
}
#endif

static void
test_cpuid (void *ignored)
{
  uint32_t eax, ebx, ecx, edx;

  get_cpuid (0x00000000, &eax, &ebx, &ecx, &edx);
}

static void
oil_cpu_detect_cpuid (void)
{
  uint32_t eax, ebx, ecx, edx;
  uint32_t level;
  char vendor[13] = { 0 };
  int ret;

  oil_fault_check_enable ();
  ret = oil_fault_check_try(test_cpuid, NULL);
  oil_fault_check_disable ();
  if (!ret) {
    /* CPU thinks cpuid is an illegal instruction. */
    return;
  }

  get_cpuid (0x00000000, &level, (uint32_t *)(vendor+0),
      (uint32_t *)(vendor+8), (uint32_t *)(vendor+4));

  OIL_DEBUG("cpuid %d %s", level, vendor);

  if (level < 1) {
    return;
  }

  get_cpuid (0x00000001, &eax, &ebx, &ecx, &edx);

#ifdef HAVE_GCC_ASM
  if (edx & (1<<4)) {
    _oil_profile_stamp = oil_profile_stamp_rdtsc;
  }
#endif

  /* Intel flags */
  if (edx & (1<<15)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_CMOV;
  }
  if (edx & (1<<23)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_MMX;
  }
  if (edx & (1<<25)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE;
  }
  if (edx & (1<<26)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
    oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
  }
  if (ecx & (1<<0)) {
    oil_cpu_flags |= OIL_IMPL_FLAG_SSE3;
  }
  
  if (memcmp (vendor, "AuthenticAMD", 12) == 0) {
    get_cpuid (0x80000001, &eax, &ebx, &ecx, &edx);

    /* AMD flags */
    if (edx & (1<<22)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
    }
    if (edx & (1<<31)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOW;
    }
    if (edx & (1<<30)) {
      oil_cpu_flags |= OIL_IMPL_FLAG_3DNOWEXT;
    }

    get_cpuid (0x80000005, &eax, &ebx, &ecx, &edx);

    OIL_INFO("L1 D-cache: %d kbytes, %d-way, %d lines/tag, %d line size",
        (ecx>>24)&0xff, (ecx>>16)&0xff, (ecx>>8)&0xff, ecx&0xff);
    OIL_INFO("L1 I-cache: %d kbytes, %d-way, %d lines/tag, %d line size",
        (edx>>24)&0xff, (edx>>16)&0xff, (edx>>8)&0xff, edx&0xff);

    get_cpuid (0x80000006, &eax, &ebx, &ecx, &edx);
    OIL_INFO("L2 cache: %d kbytes, %d assoc, %d lines/tag, %d line size",
        (ecx>>16)&0xffff, (ecx>>12)&0xf, (ecx>>8)&0xf, ecx&0xff);
  }
}
#endif

#ifdef USE_I386_GETISAX
static void
oil_cpu_detect_getisax (void)
{
  uint_t ui;

  getisax (&ui, 1);

  if (ui & AV_386_CMOV) {
     oil_cpu_flags |= OIL_IMPL_FLAG_CMOV;
  }
  if (ui & AV_386_MMX) {
     oil_cpu_flags |= OIL_IMPL_FLAG_MMX;
  }
  if (ui & AV_386_SSE) {
     oil_cpu_flags |= OIL_IMPL_FLAG_SSE;
  }
  if (ui & AV_386_SSE2) {
     oil_cpu_flags |= OIL_IMPL_FLAG_SSE2;
     oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
  }
  if (ui & AV_386_SSE3) {
     oil_cpu_flags |= OIL_IMPL_FLAG_SSE3;
  }
  if (ui & AV_386_AMD_3DNow) {
    oil_cpu_flags |= OIL_IMPL_FLAG_3DNOW;
  }
  if (ui & AV_386_AMD_3DNowx) {
    oil_cpu_flags |= OIL_IMPL_FLAG_3DNOWEXT;
  }
  if (ui & AV_386_AMD_MMX) {
    oil_cpu_flags |= OIL_IMPL_FLAG_MMXEXT;
  }
}
#endif

/* Reduce the set of CPU capabilities detected by whatever detection mechanism
 * was chosen, according to kernel limitations.  SSE requires kernel support for
 * use.
 */
static void
oil_cpu_detect_kernel_support (void)
{
#if defined(__APPLE__)
  int ret, enabled;
  size_t len;

  len = sizeof(enabled);
  ret = sysctlbyname("hw.optional.sse", &enabled, &len, NULL, 0);
  if (ret || !enabled) {
    oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		       OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
  }
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
  int ret, enabled;
  size_t len;

  len = sizeof(enabled);
  ret = sysctlbyname("hw.instruction_sse", &enabled, &len, NULL, 0);
  if (ret || !enabled) {
    oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		       OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
  }
#elif defined(__linux__)
  /*
   * Might also want to grow a check for the old RedHat + Linux 2.2
   * unmasked SSE FPU exception bug.  Other than that, if /proc/cpuinfo
   * reported SSE, then it's safe.
   */
#elif defined(__sun) || defined(__NetBSD__) || defined(__OpenBSD__)
  /* Solaris/NetBSD/OpenBSD are OK */
#else
   
  OIL_WARNING("Operating system is not known to support SSE.  "
      "Assuming it does, which might cause problems");
#if 0
  oil_cpu_flags &= ~(OIL_IMPL_FLAG_SSE | OIL_IMPL_FLAG_SSE2 |
		     OIL_IMPL_FLAG_MMXEXT | OIL_IMPL_FLAG_SSE3);
#endif
#endif
}

void
oil_cpu_detect_arch(void)
{
#ifdef USE_I386_CPUID
  oil_cpu_detect_cpuid ();
#endif
#ifdef USE_I386_GETISAX
  oil_cpu_detect_getisax ();
#endif
#ifdef USE_I386_CPUINFO
  oil_cpu_detect_cpuinfo ();
#endif

  oil_cpu_detect_kernel_support ();
}


