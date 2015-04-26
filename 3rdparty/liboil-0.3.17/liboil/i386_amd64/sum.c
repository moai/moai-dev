
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
sum_s16_mmx (int16_t *d, int16_t *s, int n)
{
  int16_t sum = 0;
  int16_t a[4];

  while(n&15) {
    sum += s[0];
    s++;
    n--;
  }
  if (n > 0) {

    n >>= 4;
    asm volatile ("\n"
        "  pxor %%mm0, %%mm0\n"
        "1:\n"
        "  paddw 0(%0), %%mm0\n"
        "  paddw 8(%0), %%mm0\n"
        "  paddw 16(%0), %%mm0\n"
        "  paddw 24(%0), %%mm0\n"
        "  add $32, %0\n"
        "  decl %1\n"
        "  jnz 1b\n"
        "  movq %%mm0, 0(%2)\n"
        "  emms\n"
        : "+r" (s), "+r" (n)
        : "r" (a)
        : "memory");

    sum += a[0];
    sum += a[1];
    sum += a[2];
    sum += a[3];
  }

  d[0] = sum;
}
OIL_DEFINE_IMPL_FULL (sum_s16_mmx, sum_s16, OIL_IMPL_FLAG_MMX);

