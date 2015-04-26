
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>
#include <stddef.h>


void
deinterleave2_asm (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  if (n == 0) return;

  while (n&1) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
    d2++;
    s_2xn+=2;
    n--;
  }

  asm volatile ("\n"
      "  sub $2, %%rcx\n"
      "1:\n"
      "  movw (%1,%%rcx,4), %%ax\n"
      "  movw %%ax, (%0,%%rcx,2)\n"
      "  movw 2(%1,%%rcx,4), %%ax\n"
      "  movw %%ax, (%2,%%rcx,2)\n"
      "  movw 4(%1,%%rcx,4), %%ax\n"
      "  movw %%ax, 2(%0,%%rcx,2)\n"
      "  movw 6(%1,%%rcx,4), %%ax\n"
      "  movw %%ax, 2(%2,%%rcx,2)\n"
      "  sub $2, %%rcx\n"
      "  jge 1b\n"
      : "+r" (d1), "+r" (s_2xn), "+r" (d2), "+c" (n)
      :
      : "rax");
}
OIL_DEFINE_IMPL (deinterleave2_asm, deinterleave2_s16);

void
deinterleave2_mmx (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%1,%%rcx,4), %%mm0\n"
      "  movq 8(%1,%%rcx,4), %%mm1\n"
      "  pslld $16, %%mm0\n"
      "  pslld $16, %%mm1\n"
      "  psrad $16, %%mm0\n"
      "  psrad $16, %%mm1\n"
      "  packssdw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%rcx,2)\n"
      "  movq (%1,%%rcx,4), %%mm0\n"
      "  movq 8(%1,%%rcx,4), %%mm1\n"
      "  psrad $16, %%mm0\n"
      "  psrad $16, %%mm1\n"
      "  packssdw %%mm1, %%mm0\n"
      "  movq %%mm0, (%2,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "m" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx, deinterleave2_s16, OIL_IMPL_FLAG_MMX);

void
deinterleave2_mmx_2 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  pshufw $0xd8, (%1,%%rcx,4), %%mm0\n"
      "  movd %%mm0, (%0,%%rcx,2)\n"
      "  pshufw $0x8d, (%1,%%rcx,4), %%mm0\n"
      "  movd %%mm0, (%2,%%rcx,2)\n"
      "  add $2, %%rcx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_2, deinterleave2_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
deinterleave2_mmx_3 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&3) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%1,%%rcx,4), %%mm1\n"
      "  movq (%1,%%rcx,4), %%mm2\n"
      "  movq 8(%1,%%rcx,4), %%mm0\n"
      "  punpcklwd %%mm0, %%mm1\n"
      "  punpckhwd %%mm0, %%mm2\n"
      "  movq %%mm1, %%mm0\n"
      "  punpcklwd %%mm2, %%mm0\n"
      "  punpckhwd %%mm2, %%mm1\n"
      "  movq %%mm0, (%0,%%rcx,2)\n"
      "  movq %%mm1, (%2,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_3, deinterleave2_s16, OIL_IMPL_FLAG_MMX);

void
deinterleave2_mmx_4 (int16_t *d1, int16_t *d2, int16_t *s_2xn, int n)
{
  while (n&7) {
    d1[0] = s_2xn[0];
    d2[0] = s_2xn[1];
    d1++;
    d2++;
    s_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%1,%%rcx,4), %%mm1\n"
      "  movq %%mm1, %%mm2\n"
      "  movq 8(%1,%%rcx,4), %%mm0\n"
      "   movq 16(%1,%%rcx,4), %%mm5\n"
      "  punpcklwd %%mm0, %%mm1\n"
      "   movq %%mm5, %%mm6\n"
      "  punpckhwd %%mm0, %%mm2\n"
      "   movq 24(%1,%%rcx,4), %%mm4\n"
      "  movq %%mm1, %%mm0\n"
      "   punpcklwd %%mm4, %%mm5\n"
      "  punpcklwd %%mm2, %%mm0\n"
      "   punpckhwd %%mm4, %%mm6\n"
      "  punpckhwd %%mm2, %%mm1\n"
      "   movq %%mm5, %%mm4\n"
      "  movq %%mm0, (%0,%%rcx,2)\n"
      "   punpcklwd %%mm6, %%mm4\n"
      "  movq %%mm1, (%2,%%rcx,2)\n"
      "   punpckhwd %%mm6, %%mm5\n"
      "   movq %%mm4, 8(%0,%%rcx,2)\n"
      "   movq %%mm5, 8(%2,%%rcx,2)\n"
      "  add $8, %%rcx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s_2xn), "+r" (d2)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (deinterleave2_mmx_4, deinterleave2_s16, OIL_IMPL_FLAG_MMX);


void
lift_add_mult_shift12_i386_mmx (int16_t *d, int16_t *s1, int16_t *s2,
    int16_t *s3, int16_t *s4, int n)
{
  uint32_t val = *s4;

  while (n&3) {
    d[0] = s1[0] + ((s4[0]*(s2[0] + s3[0]))>>12);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  val = ((*(uint16_t *)s4)<<16) | (*(uint16_t *)s4);
  n>>=2;
  asm volatile ("\n"
      "  mov %5, %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  punpcklwd %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  paddsw 0(%3), %%mm0\n"
      "  movq %%mm0, %%mm1\n"
      "  pmullw %%mm7, %%mm0\n"
      "  pmulhw %%mm7, %%mm1\n"
      "  psrlw $12, %%mm0\n"
      "  psllw $4, %%mm1\n"
      "  por %%mm1, %%mm0\n"
      "  paddsw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  add $8, %3\n"
      "  decl %4\n"
      "  jne 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3), "+r" (n)
      : "r" (val)
      : "ecx");
}
OIL_DEFINE_IMPL (lift_add_mult_shift12_i386_mmx, lift_add_mult_shift12);

void
interleave2_mmx (int16_t *d_2xn, int16_t *s1, int16_t *s2, int n)
{
  while (n&3) {
    d_2xn[0] = s1[0];
    d_2xn[1] = s2[0];
    s1++;
    s2++;
    d_2xn+=2;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%1,%%rcx,2), %%mm0\n"
      "  movq (%2,%%rcx,2), %%mm1\n"
      "  movq %%mm0, %%mm2\n"
      "  punpckhwd %%mm1, %%mm0\n"
      "  punpcklwd %%mm1, %%mm2\n"
      "  movq %%mm2, (%0,%%rcx,4)\n"
      "  movq %%mm0, 8(%0,%%rcx,4)\n"
      "  add $4, %%rcx\n"
      "  cmp %3, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d_2xn), "+r" (s1), "+r" (s2)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (interleave2_mmx, interleave2_s16, OIL_IMPL_FLAG_MMX);

void
lift_add_shift1_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] + ((s2[0] + s3[0])>>1);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%2,%%rcx,2), %%mm1\n"
      "  movq (%3,%%rcx,2), %%mm2\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $1, %%mm1\n"
      "  paddw (%1,%%rcx,2), %%mm1\n"
      "  movq %%mm1, (%0,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (lift_add_shift1_mmx, lift_add_shift1, OIL_IMPL_FLAG_MMX);

void
lift_sub_shift1_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] - ((s2[0] + s3[0])>>1);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%2,%%rcx,2), %%mm1\n"
      "  movq (%3,%%rcx,2), %%mm2\n"
      "  movq (%1,%%rcx,2), %%mm0\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $1, %%mm1\n"
      "  psubw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "r" (n)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lift_sub_shift1_mmx, lift_sub_shift1, OIL_IMPL_FLAG_MMX);

