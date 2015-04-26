/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2006 David A. Schleef <ds@schleef.org>
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
#include <config.h>
#endif

#include <stdio.h>
#include <math.h>
#include <liboil/liboil.h>

#include <liboil/liboilprototype.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilcpu.h>
#include <liboil/liboilrandom.h>

#define OFFSET 100

void mmx_engine_test(void);
void sse2_engine_test(void);

int main (int argc, char *argv[])
{
  uint32_t cpu_flags;

  oil_init ();

  cpu_flags = oil_cpu_get_flags ();

#ifdef HAVE_GCC_ASM
#if defined(HAVE_I386) || defined(HAVE_AMD64)
  if (cpu_flags & OIL_IMPL_FLAG_MMX) {
    mmx_engine_test();
  } else {
    printf("No MMX.\n");
  }
  if (cpu_flags & OIL_IMPL_FLAG_SSE2) {
    sse2_engine_test();
  } else {
    printf("No SSE2.\n");
  }
#endif
#endif

  return 0;
}

int sci_sprint_alt(char *s,double x,double y)
{
  int errsig;
  int maxsig;
  int sigfigs;
  double mantissa;
  double error;
  double mindigit;

  errsig = floor(log10(fabs(y)));
  maxsig = floor(log10(fabs(x)));
  mindigit = pow(10,errsig);

  if(maxsig<errsig)maxsig=errsig;

  sigfigs = maxsig-errsig+2;

  mantissa = x*pow(10,-maxsig);
  error = y*pow(10,-errsig+1);

  if(isnan(x)){
    return sprintf(s,"%g",x);
  }
  if(errsig==1 && maxsig<4 && maxsig>1){
    return sprintf(s,"%0.0f(%2.0f)",x,error);
  }
  if(maxsig<=0 && maxsig>=-2){
    return sprintf(s,"%0.*f(%2.0f)",sigfigs-1-maxsig,
        mantissa*pow(10,maxsig),error);
  }
  return sprintf(s,"%0.*f(%2.0f)e%d",sigfigs-1,mantissa,error,maxsig);
}


