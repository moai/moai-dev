
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>

void
add_s16_mmx(int16_t *d1, int16_t *s1, int16_t *s2, int n)
{
  while(n&3) {
    d1[0] = s1[0] + s2[0];
    d1++;
    s1++;
    s2++;
    n--;
  }
  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
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
OIL_DEFINE_IMPL_FULL (add_s16_mmx, add_s16, OIL_IMPL_FLAG_MMX);

void
add_s16_u8_mmx(int16_t *d1, int16_t *s1, uint8_t *s2, int n)
{
  while(n&7) {
    d1[0] = s1[0] + s2[0];
    d1++;
    s1++;
    s2++;
    n--;
  }
  n>>=3;
  if (n==0) return;
  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  movq 0(%2), %%mm1\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  punpckhbw %%mm7, %%mm1\n"
      "  paddw 0(%1), %%mm0\n"
      "  paddw 8(%1), %%mm1\n"
      "  movq %%mm0, 0(%0)\n"
      "  movq %%mm1, 8(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );

}
OIL_DEFINE_IMPL_FULL (add_s16_u8_mmx, add_s16_u8, OIL_IMPL_FLAG_MMX);

void
subtract_s16_mmx(int16_t *d1, int16_t *s1, int16_t *s2, int n)
{
  while(n&3) {
    d1[0] = s1[0] - s2[0];
    d1++;
    s1++;
    s2++;
    n--;
  }
  n>>=2;
  if (n==0) return;
  asm volatile ("\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  psubw 0(%2), %%mm0\n"
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
OIL_DEFINE_IMPL_FULL (subtract_s16_mmx, subtract_s16, OIL_IMPL_FLAG_MMX);

void
subtract_s16_u8_mmx(int16_t *d1, int16_t *s1, uint8_t *s2, int n)
{
  while(n&7) {
    d1[0] = s1[0] - s2[0];
    d1++;
    s1++;
    s2++;
    n--;
  }
  n>>=3;
  if (n==0) return;
  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "1:\n"
      "  movq 0(%2), %%mm0\n"
      "  movq 0(%2), %%mm1\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  punpckhbw %%mm7, %%mm1\n"
      "  movq 0(%1), %%mm2\n"
      "  psubw %%mm0, %%mm2\n"
      "  movq 8(%1), %%mm3\n"
      "  psubw %%mm1, %%mm3\n"
      "  movq %%mm2, 0(%0)\n"
      "  movq %%mm3, 8(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  add $8, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (d1), "+r" (s1), "+r" (s2), "+r" (n)
      );

}
OIL_DEFINE_IMPL_FULL (subtract_s16_u8_mmx, subtract_s16_u8, OIL_IMPL_FLAG_MMX);