void
lift_add_shift2_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] + ((s2[0] + s3[0])>>2);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%2,%%rcx,2), %%mm1\n"
      "  movq (%3,%%rcx,2), %%mm2\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $2, %%mm1\n"
      "  paddw (%1,%%rcx,2), %%mm1\n"
      "  movq %%mm1, (%0,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (lift_add_shift2_mmx, lift_add_shift2, OIL_IMPL_FLAG_MMX);

void
lift_sub_shift2_mmx (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while (n&3) {
    d[0] = s1[0] - ((s2[0] + s3[0])>>2);
    d++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  asm volatile ("\n"
      "  xor %%rcx, %%rcx\n"
      "1:\n"
      "  movq (%2,%%rcx,2), %%mm1\n"
      "  movq (%3,%%rcx,2), %%mm2\n"
      "  movq (%1,%%rcx,2), %%mm0\n"
      "  paddw %%mm2, %%mm1\n"
      "  psraw $2, %%mm1\n"
      "  psubw %%mm1, %%mm0\n"
      "  movq %%mm0, (%0,%%rcx,2)\n"
      "  add $4, %%rcx\n"
      "  cmp %4, %%ecx\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (d), "+r" (s1), "+r" (s2), "+r" (s3)
      : "r" (n)
      : "rcx");
}
OIL_DEFINE_IMPL_FULL (lift_sub_shift2_mmx, lift_sub_shift2, OIL_IMPL_FLAG_MMX);

#if 0
#ifdef ENABLE_BROKEN_IMPLS
void
synth_53_mmx (int16_t *d_2xn, int16_t *s_2xn, int n)
{
  int i;
    
  if (n==0) return;
  if (n == 1) {
    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);
    d_2xn[1] = s_2xn[1] + d_2xn[0]; 
  } else {
    int i;

    d_2xn[0] = s_2xn[0] - (s_2xn[1] >> 1);

    if (n > 6) {
      n-=5;

      asm volatile ("\n"
          "  xor %%ecx, %%ecx\n"
          "  movw 2(%1), %%ecx\n"
          "  movd %%ecx, %%mm7\n"
          "  movw 0(%0), %%ecx\n"
          "  movd %%ecx, %%mm6\n"
          "  movw 0(%1), %%ecx\n"
          "  movd %%ecx, %%mm5\n"

          "  xor %%ecx, %%ecx\n"
          "1:\n"
          "  movq 4(%1,%%ecx,4), %%mm1\n"  // mm1 = s5 s4 s3 s2
          "  movq %%mm1, %%mm2\n"          // mm2 = s5 s4 s3 s2
          "  movq 12(%1,%%ecx,4), %%mm0\n" // mm0 = s9 s8 s7 s6
          "  punpcklwd %%mm0, %%mm1\n"     // mm1 = s7 s3 s6 s2
          "  punpckhwd %%mm0, %%mm2\n"     // mm2 = s9 s5 s8 s4
          "  movq %%mm1, %%mm0\n"          // mm0 = s7 s3 s6 s2
          "  punpcklwd %%mm2, %%mm0\n"     // mm0 = s8 s6 s4 s2
          "  punpckhwd %%mm2, %%mm1\n"     // mm1 = s9 s7 s5 s3
          //"  movq %%mm0, %%mm3\n"          // mm0 = s8 s6 s4 s2

          "  movq %%mm1, %%mm2\n"          // mm2 = s9 s7 s5 s3
          "  psllq $16, %%mm2\n"           // mm2 = s7 s5 s3 00
          "  por %%mm7, %%mm2\n"           // mm2 = s7 s5 s3 s1
          "  movq %%mm2, %%mm4\n"          // mm4 = s7 s5 s3 s1
          "  paddw %%mm1, %%mm2\n"         // mm2 = s9+s7 ...
          "  psraw $2, %%mm2\n"            // mm2 = (s9+s7)>>2 ...
          "  movq %%mm1, %%mm7\n"          // mm7 = s9 s7 s5 s3
          "  psrlq $48, %%mm7\n"           // mm7 = 00 00 00 s9
          "  psubw %%mm2, %%mm0\n"         // mm0 = d8 d6 d4 d2

          "  movq %%mm0, %%mm1\n"          // mm1 = d8 d6 d4 d2
          "  movq %%mm0, %%mm3\n"          // mm1 = d8 d6 d4 d2
          "  psllq $16, %%mm0\n"           // mm0 = d6 d4 d2 00
          "  por %%mm6, %%mm0\n"           // mm0 = d6 d4 d2 d0
          "  psrlq $48, %%mm1\n"           // mm1 = 00 00 00 d8
          "  movq %%mm1, %%mm6\n"          // mm6 = 00 00 00 d8

          "  movq %%mm0, %%mm1\n"
          "  paddw %%mm3, %%mm1\n"         // mm0 = d8+d6 ...
          "  psraw $1, %%mm1\n"            // mm1 = (d8+d6)>>1 ...
          "  paddw %%mm4, %%mm1\n"         // mm1 = d7 d5 d3 d1

          "  movq %%mm1, %%mm2\n"

          "  movq %%mm0, %%mm1\n"
          "  punpcklwd %%mm2, %%mm0\n"
          "  punpckhwd %%mm2, %%mm1\n"

          "  movq %%mm0, (%0, %%ecx, 4)\n"
          "  movq %%mm1, 8(%0, %%ecx, 4)\n"

          "  add $4, %%ecx\n"
          "  cmp %3, %%ecx\n"
          "  jl 1b\n"
          "  emms\n"
          : "+r" (d_2xn), "+r" (s_2xn), "+ecx" (i)
          : "m" (n));

      i*=2;
      n+=5;
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
      i+=2;
    } else {
      i = 2;
    }
    for(;i<n*2-2;i+=2){
      d_2xn[i] = s_2xn[i] - ((s_2xn[i-1] + s_2xn[i+1]) >> 2);
      d_2xn[i-1] = s_2xn[i-1] + ((d_2xn[i] + d_2xn[i-2]) >> 1);
    }
    d_2xn[n*2-2] = s_2xn[n*2-2] - ((s_2xn[n*2-3] + s_2xn[n*2-1]) >> 2);
    d_2xn[n*2-3] = s_2xn[n*2-3] + ((d_2xn[n*2-2] + d_2xn[n*2-4]) >> 1);
    d_2xn[n*2-1] = s_2xn[n*2-1] + d_2xn[n*2-2];
  } 
}
OIL_DEFINE_IMPL_FULL (synth_53_mmx, synth_53, OIL_IMPL_FLAG_MMX);
#endif
#endif


void
mas2_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int shift = s4_2[1];

  while (n&3) {
    int x;

    x = s4_2[0] + s2[0]*s3_2[0] + s2[1]*s3_2[1];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movzwl 0(%0), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"       // mm0 = s0, s1, s2, s3
      "  movq 0(%2), %%mm1\n"       // mm1 = s0, s1, s2, s3
      "  pmullw %%mm7, %%mm0\n"     // mm0 = lo(s0*a0), lo(s1*a0), ...
      "  pmulhw %%mm7, %%mm1\n"     // mm1 = hi(s0*a0), hi(s1*a0), ...
      "  movq %%mm0, %%mm2\n"       // mm2 = lo(s0*a0), lo(s1*a0), ...
      "  punpcklwd %%mm1, %%mm0\n"  // mm0 = s0*a0, s1*a0
      "  punpckhwd %%mm1, %%mm2\n"  // mm2 = s2*a0, s3*a0
      "  movq %%mm2, %%mm1\n"       // mm1 = s2*a0, s3*a0

      "  movq 2(%2), %%mm2\n"
      "  movq 2(%2), %%mm3\n"
      "  pmullw %%mm6, %%mm2\n"
      "  pmulhw %%mm6, %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"  // mm2 = s1*a1, s2*a1
      "  punpckhwd %%mm3, %%mm4\n"  // mm4 = s3*a1, s4*a1
      "  movq %%mm4, %%mm3\n"       // mm3 = s3*a1, s4*a1

      "  paddd %%mm3, %%mm1\n"      // mm1 = s2*a0 + s3*a1, ...
      "  paddd %%mm2, %%mm0\n"      // mm0 = s0*a0 + s1*a1, ...

      "  paddd %%mm5, %%mm1\n"      // mm1 = s2*a0 + s3*a1 + offset, ...
      "  paddd %%mm5, %%mm0\n"      // mm0 = s0*a0 + s1*a1 + offset, ...

      "  movd %4, %%mm4\n"
      "  psrad %%mm4, %%mm1\n"      // mm1 = (s2*a0 + s3*a1 + offset)>>shift, ...
      "  psrad %%mm4, %%mm0\n"      // mm0 = (s0*a0 + s1*a1 + offset)>>shift, ...

      "  packssdw %%mm1, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      : "r" (shift)
      );
}
OIL_DEFINE_IMPL_FULL (mas2_add_s16_mmx, mas2_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
mas4_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_4,
    int16_t *s4_2, int n)
{
  int shift = s4_2[1];
  //int m;

  //m = n&3;
#if 1
  while (n&3) {
    int x;
    int i;

    x = s4_2[0];
    for(i=0;i<4;i++){
      x += s2[i]*s3_4[i];
    }
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
#endif
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movq 0(%0), %%mm7\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5\n"
      :: "r" (s3_4), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"       // mm0 = s0, s1, s2, s3
      "  movq 0(%2), %%mm1\n"       // mm1 = s0, s1, s2, s3
      "  pshufw $0x00, %%mm7, %%mm6\n"
      "  pmullw %%mm6, %%mm0\n"     // mm0 = lo(s0*a0), lo(s1*a0), ...
      "  pmulhw %%mm6, %%mm1\n"     // mm1 = hi(s0*a0), hi(s1*a0), ...
      "  movq %%mm0, %%mm2\n"       // mm2 = lo(s0*a0), lo(s1*a0), ...
      "  punpcklwd %%mm1, %%mm0\n"  // mm0 = s0*a0, s1*a0
      "  punpckhwd %%mm1, %%mm2\n"  // mm2 = s2*a0, s3*a0
      "  movq %%mm2, %%mm1\n"       // mm1 = s2*a0, s3*a0

      "  movq 2(%2), %%mm2\n"
      "  movq 2(%2), %%mm3\n"
      "  pshufw $0x55, %%mm7, %%mm6\n"
      "  pmullw %%mm6, %%mm2\n"
      "  pmulhw %%mm6, %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"  // mm2 = s1*a1, s2*a1
      "  punpckhwd %%mm3, %%mm4\n"  // mm4 = s3*a1, s4*a1
      "  movq %%mm4, %%mm3\n"       // mm3 = s3*a1, s4*a1
      "  paddd %%mm3, %%mm1\n"      // mm1 = s2*a0 + s3*a1, ...
      "  paddd %%mm2, %%mm0\n"      // mm0 = s0*a0 + s1*a1, ...

      "  movq 4(%2), %%mm2\n"
      "  movq 4(%2), %%mm3\n"
      "  pshufw $0xaa, %%mm7, %%mm6\n"
      "  pmullw %%mm6, %%mm2\n"
      "  pmulhw %%mm6, %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  movq %%mm4, %%mm3\n"
      "  paddd %%mm3, %%mm1\n"
      "  paddd %%mm2, %%mm0\n"

      "  movq 6(%2), %%mm2\n"
      "  movq 6(%2), %%mm3\n"
      "  pshufw $0xff, %%mm7, %%mm6\n"
      "  pmullw %%mm6, %%mm2\n"
      "  pmulhw %%mm6, %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  movq %%mm4, %%mm3\n"
      "  paddd %%mm3, %%mm1\n"
      "  paddd %%mm2, %%mm0\n"

      "  paddd %%mm5, %%mm1\n"
      "  paddd %%mm5, %%mm0\n"

      "  movd %4, %%mm4\n"
      "  psrad %%mm4, %%mm1\n"
      "  psrad %%mm4, %%mm0\n"

      "  packssdw %%mm1, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      : "r" (shift)
      );
#if 0
  while (m) {
    int x;
    int i;

    x = s4_2[0];
    for(i=0;i<4;i++){
      x += s2[i]*s3_4[i];
    }
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    m--;
  }
#endif
}
OIL_DEFINE_IMPL_FULL (mas4_add_s16_mmx, mas4_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

#if 0
/* This only does 16-bit intermediates, whereas the ref specifies 32-bit */
void
mas2_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0]*s3_2[0] + s2[1]*s3_2[1];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movzwl 0(%0), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x00, %%mm5, %%mm5\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm7, %%mm0\n"
      "  movq 2(%2), %%mm1\n"
      "  pmullw %%mm6, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  paddw %%mm5, %%mm0\n"
      "  psraw %%mm4, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );
}
OIL_DEFINE_IMPL_FULL (mas2_add_s16_mmx, mas2_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif


#if 0
/* This only does 16-bit intermediates, whereas the ref specifies 32-bit */
void
mas4_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0]*s3_2[0] + s2[1]*s3_2[1] + 
      s2[2]*s3_2[2] + s2[2]*s3_2[2];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movzwl 0(%0), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x00, %%mm5, %%mm5\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      "  pshufw $0x00, %%mm3, %%mm3\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm2\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm7, %%mm0\n"
      "  movq 2(%2), %%mm1\n"
      "  pmullw %%mm6, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  movq 4(%2), %%mm1\n"
      "  pmullw %%mm5, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  movq 6(%2), %%mm1\n"
      "  pmullw %%mm4, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  paddw %%mm3, %%mm0\n"
      "  psraw %%mm2, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );
}
OIL_DEFINE_IMPL_FULL (mas4_add_s16_mmx, mas4_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif


#if 0
/* This only does 16-bit intermediates, whereas the ref specifies 32-bit */
void
mas8_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;
    int i;

    x = s4_2[0];
    for(i=0;i<8;i++){
      x += s2[i]*s3_2[i];
    }
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movq 0(%0), %%mm6\n"
      "  movq 8(%0), %%mm7\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      "  pshufw $0x00, %%mm3, %%mm3\n"
      "  pxor %%mm4, %%mm4\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  pshufw $0x00, %%mm6, %%mm1\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm1, %%mm0\n"
      "  pshufw $0x55, %%mm6, %%mm2\n"
      "  movq 2(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0xaa, %%mm6, %%mm2\n"
      "  movq 4(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0xff, %%mm6, %%mm2\n"
      "  movq 6(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"

      "  pshufw $0x00, %%mm7, %%mm2\n"
      "  movq 8(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x55, %%mm7, %%mm2\n"
      "  movq 10(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0xaa, %%mm7, %%mm2\n"
      "  movq 12(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0xff, %%mm7, %%mm2\n"
      "  movq 14(%2), %%mm1\n"
      "  pmullw %%mm2, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"

      "  paddw %%mm3, %%mm0\n"
      "  psraw %%mm4, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );
}
OIL_DEFINE_IMPL_FULL (mas8_add_s16_mmx, mas8_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif


void
mas4_add_s16_pmaddwd (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  if (n==0) return;
  asm volatile ("\n"
      "  movq 0(%0), %%mm6\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmaddwd %%mm6, %%mm0\n"
      "  pshufw $0xee, %%mm0, %%mm1\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm3, %%mm0\n"
      "  psrad %%mm4, %%mm0\n"
      "  movd %%mm0, %%eax\n"
      "  addw 0(%1), %%ax\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  add $2, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      :
      : "eax"
      );
}
OIL_DEFINE_IMPL_FULL (mas4_add_s16_pmaddwd, mas4_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
mas8_add_s16_pmaddwd (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  if (n==0) return;
  asm volatile ("\n"
      "  movq 0(%0), %%mm6\n"
      "  movq 8(%0), %%mm7\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmaddwd %%mm6, %%mm0\n"
      "  movq 8(%2), %%mm1\n"
      "  pmaddwd %%mm7, %%mm1\n"
      "  paddd %%mm1, %%mm0\n"
      "  pshufw $0xee, %%mm0, %%mm1\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm3, %%mm0\n"
      "  psrad %%mm4, %%mm0\n"
      "  movd %%mm0, %%eax\n"
      "  addw 0(%1), %%ax\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  add $2, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      :
      : "eax"
      );
}
OIL_DEFINE_IMPL_FULL (mas8_add_s16_pmaddwd, mas8_add_s16, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);



#if 0
void
mas8_add_s16_pmaddwd2 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;
    int i;

    x = s4_2[0];
    for(i=0;i<8;i++){
      x += s2[i]*s3_2[i];
    }
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movq 0(%0), %%mm6\n"
      "  movq 8(%0), %%mm7\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x00, %%mm5, %%mm5\n"
      "  pxor %%mm4, %%mm4\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmaddwd %%mm6, %%mm0\n"
      "  movq 8(%2), %%mm1\n"
      "  pmaddwd %%mm7, %%mm1\n"
      "  paddd %%mm1, %%mm0\n"
      "  pshufw $0xee, %%mm0, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"

      "  movq 2(%2), %%mm2\n"
      "  pmaddwd %%mm6, %%mm2\n"
      "  movq 10(%2), %%mm3\n"
      "  pmaddwd %%mm7, %%mm3\n"
      "  paddd %%mm3, %%mm2\n"
      "  pshufw $0xee, %%mm2, %%mm3\n"
      "  paddw %%mm3, %%mm2\n"
      "  pextrw $0, %%mm2, %%eax\n"
      "  pinsrw $1, %%eax, %%mm0\n"

      "  movq 4(%2), %%mm2\n"
      "  pmaddwd %%mm6, %%mm2\n"
      "  movq 12(%2), %%mm3\n"
      "  pmaddwd %%mm7, %%mm3\n"
      "  paddd %%mm3, %%mm2\n"
      "  pshufw $0xee, %%mm2, %%mm3\n"
      "  paddw %%mm3, %%mm2\n"
      "  pextrw $0, %%mm2, %%eax\n"
      "  pinsrw $2, %%eax, %%mm0\n"

      "  movq 6(%2), %%mm2\n"
      "  pmaddwd %%mm6, %%mm2\n"
      "  movq 14(%2), %%mm3\n"
      "  pmaddwd %%mm7, %%mm3\n"
      "  paddd %%mm3, %%mm2\n"
      "  pshufw $0xee, %%mm2, %%mm3\n"
      "  paddw %%mm3, %%mm2\n"
      "  pextrw $0, %%mm2, %%eax\n"
      "  pinsrw $3, %%eax, %%mm0\n"

      "  paddw %%mm5, %%mm0\n"
      "  psraw %%mm4, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      :
      : "eax"
      );
}
OIL_DEFINE_IMPL_FULL (mas8_add_s16_pmaddwd2, mas8_add_s16, OIL_IMPL_FLAG_SSE);
#endif

