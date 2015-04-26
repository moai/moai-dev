
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>

void
multiply_and_acc_6xn_s16_u8_mmx (int16_t *i1, int is1, int16_t *s1,
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
      "   movd 2(%2), %%mm1\n"
      "   punpcklbw %%mm7, %%mm1\n"
      "   pmullw 4(%1), %%mm1\n"
      "   paddw 4(%0), %%mm1\n"
      "   pshufw $0xee, %%mm1, %%mm1\n"
      "   movd %%mm1, 8(%0)\n"

      "  add %4, %0\n"
      "  add %5, %1\n"
      "  add %6, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (i1), "+r" (s1), "+r" (s2), "+m" (n)
#ifdef __i386__
      : "m" (is1), "m" (ss1), "m" (ss2)
#else
      : "r" ((long)is1), "r" ((long)ss1), "r" ((long)ss2)
#endif
      );
}
OIL_DEFINE_IMPL_FULL (multiply_and_acc_6xn_s16_u8_mmx,
    multiply_and_acc_6xn_s16_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
multiply_and_acc_8xn_s16_u8_mmx (int16_t *i1, int is1, int16_t *s1,
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

      "  add %4, %0\n"
      "  add %5, %1\n"
      "  add %6, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (i1), "+r" (s1), "+r" (s2), "+m" (n)
#ifdef __i386__
      : "m" (is1), "m" (ss1), "m" (ss2)
#else
      : "r" ((long)is1), "r" ((long)ss1), "r" ((long)ss2)
#endif
      );
}
OIL_DEFINE_IMPL_FULL (multiply_and_acc_8xn_s16_u8_mmx,
    multiply_and_acc_8xn_s16_u8, OIL_IMPL_FLAG_MMX);

void
multiply_and_acc_16xn_s16_u8_mmx (int16_t *i1, int is1, int16_t *s1,
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
      "     movd 12(%2), %%mm2\n"
      "     punpcklbw %%mm7, %%mm2\n"
      "     pmullw 24(%1), %%mm2\n"
      "     paddw 24(%0), %%mm2\n"
      "     movq %%mm2, 24(%0)\n"

      "  add %4, %0\n"
      "  add %5, %1\n"
      "  add %6, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (i1), "+r" (s1), "+r" (s2), "+m" (n)
#ifdef __i386__
      : "m" (is1), "m" (ss1), "m" (ss2)
#else
      : "r" ((long)is1), "r" ((long)ss1), "r" ((long)ss2)
#endif
      );
}
OIL_DEFINE_IMPL_FULL (multiply_and_acc_16xn_s16_u8_mmx,
    multiply_and_acc_16xn_s16_u8, OIL_IMPL_FLAG_MMX);

void
multiply_and_acc_24xn_s16_u8_mmx (int16_t *i1, int is1, int16_t *s1,
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
      "     movd 12(%2), %%mm2\n"
      "     punpcklbw %%mm7, %%mm2\n"
      "     pmullw 24(%1), %%mm2\n"
      "     paddw 24(%0), %%mm2\n"
      "     movq %%mm2, 24(%0)\n"
      " movd 16(%2), %%mm2\n"
      " punpcklbw %%mm7, %%mm2\n"
      " pmullw 32(%1), %%mm2\n"
      " paddw 32(%0), %%mm2\n"
      " movq %%mm2, 32(%0)\n"
      "  movd 20(%2), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmullw 40(%1), %%mm2\n"
      "  paddw 40(%0), %%mm2\n"
      "  movq %%mm2, 40(%0)\n"

      "  add %4, %0\n"
      "  add %5, %1\n"
      "  add %6, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (i1), "+r" (s1), "+r" (s2), "+m" (n)
#ifdef __i386__
      : "m" (is1), "m" (ss1), "m" (ss2)
#else
      : "r" ((long)is1), "r" ((long)ss1), "r" ((long)ss2)
#endif
      );
}
OIL_DEFINE_IMPL_FULL (multiply_and_acc_24xn_s16_u8_mmx,
    multiply_and_acc_24xn_s16_u8, OIL_IMPL_FLAG_MMX);


