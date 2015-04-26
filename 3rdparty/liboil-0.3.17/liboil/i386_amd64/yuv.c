
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
packyuyv_mmx (uint32_t *d, uint8_t *s1_nx2, uint8_t *s2, uint8_t *s3, int n)
{
  while(n&3) {
    uint8_t *dest = (uint8_t *)d;

    dest[0] = s1_nx2[0];
    dest[2] = s1_nx2[1];
    dest[1] = s2[0];
    dest[3] = s3[0];

    s1_nx2+=2;
    s2++;
    s3++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 2;
    __asm__ __volatile__ ("\n"
        "1:\n"
        "  movd 0(%[s2]), %%mm0\n"
        "  punpcklbw 0(%[s3]), %%mm0\n"
        "  movq 0(%[s1]), %%mm1\n"
        "  movq %%mm1, %%mm2\n"
        "  punpcklbw %%mm0, %%mm1\n"
        "  punpckhbw %%mm0, %%mm2\n"
        "  movq %%mm1, 0(%[d])\n"
        "  movq %%mm2, 8(%[d])\n"
        "  add $4, %[s3]\n"
        "  add $4, %[s2]\n"
        "  add $8, %[s1]\n"
        "  add $16, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        "  emms\n"
        : [s1] "+r" (s1_nx2), [n] "+m" (n), [d] "+r" (d),
          [s2] "+r" (s2), [s3] "+r" (s3));
  }
}
OIL_DEFINE_IMPL_FULL (packyuyv_mmx, packyuyv, OIL_IMPL_FLAG_MMX);

void
packyuyv_mmx2 (uint32_t *d, uint8_t *s1_nx2, uint8_t *s2, uint8_t *s3, int n)
{
  while(n&7) {
    uint8_t *dest = (uint8_t *)d;

    dest[0] = s1_nx2[0];
    dest[2] = s1_nx2[1];
    dest[1] = s2[0];
    dest[3] = s3[0];

    s1_nx2+=2;
    s2++;
    s3++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 3;
    __asm__ __volatile__ ("\n"
        "1:\n"
        "  movq 0(%[s2]), %%mm0\n"
        "  movq %%mm0, %%mm1\n"
        "  punpcklbw 0(%[s3]), %%mm0\n"
        "  punpckhbw 0(%[s3]), %%mm1\n"

        "  movq 0(%[s1]), %%mm2\n"
        "  punpcklbw %%mm0, %%mm2\n"
        "  movq %%mm2, 0(%[d])\n"

        "  movq 0(%[s1]), %%mm3\n"
        "  punpckhbw %%mm0, %%mm3\n"
        "  movq %%mm3, 8(%[d])\n"

        "  movq 8(%[s1]), %%mm2\n"
        "  punpcklbw %%mm1, %%mm2\n"
        "  movq %%mm2, 16(%[d])\n"

        "  movq 8(%[s1]), %%mm3\n"
        "  punpckhbw %%mm1, %%mm3\n"
        "  movq %%mm3, 24(%[d])\n"

        "  add $8, %[s3]\n"
        "  add $8, %[s2]\n"
        "  add $16, %[s1]\n"
        "  add $32, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        "  emms\n"
        : [s1] "+r" (s1_nx2), [n] "+m" (n), [d] "+r" (d),
          [s2] "+r" (s2), [s3] "+r" (s3));
  }
}
OIL_DEFINE_IMPL_FULL (packyuyv_mmx2, packyuyv, OIL_IMPL_FLAG_MMX);

