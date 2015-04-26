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

/***** alpha *****/

#if defined(__alpha__)
static unsigned long
oil_profile_stamp_alpha(void)
{
  unsigned int ts;
  __asm__ __volatile__ ("rpcc %0\n" : "=r"(ts));
  return ts;
}

static void
oil_cpu_detect_alpha(void)
{
  _oil_profile_stamp = oil_profile_stamp_alpha;
}
#endif

/***** ia64 *****/

#if defined(__ia64__)
static unsigned long
oil_profile_stamp_ia64(void)
{
  unsigned int ts;
  __asm__ __volatile__("mov %0=ar.itc\n" : "=r"(ts) :: "memory");
  return ts;
}

static void
oil_cpu_detect_ia64(void)
{
  _oil_profile_stamp = oil_profile_stamp_ia64;
}
#endif

/***** s390 *****/

#if defined(__s390__)
static unsigned long
oil_profile_stamp_s390(void)
{
  uint64_t ts;
  __asm__ __volatile__ ("STCK %0(%0)\n" : : "r" (&ts));
  return ts;
}

static void
oil_cpu_detect_s390(void)
{
  _oil_profile_stamp = oil_profile_stamp_s390;
}
#endif

/***** mips *****/

#if defined(__mips__)
#if 0
/* broken */
static unsigned long
oil_profile_stamp_mips(void)
{
	unsigned int ts;
	__asm__ __volatile__ (
		"	.set	push		\n"
		"	.set	reorder		\n"
		"	mfc0	%0,$9		\n"
		"	.set	pop		\n"
	: "=m" (ts));
	return ts;
}
#endif

static void
oil_cpu_detect_mips(void)
{
  //_oil_profile_stamp = oil_profile_stamp_mips;
}
#endif

void
oil_cpu_detect_arch(void)
{
#ifdef __alpha__
  oil_cpu_detect_alpha();
#endif
#ifdef __ia64__
  oil_cpu_detect_ia64();
#endif
#ifdef __s390__
  oil_cpu_detect_s390();
#endif
#ifdef __mips__
  oil_cpu_detect_mips();
#endif
}