#ifdef HAVE_GCC_ASM
#if defined(HAVE_I386) || defined(HAVE_AMD64)
void mmx_engine_test(void)
{
  OilProfile prof;
  double ave, std;
  char s[40];
  int i;

#define CHECK_LATENCY(insn) \
  oil_profile_init (&prof); \
  for(i=0;i<10;i++) { \
    oil_profile_start(&prof); \
    asm volatile ( \
        "  mov $1000, %%ecx\n" \
        ".p2align 4,,15\n" \
        "1:\n" \
        "  " #insn " %%mm0, %%mm1\n" \
        "  " #insn " %%mm1, %%mm2\n" \
        "  " #insn " %%mm2, %%mm3\n" \
        "  " #insn " %%mm3, %%mm4\n" \
        "  " #insn " %%mm4, %%mm5\n" \
        "  " #insn " %%mm5, %%mm6\n" \
        "  " #insn " %%mm6, %%mm7\n" \
        "  " #insn " %%mm7, %%mm0\n" \
        "  " #insn " %%mm0, %%mm1\n" \
        "  " #insn " %%mm1, %%mm2\n" \
        "  " #insn " %%mm2, %%mm3\n" \
        "  " #insn " %%mm3, %%mm4\n" \
        "  " #insn " %%mm4, %%mm5\n" \
        "  " #insn " %%mm5, %%mm6\n" \
        "  " #insn " %%mm6, %%mm7\n" \
        "  " #insn " %%mm7, %%mm0\n" \
        "  " #insn " %%mm0, %%mm1\n" \
        "  " #insn " %%mm1, %%mm2\n" \
        "  " #insn " %%mm2, %%mm3\n" \
        "  " #insn " %%mm3, %%mm4\n" \
        "  " #insn " %%mm4, %%mm5\n" \
        "  " #insn " %%mm5, %%mm6\n" \
        "  " #insn " %%mm6, %%mm7\n" \
        "  " #insn " %%mm7, %%mm0\n" \
        "  decl %%ecx\n" \
        "  jne 1b\n" \
        "  emms\n" \
        :::"ecx"); \
    oil_profile_stop(&prof); \
  } \
  oil_profile_get_ave_std(&prof, &ave, &std); \
  ave -= OFFSET; \
  sci_sprint_alt(s,ave/24000,std/24000); \
  printf("latency of " #insn ": %s\n", s);

  CHECK_LATENCY(packssdw)
  CHECK_LATENCY(packsswb)
  CHECK_LATENCY(packuswb)
  CHECK_LATENCY(paddb)
  CHECK_LATENCY(paddd)
  CHECK_LATENCY(paddsb)
  CHECK_LATENCY(paddsw)
  CHECK_LATENCY(paddusb)
  CHECK_LATENCY(paddusw)
  CHECK_LATENCY(paddw)
  CHECK_LATENCY(pand)
  CHECK_LATENCY(pandn)
  CHECK_LATENCY(pcmpeqb)
  CHECK_LATENCY(pcmpeqd)
  CHECK_LATENCY(pcmpeqw)
  CHECK_LATENCY(pcmpgtb)
  CHECK_LATENCY(pcmpgtd)
  CHECK_LATENCY(pcmpgtw)
  CHECK_LATENCY(pmaddwd)
  CHECK_LATENCY(pmulhw)
  CHECK_LATENCY(pmullw)
  //CHECK_LATENCY(pmulhuw)
  CHECK_LATENCY(por)
  CHECK_LATENCY(pslld)
  CHECK_LATENCY(psllq)
  CHECK_LATENCY(psllw)
  CHECK_LATENCY(psrad)
  CHECK_LATENCY(psraw)
  CHECK_LATENCY(psrld)
  CHECK_LATENCY(psrlq)
  CHECK_LATENCY(psrlw)
  CHECK_LATENCY(psubb)
  CHECK_LATENCY(psubd)
  CHECK_LATENCY(psubsb)
  CHECK_LATENCY(psubsw)
  CHECK_LATENCY(psubusb)
  CHECK_LATENCY(psubusw)
  CHECK_LATENCY(psubw)
  CHECK_LATENCY(punpckhbw)
  CHECK_LATENCY(punpckhdq)
  CHECK_LATENCY(punpckhwd)
  CHECK_LATENCY(punpcklbw)
  CHECK_LATENCY(punpckldq)
  CHECK_LATENCY(punpcklwd)
  CHECK_LATENCY(pxor)
  //CHECK_LATENCY(psadbw)

#define CHECK_THROUGHPUT(insn) \
  oil_profile_init (&prof); \
  for(i=0;i<10;i++) { \
    oil_profile_start(&prof); \
    asm volatile ( \
        "  mov $1000, %%ecx\n" \
        ".p2align 4,,15\n" \
        "1:\n" \
        "  " #insn " %%mm0, %%mm0\n" \
        "  " #insn " %%mm1, %%mm1\n" \
        "  " #insn " %%mm2, %%mm2\n" \
        "  " #insn " %%mm3, %%mm3\n" \
        "  " #insn " %%mm4, %%mm4\n" \
        "  " #insn " %%mm5, %%mm5\n" \
        "  " #insn " %%mm6, %%mm6\n" \
        "  " #insn " %%mm7, %%mm7\n" \
        "  " #insn " %%mm0, %%mm0\n" \
        "  " #insn " %%mm1, %%mm1\n" \
        "  " #insn " %%mm2, %%mm2\n" \
        "  " #insn " %%mm3, %%mm3\n" \
        "  " #insn " %%mm4, %%mm4\n" \
        "  " #insn " %%mm5, %%mm5\n" \
        "  " #insn " %%mm6, %%mm6\n" \
        "  " #insn " %%mm7, %%mm7\n" \
        "  " #insn " %%mm0, %%mm0\n" \
        "  " #insn " %%mm1, %%mm1\n" \
        "  " #insn " %%mm2, %%mm2\n" \
        "  " #insn " %%mm3, %%mm3\n" \
        "  " #insn " %%mm4, %%mm4\n" \
        "  " #insn " %%mm5, %%mm5\n" \
        "  " #insn " %%mm6, %%mm6\n" \
        "  " #insn " %%mm7, %%mm7\n" \
        "  decl %%ecx\n" \
        "  jne 1b\n" \
        "  emms\n" \
        :::"ecx"); \
    oil_profile_stop(&prof); \
  } \
  oil_profile_get_ave_std(&prof, &ave, &std); \
  ave -= OFFSET; \
  sci_sprint_alt(s,ave/24000,std/24000); \
  printf("throughput of " #insn ": %s\n", s);

  CHECK_THROUGHPUT(packssdw)
  CHECK_THROUGHPUT(packsswb)
  CHECK_THROUGHPUT(packuswb)
  CHECK_THROUGHPUT(paddb)
  CHECK_THROUGHPUT(paddd)
  CHECK_THROUGHPUT(paddsb)
  CHECK_THROUGHPUT(paddsw)
  CHECK_THROUGHPUT(paddusb)
  CHECK_THROUGHPUT(paddusw)
  CHECK_THROUGHPUT(paddw)
  CHECK_THROUGHPUT(pand)
  CHECK_THROUGHPUT(pandn)
  CHECK_THROUGHPUT(pcmpeqb)
  CHECK_THROUGHPUT(pcmpeqd)
  CHECK_THROUGHPUT(pcmpeqw)
  CHECK_THROUGHPUT(pcmpgtb)
  CHECK_THROUGHPUT(pcmpgtd)
  CHECK_THROUGHPUT(pcmpgtw)
  CHECK_THROUGHPUT(pmaddwd)
  CHECK_THROUGHPUT(pmulhw)
  CHECK_THROUGHPUT(pmullw)
#if 0
  /* pmulhuw requires mmxext */
  CHECK_THROUGHPUT(pmulhuw)
#endif
  CHECK_THROUGHPUT(por)
  CHECK_THROUGHPUT(pslld)
  CHECK_THROUGHPUT(psllq)
  CHECK_THROUGHPUT(psllw)
  CHECK_THROUGHPUT(psrad)
  CHECK_THROUGHPUT(psraw)
  CHECK_THROUGHPUT(psrld)
  CHECK_THROUGHPUT(psrlq)
  CHECK_THROUGHPUT(psrlw)
  CHECK_THROUGHPUT(psubb)
  CHECK_THROUGHPUT(psubd)
  CHECK_THROUGHPUT(psubsb)
  CHECK_THROUGHPUT(psubsw)
  CHECK_THROUGHPUT(psubusb)
  CHECK_THROUGHPUT(psubusw)
  CHECK_THROUGHPUT(psubw)
  CHECK_THROUGHPUT(punpckhbw)
  CHECK_THROUGHPUT(punpckhdq)
  CHECK_THROUGHPUT(punpckhwd)
  CHECK_THROUGHPUT(punpcklbw)
  CHECK_THROUGHPUT(punpckldq)
  CHECK_THROUGHPUT(punpcklwd)
  CHECK_THROUGHPUT(pxor)
  //CHECK_THROUGHPUT(psadbw)

#undef CHECK_LATENCY
#undef CHECK_THROUGHPUT
}

void sse2_engine_test(void)
{
  OilProfile prof;
  double ave, std;
  char s[40];
  int i;

#define CHECK_LATENCY(insn) \
  oil_profile_init (&prof); \
  for(i=0;i<10;i++) { \
    oil_profile_start(&prof); \
    asm volatile ( \
        "  mov $1000, %%ecx\n" \
        ".p2align 4,,15\n" \
        "1:\n" \
        "  " #insn " %%xmm0, %%xmm1\n" \
        "  " #insn " %%xmm1, %%xmm2\n" \
        "  " #insn " %%xmm2, %%xmm3\n" \
        "  " #insn " %%xmm3, %%xmm4\n" \
        "  " #insn " %%xmm4, %%xmm5\n" \
        "  " #insn " %%xmm5, %%xmm6\n" \
        "  " #insn " %%xmm6, %%xmm7\n" \
        "  " #insn " %%xmm7, %%xmm0\n" \
        "  " #insn " %%xmm0, %%xmm1\n" \
        "  " #insn " %%xmm1, %%xmm2\n" \
        "  " #insn " %%xmm2, %%xmm3\n" \
        "  " #insn " %%xmm3, %%xmm4\n" \
        "  " #insn " %%xmm4, %%xmm5\n" \
        "  " #insn " %%xmm5, %%xmm6\n" \
        "  " #insn " %%xmm6, %%xmm7\n" \
        "  " #insn " %%xmm7, %%xmm0\n" \
        "  " #insn " %%xmm0, %%xmm1\n" \
        "  " #insn " %%xmm1, %%xmm2\n" \
        "  " #insn " %%xmm2, %%xmm3\n" \
        "  " #insn " %%xmm3, %%xmm4\n" \
        "  " #insn " %%xmm4, %%xmm5\n" \
        "  " #insn " %%xmm5, %%xmm6\n" \
        "  " #insn " %%xmm6, %%xmm7\n" \
        "  " #insn " %%xmm7, %%xmm0\n" \
        "  decl %%ecx\n" \
        "  jne 1b\n" \
        :::"ecx"); \
    oil_profile_stop(&prof); \
  } \
  oil_profile_get_ave_std(&prof, &ave, &std); \
  ave -= OFFSET; \
  sci_sprint_alt(s,ave/24000,std/24000); \
  printf("latency of " #insn ": %s\n", s);

  CHECK_LATENCY(packssdw)
  CHECK_LATENCY(packsswb)
  CHECK_LATENCY(packuswb)
  CHECK_LATENCY(paddb)
  CHECK_LATENCY(paddd)
  CHECK_LATENCY(paddsb)
  CHECK_LATENCY(paddsw)
  CHECK_LATENCY(paddusb)
  CHECK_LATENCY(paddusw)
  CHECK_LATENCY(paddw)
  CHECK_LATENCY(pand)
  CHECK_LATENCY(pandn)
  CHECK_LATENCY(pcmpeqb)
  CHECK_LATENCY(pcmpeqd)
  CHECK_LATENCY(pcmpeqw)
  CHECK_LATENCY(pcmpgtb)
  CHECK_LATENCY(pcmpgtd)
  CHECK_LATENCY(pcmpgtw)
  CHECK_LATENCY(pmaddwd)
  CHECK_LATENCY(pmulhw)
  CHECK_LATENCY(pmullw)
#if 0
  /* pmulhuw requires mmxext */
  CHECK_LATENCY(pmulhuw)
#endif
  CHECK_LATENCY(por)
  CHECK_LATENCY(pslld)
  CHECK_LATENCY(psllq)
  CHECK_LATENCY(psllw)
  CHECK_LATENCY(psrad)
  CHECK_LATENCY(psraw)
  CHECK_LATENCY(psrld)
  CHECK_LATENCY(psrlq)
  CHECK_LATENCY(psrlw)
  CHECK_LATENCY(psubb)
  CHECK_LATENCY(psubd)
  CHECK_LATENCY(psubsb)
  CHECK_LATENCY(psubsw)
  CHECK_LATENCY(psubusb)
  CHECK_LATENCY(psubusw)
  CHECK_LATENCY(psubw)
  CHECK_LATENCY(punpckhbw)
  CHECK_LATENCY(punpckhdq)
  CHECK_LATENCY(punpckhwd)
  CHECK_LATENCY(punpcklbw)
  CHECK_LATENCY(punpckldq)
  CHECK_LATENCY(punpcklwd)
  CHECK_LATENCY(pxor)

#define CHECK_THROUGHPUT(insn) \
  oil_profile_init (&prof); \
  for(i=0;i<10;i++) { \
    oil_profile_start(&prof); \
    asm volatile ( \
        "  mov $1000, %%ecx\n" \
        ".p2align 4,,15\n" \
        "1:\n" \
        "  " #insn " %%xmm0, %%xmm0\n" \
        "  " #insn " %%xmm1, %%xmm1\n" \
        "  " #insn " %%xmm2, %%xmm2\n" \
        "  " #insn " %%xmm3, %%xmm3\n" \
        "  " #insn " %%xmm4, %%xmm4\n" \
        "  " #insn " %%xmm5, %%xmm5\n" \
        "  " #insn " %%xmm6, %%xmm6\n" \
        "  " #insn " %%xmm7, %%xmm7\n" \
        "  " #insn " %%xmm0, %%xmm0\n" \
        "  " #insn " %%xmm1, %%xmm1\n" \
        "  " #insn " %%xmm2, %%xmm2\n" \
        "  " #insn " %%xmm3, %%xmm3\n" \
        "  " #insn " %%xmm4, %%xmm4\n" \
        "  " #insn " %%xmm5, %%xmm5\n" \
        "  " #insn " %%xmm6, %%xmm6\n" \
        "  " #insn " %%xmm7, %%xmm7\n" \
        "  " #insn " %%xmm0, %%xmm0\n" \
        "  " #insn " %%xmm1, %%xmm1\n" \
        "  " #insn " %%xmm2, %%xmm2\n" \
        "  " #insn " %%xmm3, %%xmm3\n" \
        "  " #insn " %%xmm4, %%xmm4\n" \
        "  " #insn " %%xmm5, %%xmm5\n" \
        "  " #insn " %%xmm6, %%xmm6\n" \
        "  " #insn " %%xmm7, %%xmm7\n" \
        "  decl %%ecx\n" \
        "  jne 1b\n" \
        :::"ecx"); \
    oil_profile_stop(&prof); \
  } \
  oil_profile_get_ave_std(&prof, &ave, &std); \
  ave -= OFFSET; \
  sci_sprint_alt(s,ave/24000,std/24000); \
  printf("throughput of " #insn ": %s\n", s);

  CHECK_THROUGHPUT(packssdw)
  CHECK_THROUGHPUT(packsswb)
  CHECK_THROUGHPUT(packuswb)
  CHECK_THROUGHPUT(paddb)
  CHECK_THROUGHPUT(paddd)
  CHECK_THROUGHPUT(paddsb)
  CHECK_THROUGHPUT(paddsw)
  CHECK_THROUGHPUT(paddusb)
  CHECK_THROUGHPUT(paddusw)
  CHECK_THROUGHPUT(paddw)
  CHECK_THROUGHPUT(pand)
  CHECK_THROUGHPUT(pandn)
  CHECK_THROUGHPUT(pcmpeqb)
  CHECK_THROUGHPUT(pcmpeqd)
  CHECK_THROUGHPUT(pcmpeqw)
  CHECK_THROUGHPUT(pcmpgtb)
  CHECK_THROUGHPUT(pcmpgtd)
  CHECK_THROUGHPUT(pcmpgtw)
  CHECK_THROUGHPUT(pmaddwd)
  CHECK_THROUGHPUT(pmulhw)
  CHECK_THROUGHPUT(pmullw)
  CHECK_THROUGHPUT(pmulhuw)
  CHECK_THROUGHPUT(por)
  CHECK_THROUGHPUT(pslld)
  CHECK_THROUGHPUT(psllq)
  CHECK_THROUGHPUT(psllw)
  CHECK_THROUGHPUT(psrad)
  CHECK_THROUGHPUT(psraw)
  CHECK_THROUGHPUT(psrld)
  CHECK_THROUGHPUT(psrlq)
  CHECK_THROUGHPUT(psrlw)
  CHECK_THROUGHPUT(psubb)
  CHECK_THROUGHPUT(psubd)
  CHECK_THROUGHPUT(psubsb)
  CHECK_THROUGHPUT(psubsw)
  CHECK_THROUGHPUT(psubusb)
  CHECK_THROUGHPUT(psubusw)
  CHECK_THROUGHPUT(psubw)
  CHECK_THROUGHPUT(punpckhbw)
  CHECK_THROUGHPUT(punpckhdq)
  CHECK_THROUGHPUT(punpckhwd)
  CHECK_THROUGHPUT(punpcklbw)
  CHECK_THROUGHPUT(punpckldq)
  CHECK_THROUGHPUT(punpcklwd)
  CHECK_THROUGHPUT(pxor)
}
#endif
#endif


