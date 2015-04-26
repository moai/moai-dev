
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>




void
add2_rshift_add_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 0(%[s1]), %%mm0\n"
      "  movq %%mm0, 0(%[d1])\n"

      "  add $8, %[d1]\n"
      "  add $8, %[s1]\n"
      "  add $8, %[s2]\n"
      "  add $8, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_add_s16_mmx, add2_rshift_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add2_rshift_sub_s16_mmx (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&3) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] - x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=2;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  movq 0(%[s1]), %%mm1\n"
      "  psubw %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%[d1])\n"

      "  add $8, %[d1]\n"
      "  add $8, %[s1]\n"
      "  add $8, %[s2]\n"
      "  add $8, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_sub_s16_mmx, add2_rshift_sub_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add2_rshift_add_s16_mmx_unroll2 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&7) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 0(%[s1]), %%mm0\n"
      "  movq %%mm0, 0(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 8(%[s2]), %%mm0\n"
      "  paddw 8(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 8(%[s1]), %%mm0\n"
      "  movq %%mm0, 8(%[d1])\n"

      "  add $16, %[d1]\n"
      "  add $16, %[s1]\n"
      "  add $16, %[s2]\n"
      "  add $16, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_add_s16_mmx_unroll2, add2_rshift_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add2_rshift_sub_s16_mmx_unroll2 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&7) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] - x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  movq 0(%[s1]), %%mm1\n"
      "  psubw %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 8(%[s2]), %%mm0\n"
      "  paddw 8(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  movq 8(%[s1]), %%mm1\n"
      "  psubw %%mm0, %%mm1\n"
      "  movq %%mm1, 8(%[d1])\n"

      "  add $16, %[d1]\n"
      "  add $16, %[s1]\n"
      "  add $16, %[s2]\n"
      "  add $16, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_sub_s16_mmx_unroll2, add2_rshift_sub_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add2_rshift_add_s16_mmx_unroll4 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&15) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=4;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 0(%[s1]), %%mm0\n"
      "  movq %%mm0, 0(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 8(%[s2]), %%mm0\n"
      "  paddw 8(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 8(%[s1]), %%mm0\n"
      "  movq %%mm0, 8(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 16(%[s2]), %%mm0\n"
      "  paddw 16(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 16(%[s1]), %%mm0\n"
      "  movq %%mm0, 16(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 24(%[s2]), %%mm0\n"
      "  paddw 24(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  paddw 24(%[s1]), %%mm0\n"
      "  movq %%mm0, 24(%[d1])\n"

      "  add $32, %[d1]\n"
      "  add $32, %[s1]\n"
      "  add $32, %[s2]\n"
      "  add $32, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_add_s16_mmx_unroll4, add2_rshift_add_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
add2_rshift_sub_s16_mmx_unroll4 (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&15) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] - x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=4;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movq %%mm4, %%mm0\n"
      "  paddw 0(%[s2]), %%mm0\n"
      "  paddw 0(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  movq 0(%[s1]), %%mm1\n"
      "  psubw %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%[d1])\n"

      "  movq %%mm4, %%mm2\n"
      "  paddw 8(%[s2]), %%mm2\n"
      "  paddw 8(%[s3]), %%mm2\n"
      "  psraw %%mm5, %%mm2\n"
      "  movq 8(%[s1]), %%mm3\n"
      "  psubw %%mm2, %%mm3\n"
      "  movq %%mm3, 8(%[d1])\n"

      "  movq %%mm4, %%mm0\n"
      "  paddw 16(%[s2]), %%mm0\n"
      "  paddw 16(%[s3]), %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  movq 16(%[s1]), %%mm1\n"
      "  psubw %%mm0, %%mm1\n"
      "  movq %%mm1, 16(%[d1])\n"

      "  movq %%mm4, %%mm2\n"
      "  paddw 24(%[s2]), %%mm2\n"
      "  paddw 24(%[s3]), %%mm2\n"
      "  psraw %%mm5, %%mm2\n"
      "  movq 24(%[s1]), %%mm3\n"
      "  psubw %%mm2, %%mm3\n"
      "  movq %%mm3, 24(%[d1])\n"

      "  add $32, %[d1]\n"
      "  add $32, %[s1]\n"
      "  add $32, %[s2]\n"
      "  add $32, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_sub_s16_mmx_unroll4, add2_rshift_sub_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