#if 0
/* This only does 16-bit intermediates, whereas the ref specifies 32-bit */
void
mas8_add_s16_sse2 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  asm volatile ("\n"
      "  movq 0(%0), %%mm6\n"
      "  movq 8(%0), %%mm7\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      "  pshufw $0x00, %%mm3, %%mm3\n"
      "  pxor %%mm4, %%mm4\n"
      "  movzwl 2(%1), %%ecx\n"
      "  movd %%ecx, %%mm4\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw %%mm6, %%mm0\n"
      "  movq 8(%2), %%mm1\n"
      "  pmullw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0xee, %%mm0, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x01, %%mm0, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  paddw %%mm3, %%mm0\n"
      "  psraw %%mm4, %%mm0\n"
      "  movd %%mm0, %%eax\n"
      "  addw 0(%1), %%ax\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  add $2, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      :
      : "eax"
      );
}
OIL_DEFINE_IMPL_FULL (mas8_add_s16_sse2, mas8_add_s16, OIL_IMPL_FLAG_SSE);
#endif

void
mas2_across_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int16_t *s5_2, int n)
{
  int shift = s5_2[1];

  while (n&3) {
    int x;

    x = s5_2[0] + s2[0]*s4_2[0] + s3[0]*s4_2[1];
    x >>= s5_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "  movzwl 0(%0), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5\n"
      :: "r" (s4_2), "r" (s5_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"       // mm0 = s0, s1, s2, s3
      "  movq 0(%2), %%mm1\n"       // mm1 = s0, s1, s2, s3
      "  pmullw %%mm7, %%mm0\n"     // mm0 = lo(s0*a0), lo(s1*a0), ...
      "  pmulhw %%mm7, %%mm1\n"     // mm1 = hi(s0*a0), hi(s1*a0), ...
      "  movq %%mm0, %%mm2\n"       // mm2 = lo(s0*a0), lo(s1*a0), ...
      "  punpcklwd %%mm1, %%mm0\n"  // mm0 = s0*a0, s1*a0
      "  punpckhwd %%mm1, %%mm2\n"  // mm2 = s2*a0, s3*a0
      "  movq %%mm2, %%mm1\n"       // mm1 = s2*a0, s3*a0

      "  movq 0(%3), %%mm2\n"
      "  movq 0(%3), %%mm3\n"
      "  pmullw %%mm6, %%mm2\n"
      "  pmulhw %%mm6, %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"  // mm2 = s1*a1, s2*a1
      "  punpckhwd %%mm3, %%mm4\n"  // mm4 = s3*a1, s4*a1
      "  movq %%mm4, %%mm3\n"       // mm3 = s3*a1, s4*a1

      "  paddd %%mm3, %%mm1\n"      // mm1 = s2*a0 + s3*a1, ...
      "  paddd %%mm2, %%mm0\n"      // mm0 = s0*a0 + s1*a1, ...

      "  paddd %%mm5, %%mm1\n"      // mm1 = s2*a0 + s3*a1 + offset, ...
      "  paddd %%mm5, %%mm0\n"      // mm0 = s0*a0 + s1*a1 + offset, ...

      "  movd %5, %%mm4\n"
      "  psrad %%mm4, %%mm1\n"      // mm1 = (s2*a0 + s3*a1 + offset)>>shift, ...
      "  psrad %%mm4, %%mm0\n"      // mm0 = (s0*a0 + s1*a1 + offset)>>shift, ...

      "  packssdw %%mm1, %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  add $8, %3\n"
      "  decl %4\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (s3), "+m" (n)
      : "r" (shift)
      );
}
OIL_DEFINE_IMPL_FULL (mas2_across_add_s16_mmx, mas2_across_add_s16,
    OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add_const_rshift_s16_mmx(int16_t *d1, int16_t *s1, int16_t *s2_2, int n)
{
  while(n&3) {
    d1[0] = (s1[0] + s2_2[0])>>s2_2[1];
    d1++;
    s1++;
    n--;
  }
  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "  movzwl 0(%2), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%2), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  paddsw %%mm7, %%mm0\n"
      "  psraw %%mm6, %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2_2), "+r" (n)
      :
      : "ecx"
      );

}
OIL_DEFINE_IMPL_FULL (add_const_rshift_s16_mmx, add_const_rshift_s16,
    OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
multiply_and_add_s16_mmx(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3, int n)
{
  while(n&3) {
    d1[0] = s1[0] + s2[0]*s3[0];
    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  pmullw 0(%3), %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  add $8, %3\n"
      "  decl %4\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (s3), "+r" (n)
      );

}
OIL_DEFINE_IMPL_FULL (multiply_and_add_s16_mmx, multiply_and_add_s16,
    OIL_IMPL_FLAG_MMX);

void
multiply_and_add_s16_u8_mmx(int16_t *d1, int16_t *s1, int16_t *s2,
    uint8_t *s3, int n)
{
  while(n&3) {
    d1[0] = s1[0] + s2[0]*s3[0];
    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%3), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmullw 0(%2), %%mm0\n"
      "  paddw 0(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  add $4, %3\n"
      "  decl %4\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (s3), "+r" (n)
      );

}
OIL_DEFINE_IMPL_FULL (multiply_and_add_s16_u8_mmx, multiply_and_add_s16_u8,
    OIL_IMPL_FLAG_MMX);

void
multiply_and_add_s16_u8_mmx_2(int16_t *d1, int16_t *s1, int16_t *s2,
    uint8_t *s3, int n)
{
  while(n&7) {
    d1[0] = s1[0] + s2[0]*s3[0];
    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  n>>=3;
  if (n==0) return;
  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "1:\n"
      "  movd 0(%3), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "   movd 4(%3), %%mm1\n"
      "  pmullw 0(%2), %%mm0\n"
      "   punpcklbw %%mm7, %%mm1\n"
      "  paddw 0(%1), %%mm0\n"
      "   pmullw 8(%2), %%mm1\n"
      "  movq %%mm0, 0(%0)\n"
      "   paddw 8(%1), %%mm1\n"
      "   movq %%mm1, 8(%0)\n"

      "  add $16, %0\n"
      "  add $16, %1\n"
      "  add $16, %2\n"
      "  add $8, %3\n"
      "  decl %4\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (s3), "+r" (n)
      );

}
OIL_DEFINE_IMPL_FULL (multiply_and_add_s16_u8_mmx_2, multiply_and_add_s16_u8,
    OIL_IMPL_FLAG_MMX);

void
multiply_and_acc_12xn_s16_u8_mmx (int16_t *i1, int is1, int16_t *s1,
    int ss1, uint8_t *s2, int ss2, int n)
{
  if (n==0) return;
  __asm__ __volatile__ ("\n"
      "  pxor %%mm7, %%mm7\n"
      "1:\n"
      "  movd 0(%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmullw 0(%1), %%mm0\n"
      "  paddw 0(%0), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "   movd 4(%2), %%mm1\n"
      "   punpcklbw %%mm7, %%mm1\n"
      "   pmullw 8(%1), %%mm1\n"
      "   paddw 8(%0), %%mm1\n"
      "   movq %%mm1, 8(%0)\n"
      "    movd 8(%2), %%mm2\n"
      "    punpcklbw %%mm7, %%mm2\n"
      "    pmullw 16(%1), %%mm2\n"
      "    paddw 16(%0), %%mm2\n"
      "    movq %%mm2, 16(%0)\n"

      "  add %4, %0\n"
      "  add %5, %1\n"
      "  add %6, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (i1), "+r" (s1), "+r" (s2), "+r" (n)
      : "r" ((long)is1), "r" ((long)ss1), "r" ((long)ss2)
      );
}
OIL_DEFINE_IMPL_FULL (multiply_and_acc_12xn_s16_u8_mmx,
    multiply_and_acc_12xn_s16_u8, OIL_IMPL_FLAG_MMX);


#ifdef ENABLE_BROKEN_IMPLS
void
mas4_across_add_s16_mmx (int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2,
    int16_t *s3_4, int16_t *s4_2, int n)
{
  int16_t *s2_nx4_off;

  while (n&3) {
    int x;
    int j;
    x = s4_2[0];
    for(j=0;j<4;j++){
      x += OIL_GET(s2_nx4, j*sstr2, int16_t)*s3_4[j];
    }
    x >>= s4_2[1];
    d[0] = s1[0] + x;

    n--;
    d++;
    s1++;
    s2_nx4++;
  }
  if (n==0) return;

  s2_nx4_off = OIL_OFFSET(s2_nx4, 3*sstr2);

  n >>= 2;
  __asm__ __volatile__ ("\n"
      "  movq 0(%[s3_4]), %%mm0\n"
      "  pshufw $0x55, %%mm0, %%mm1\n"
      "  pshufw $0xaa, %%mm0, %%mm2\n"
      "  pshufw $0xff, %%mm0, %%mm3\n"
      "  pshufw $0x00, %%mm0, %%mm0\n"
      "  movzwl 0(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm7\n"
      "  pshufw $0x00, %%mm7, %%mm7\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      :
      : [s3_4] "r" (s3_4),
        [s4_2] "r" (s4_2)
      : "ecx"
      );

  __asm__ __volatile__ ("\n"
      "1:\n"
      "  movq 0(%[s2_nx4]), %%mm4\n"
      "  pmullw %%mm0, %%mm4\n"
      "  movq (%[s2_nx4],%[sstr]), %%mm5\n"
      "  pmullw %%mm1, %%mm5\n"
      "  paddsw %%mm5,%%mm4\n"
      "  movq (%[s2_nx4],%[sstr],2), %%mm5\n"
      "  pmullw %%mm2, %%mm5\n"
      "  paddsw %%mm5,%%mm4\n"
      "  movq (%[s2_nx4_off]), %%mm5\n"
      "  pmullw %%mm3, %%mm5\n"
      "  paddsw %%mm5,%%mm4\n"
      "  paddsw %%mm7, %%mm4\n"
      "  psraw %%mm6, %%mm4\n"
      "  paddsw (%[s1]),%%mm4\n"
      "  movq %%mm4, 0(%[d])\n"

      "  add $8, %[s2_nx4]\n"
      "  add $8, %[s2_nx4_off]\n"
      "  add $8, %[s1]\n"
      "  add $8, %[d]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [s2_nx4] "+r" (s2_nx4),
        [d] "+r" (d),
        [s2_nx4_off] "+r" (s2_nx4_off),
        [n] "+m" (n),
        [s1] "+r" (s1)
      : [sstr] "r" (sstr2)
      );
}
OIL_DEFINE_IMPL_FULL (mas4_across_add_s16_mmx, mas4_across_add_s16,
    OIL_IMPL_FLAG_MMX);
#endif

void
mas4_across_add_s16_mmx (int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2,
    int16_t *s3_4, int16_t *s4_2, int n)
{
  int16_t *s2_nx4_off;

  while (n&3) {
    int x;
    int j;
    x = s4_2[0];
    for(j=0;j<4;j++){
      x += OIL_GET(s2_nx4, j*sstr2, int16_t)*s3_4[j];
    }
    x >>= s4_2[1];
    d[0] = s1[0] + x;

    n--;
    d++;
    s1++;
    s2_nx4++;
  }
  if (n==0) return;

  s2_nx4_off = OIL_OFFSET(s2_nx4, 3*sstr2);

  n >>= 2;
  __asm__ __volatile__ ("\n"
      "  movq 0(%[s3_4]), %%mm0\n"
      "  pxor %%mm5, %%mm5\n"
      "  movd 0(%[s4_2]), %%mm5\n"
      :
      : [s3_4] "r" (s3_4),
        [s4_2] "r" (s4_2)
      );

  __asm__ __volatile__ ("\n"
      "1:\n"
      "  pshufw $0x00, %%mm0, %%mm6\n"
      "  pmullw 0(%[s2_nx4]), %%mm6\n"
      "  pshufw $0x00, %%mm0, %%mm3\n"
      "  pmulhw 0(%[s2_nx4]), %%mm3\n"
      "  movq %%mm6, %%mm7\n"
      "  punpcklwd %%mm3, %%mm6\n"
      "  punpckhwd %%mm3, %%mm7\n"

      "  pshufw $0x55, %%mm0, %%mm2\n"
      "  pmullw 0(%[s2_nx4],%[sstr]), %%mm2\n"
      "  pshufw $0x55, %%mm0, %%mm3\n"
      "  pmulhw 0(%[s2_nx4],%[sstr]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  pshufw $0xaa, %%mm0, %%mm2\n"
      "  pmullw 0(%[s2_nx4],%[sstr],2), %%mm2\n"
      "  pshufw $0xaa, %%mm0, %%mm3\n"
      "  pmulhw 0(%[s2_nx4],%[sstr],2), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  pshufw $0xff, %%mm0, %%mm2\n"
      "  pmullw 0(%[s2_nx4_off]), %%mm2\n"
      "  pshufw $0xff, %%mm0, %%mm3\n"
      "  pmulhw 0(%[s2_nx4_off]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  pshufw $0xcc, %%mm5, %%mm1\n"
      "  paddd %%mm1, %%mm6\n"
      "  paddd %%mm1, %%mm7\n"

      "  pshufw $0xfd, %%mm5, %%mm1\n"
      "  psrad %%mm1, %%mm6\n"
      "  psrad %%mm1, %%mm7\n"
      "  packssdw %%mm7, %%mm6\n"

      "  paddsw (%[s1]),%%mm6\n"
      "  movq %%mm6, 0(%[d])\n"

      "  add $8, %[s2_nx4]\n"
      "  add $8, %[s2_nx4_off]\n"
      "  add $8, %[s1]\n"
      "  add $8, %[d]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [s2_nx4] "+r" (s2_nx4),
        [d] "+r" (d),
        [s2_nx4_off] "+r" (s2_nx4_off),
        [n] "+m" (n),
        [s1] "+r" (s1)
      : [sstr] "r" ((long)(sstr2))
      );
}
OIL_DEFINE_IMPL_FULL (mas4_across_add_s16_mmx, mas4_across_add_s16,
    OIL_IMPL_FLAG_MMX);

void
mas8_across_add_s16_mmx (int16_t *d, int16_t *s1, int16_t *s2_nx8, int sstr2,
    int16_t *s3_8, int16_t *s4_2, int n)
{
  int16_t *s2_nx8_off;
  void *tmp = NULL;

  while (n&3) {
    int x;
    int j;
    x = s4_2[0];
    for(j=0;j<8;j++){
      x += OIL_GET(s2_nx8, j*sstr2, int16_t)*s3_8[j];
    }
    x >>= s4_2[1];
    d[0] = s1[0] + x;

    n--;
    d++;
    s1++;
    s2_nx8++;
  }
  if (n==0) return;

  s2_nx8_off = OIL_OFFSET(s2_nx8, 7*sstr2);

  n >>= 2;
  __asm__ __volatile__ ("\n"
      "  movq 0(%[s3_8]), %%mm0\n"
      "  pxor %%mm5, %%mm5\n"
      "  movd 0(%[s4_2]), %%mm5\n"
      :
      : [s3_8] "r" (s3_8),
        [s4_2] "r" (s4_2)
      );

  __asm__ __volatile__ ("\n"
      "1:\n"
      "  mov %[s2_nx8], %[tmp]\n"
      "  movq 0(%[s3_8]), %%mm0\n"

      "  pshufw $0x00, %%mm0, %%mm6\n"
      "  pmullw 0(%[tmp]), %%mm6\n"
      "  pshufw $0x00, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm6, %%mm7\n"
      "  punpcklwd %%mm3, %%mm6\n"
      "  punpckhwd %%mm3, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0x55, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0x55, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0xaa, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0xaa, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0xff, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0xff, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  movq 8(%[s3_8]), %%mm0\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0x00, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0x00, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0x55, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0x55, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0xaa, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0xaa, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  add %[sstr], %[tmp]\n"
      "  pshufw $0xff, %%mm0, %%mm2\n"
      "  pmullw 0(%[tmp]), %%mm2\n"
      "  pshufw $0xff, %%mm0, %%mm3\n"
      "  pmulhw 0(%[tmp]), %%mm3\n"
      "  movq %%mm2, %%mm4\n"
      "  punpcklwd %%mm3, %%mm2\n"
      "  punpckhwd %%mm3, %%mm4\n"
      "  paddd %%mm2, %%mm6\n"
      "  paddd %%mm4, %%mm7\n"

      "  pshufw $0xcc, %%mm5, %%mm1\n"
      "  paddd %%mm1, %%mm6\n"
      "  paddd %%mm1, %%mm7\n"

      "  pshufw $0xfd, %%mm5, %%mm1\n"
      "  psrad %%mm1, %%mm6\n"
      "  psrad %%mm1, %%mm7\n"
      "  packssdw %%mm7, %%mm6\n"

      "  paddsw (%[s1]),%%mm6\n"
      "  movq %%mm6, 0(%[d])\n"

      "  add $8, %[s2_nx8]\n"
      "  add $8, %[s1]\n"
      "  add $8, %[d]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [s2_nx8] "+r" (s2_nx8),
        [tmp] "+r" (tmp),
        [s3_8] "+r" (s3_8),
        [d] "+r" (d),
        [n] "+m" (n),
        [s1] "+r" ((long)(s1))
      : [sstr] "r" ((long)(sstr2))
      );
}
OIL_DEFINE_IMPL_FULL (mas8_across_add_s16_mmx, mas8_across_add_s16,
    OIL_IMPL_FLAG_MMX);

void
lshift_s16_mmx(int16_t *d1, int16_t *s1, int16_t *s3_1, int n)
{
  while (n&3) {
    d1[0] = s1[0]<<s3_1[0];
    d1++;
    s1++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  __asm__ __volatile__ ("\n"
      "  movzwl 0(%[s3_1]), %%ecx\n"
      "  movd %%ecx, %%mm1\n"
      "1:\n"
      "  movq 0(%[s1]), %%mm0\n"
      "  psllw %%mm1, %%mm0\n"
      "  movq %%mm0, 0(%[d1])\n"
      "  add $8, %[d1]\n"
      "  add $8, %[s1]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms"
      : [d1] "+r" (d1),
        [s1] "+r" (s1),
        [n] "+r" (n)
      : [s3_1] "r" (s3_1)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lshift_s16_mmx, lshift_s16, OIL_IMPL_FLAG_MMX);

void
lshift_s16_mmx_2(int16_t *d1, int16_t *s1, int16_t *s3_1, int n)
{
  while (n&7) {
    d1[0] = s1[0]<<s3_1[0];
    d1++;
    s1++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  __asm__ __volatile__ ("\n"
      "  movzwl 0(%[s3_1]), %%ecx\n"
      "  movd %%ecx, %%mm1\n"
      "1:\n"
      "  movq 0(%[s1]), %%mm0\n"
      "  psllw %%mm1, %%mm0\n"
      "  movq %%mm0, 0(%[d1])\n"
      "  movq 8(%[s1]), %%mm0\n"
      "  psllw %%mm1, %%mm0\n"
      "  movq %%mm0, 8(%[d1])\n"
      "  add $16, %[d1]\n"
      "  add $16, %[s1]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms"
      : [d1] "+r" (d1),
        [s1] "+r" (s1),
        [n] "+r" (n)
      : [s3_1] "r" (s3_1)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (lshift_s16_mmx_2, lshift_s16, OIL_IMPL_FLAG_MMX);


