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

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifdef __sun
#include <sys/auxv.h>
#endif



/**
 * SECTION:liboilcpu
 * @title: CPU
 * @short_description: Check the capabilities of the current CPU
 *
 */

void oil_cpu_detect_arch(void);

unsigned long oil_cpu_flags;

extern unsigned long (*_oil_profile_stamp)(void);

#ifdef HAVE_GETTIMEOFDAY
static unsigned long
oil_profile_stamp_gtod (void)
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return 1000000*(unsigned long)tv.tv_sec + (unsigned long)tv.tv_usec;
}
#endif

#if defined(HAVE_CLOCK_GETTIME) && defined(HAVE_MONOTONIC_CLOCK)
static unsigned long
oil_profile_stamp_clock_gettime (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC, &ts);
  return 1000000000*ts.tv_sec + ts.tv_nsec;
}
#endif

static unsigned long
oil_profile_stamp_zero (void)
{
  return 0;
}

void
_oil_cpu_init (void)
{
  const char *envvar;

  OIL_INFO ("checking architecture");
  oil_cpu_detect_arch();

  envvar = getenv ("OIL_CPU_FLAGS");
  if (envvar != NULL) {
    char *end = NULL;
    unsigned long flags;

    flags = strtoul (envvar, &end, 0);
    if (end > envvar) {
      oil_cpu_flags = flags;
    }
    OIL_INFO ("cpu flags from environment %08lx", oil_cpu_flags);
  }

  OIL_INFO ("cpu flags %08lx", oil_cpu_flags);

#if defined(HAVE_CLOCK_GETTIME) && defined(HAVE_MONOTONIC_CLOCK)
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_clock_gettime;
    OIL_INFO("Using clock_gettime() as a timestamp function.");
  }
#endif

#ifdef HAVE_GETTIMEOFDAY
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_gtod;
    OIL_WARNING("Using gettimeofday() as a timestamp function.");
  }
#endif
  if (_oil_profile_stamp == NULL) {
    _oil_profile_stamp = oil_profile_stamp_zero;
    OIL_ERROR("No timestamping function.  This is kinda bad.");
  }
}

/**
 * oil_cpu_get_flags:
 *
 * Returns a bitmask containing the available CPU features.
 *
 * Returns: the CPU features.
 */
unsigned int
oil_cpu_get_flags (void)
{
  return oil_cpu_flags;
}


#if 0
/**
 * oil_cpu_get_ticks_per_second:
 *
 * Returns the estimated number of ticks per second.  This feature
 * is currently unimplemented.
 *
 * This function may take several milliseconds or more to execute
 * in order to calculate a good estimate of the number of ticks (as
 * measured by the profiling functions) per second.  Note that the
 * number of ticks per second is often dependent on the CPU frequency,
 * which can change dynamically.  Thus the value returned by this
 * function may be incorrect as soon as it is returned.
 *
 * Returns: a double
 */
double
oil_cpu_get_ticks_per_second (void)
{
  return _oil_ticks_per_second;
}
#endif

double
oil_cpu_get_frequency (void)
{
#if defined(__linux__)
  int freq;
  if (get_file_int ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
        &freq)) {
    return 1000.0 * freq;
  }
  return 0;
#else
  return 0;
#endif
}