void
add2_rshift_add_s16_sse (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&7) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] + x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%xmm4\n"
      "  pshuflw $0x00, %%xmm4, %%xmm4\n"
      "  pshufd $0x00, %%xmm4, %%xmm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%xmm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
#if 0
      "  movdqu %%xmm4, %%xmm0\n"
      "  movdqu 0(%[s2]), %%xmm1\n"
      "  paddw %%xmm1, %%xmm0\n"
      "  movdqu 0(%[s3]), %%xmm2\n"
      "  paddw %%xmm2, %%xmm0\n"
      "  psraw %%xmm5, %%xmm0\n"
      "  movdqu 0(%[d1]), %%xmm1\n"
      "  paddw %%xmm1, %%xmm0\n"
      "  movdqu %%xmm0, 0(%[d1])\n"
#endif
      "  movdqu %%xmm4, %%xmm0\n"
      "  movdqu 0(%[s2]), %%xmm1\n"
      "  paddw %%xmm1, %%xmm0\n"
      "  movdqu 0(%[s3]), %%xmm2\n"
      "  paddw %%xmm2, %%xmm0\n"
      "  psraw %%xmm5, %%xmm0\n"
      "  movdqu 0(%[s1]), %%xmm1\n"
      "  paddw %%xmm0, %%xmm1\n"
      "  movdqu %%xmm1, 0(%[d1])\n"

      "  add $16, %[d1]\n"
      "  add $16, %[s1]\n"
      "  add $16, %[s2]\n"
      "  add $16, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_add_s16_sse, add2_rshift_add_s16, OIL_IMPL_FLAG_MMXEXT|OIL_IMPL_FLAG_SSE|OIL_IMPL_FLAG_SSE2);

void
add2_rshift_sub_s16_sse (int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int n)
{
  while (n&7) {
    int x;

    x = s4_2[0] + s2[0] + s3[0];
    x >>= s4_2[1];
    d1[0] = s1[0] - x;

    d1++;
    s1++;
    s2++;
    s3++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  asm volatile ("\n"
      "  movd 0(%[s4_2]), %%xmm4\n"
      "  pshuflw $0x00, %%xmm4, %%xmm4\n"
      "  pshufd $0x00, %%xmm4, %%xmm4\n"
      "  movzwl 2(%[s4_2]), %%ecx\n"
      "  movd %%ecx, %%xmm5\n"
      :: [s4_2] "r" (s4_2)
      : "ecx");
  asm volatile ("\n"
      "1:\n"
      "  movdqu %%xmm4, %%xmm0\n"
      "  movdqu 0(%[s2]), %%xmm1\n"
      "  paddw %%xmm1, %%xmm0\n"
      "  movdqu 0(%[s3]), %%xmm2\n"
      "  paddw %%xmm2, %%xmm0\n"
      "  psraw %%xmm5, %%xmm0\n"
      "  movdqu 0(%[s1]), %%xmm1\n"
      "  psubw %%xmm0, %%xmm1\n"
      "  movdqu %%xmm1, 0(%[d1])\n"

      "  add $16, %[d1]\n"
      "  add $16, %[s1]\n"
      "  add $16, %[s2]\n"
      "  add $16, %[s3]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      : [d1] "+r" (d1), [s1] "+r" (s1), [s2] "+r" (s2),
        [s3] "+r" (s3), [n] "+r" (n)
      :
      );
}
OIL_DEFINE_IMPL_FULL (add2_rshift_sub_s16_sse, add2_rshift_sub_s16, OIL_IMPL_FLAG_MMXEXT|OIL_IMPL_FLAG_SSE|OIL_IMPL_FLAG_SSE2);