#if 0
void
mas2_add_s16_sse (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int shift = s4_2[1];

  while (n&7) {
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

  n>>=3;
  asm volatile ("\n"
      "  movzwl 0(%0), %%ecx\n"
      "  movd %%ecx, %%xmm7\n"
      "  pshuflw $0x00, %%xmm7, %%xmm7\n"
      "  pshufd $0x00, %%xmm7, %%xmm7\n"
      "  movzwl 2(%0), %%ecx\n"
      "  movd %%ecx, %%xmm6\n"
      "  pshuflw $0x00, %%xmm6, %%xmm6\n"
      "  pshufd $0x00, %%xmm6, %%xmm6\n"
      "  movzwl 0(%1), %%ecx\n"
      "  movd %%ecx, %%xmm5\n"
      "  pshuflw $0x44, %%xmm5, %%xmm5\n"
      "  pshufd $0x00, %%xmm5, %%xmm5\n"
      :: "r" (s3_2), "r" (s4_2)
      : "ecx"
      );
  asm volatile ("\n"
      "1:\n"
      "  movdqu 0(%2), %%xmm0\n"       // mm0 = s0, s1, s2, s3
      "  movdqu 0(%2), %%xmm1\n"       // mm1 = s0, s1, s2, s3
      "  pmullw %%xmm7, %%xmm0\n"     // mm0 = lo(s0*a0), lo(s1*a0), ...
      "  pmulhw %%xmm7, %%xmm1\n"     // mm1 = hi(s0*a0), hi(s1*a0), ...
      "  movdqu %%xmm0, %%xmm2\n"       // mm2 = lo(s0*a0), lo(s1*a0), ...
      "  punpcklwd %%xmm1, %%xmm0\n"  // mm0 = s0*a0, s1*a0
      "  punpckhwd %%xmm1, %%xmm2\n"  // mm2 = s2*a0, s3*a0
      "  movdqu %%xmm2, %%xmm1\n"       // mm1 = s2*a0, s3*a0

      "  movdqu 2(%2), %%xmm2\n"
      "  movdqu 2(%2), %%xmm3\n"
      "  pmullw %%xmm6, %%xmm2\n"
      "  pmulhw %%xmm6, %%xmm3\n"
      "  movdqu %%xmm2, %%xmm4\n"
      "  punpcklwd %%xmm3, %%xmm2\n"  // mm2 = s1*a1, s2*a1
      "  punpckhwd %%xmm3, %%xmm4\n"  // mm4 = s3*a1, s4*a1
      "  movdqu %%xmm4, %%xmm3\n"       // mm3 = s3*a1, s4*a1

      "  paddd %%xmm3, %%xmm1\n"      // mm1 = s2*a0 + s3*a1, ...
      "  paddd %%xmm2, %%xmm0\n"      // mm0 = s0*a0 + s1*a1, ...

      "  paddd %%xmm5, %%xmm1\n"      // mm1 = s2*a0 + s3*a1 + offset, ...
      "  paddd %%xmm5, %%xmm0\n"      // mm0 = s0*a0 + s1*a1 + offset, ...

      "  movd %4, %%xmm4\n"
      "  psrad %%xmm4, %%xmm1\n"      // mm1 = (s2*a0 + s3*a1 + offset)>>shift, ...
      "  psrad %%xmm4, %%xmm0\n"      // mm0 = (s0*a0 + s1*a1 + offset)>>shift, ...

      "  packssdw %%xmm1, %%xmm0\n"
      "  paddw 0(%1), %%xmm0\n"
      "  movdqu %%xmm0, 0(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  add $16, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      : "r" (shift)
      );
}
OIL_DEFINE_IMPL_FULL (mas2_add_s16_sse, mas2_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif




