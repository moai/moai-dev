
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
addc_s16_mmx (int16_t *d, int16_t *s, int16_t *s2, int n)
{
  int val = s2[0];

  while(n&15) {
    d[0] = s[0] + s2[0];
    s++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 4;
    __asm__ __volatile__ ("\n"
        "  movd %[val], %%mm0\n"
        "  pshufw $00, %%mm0, %%mm0\n"
        "1:\n"
        "  movq 0(%[s]), %%mm1\n"
        "  paddw %%mm0, %%mm1\n"
        "  movq %%mm1, 0(%[d])\n"
        "   movq 8(%[s]), %%mm2\n"
        "   paddw %%mm0, %%mm2\n"
        "   movq %%mm2, 8(%[d])\n"
        "    movq 16(%[s]), %%mm3\n"
        "    paddw %%mm0, %%mm3\n"
        "    movq %%mm3, 16(%[d])\n"
        "     movq 24(%[s]), %%mm4\n"
        "     paddw %%mm0, %%mm4\n"
        "     movq %%mm4, 24(%[d])\n"
        "  add $32, %[s]\n"
        "  add $32, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        "  emms\n"
        : [s] "+r" (s), [n] "+r" (n), [d] "+r" (d)
        : [val] "r" (val));
  }
}
OIL_DEFINE_IMPL_FULL (addc_s16_mmx, addc_s16, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
addc_s16_mmx2 (int16_t *d, int16_t *s, int16_t *s2, int n)
{
  int val = s2[0];

  while(n&15) {
    d[0] = s[0] + s2[0];
    s++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 4;
    __asm__ __volatile__ ("\n"
        "  movd %[val], %%mm0\n"
        "  pshufw $00, %%mm0, %%mm0\n"
        "1:\n"
        "  movq 0(%[s]), %%mm1\n"
        "   movq 8(%[s]), %%mm2\n"
        "  paddw %%mm0, %%mm1\n"
        "   paddw %%mm0, %%mm2\n"
        "  movq %%mm1, 0(%[d])\n"
        "   movq %%mm2, 8(%[d])\n"
        "    movq 16(%[s]), %%mm3\n"
        "     movq 24(%[s]), %%mm4\n"
        "    paddw %%mm0, %%mm3\n"
        "     paddw %%mm0, %%mm4\n"
        "    movq %%mm3, 16(%[d])\n"
        "     movq %%mm4, 24(%[d])\n"
        "  add $32, %[s]\n"
        "  add $32, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        "  emms\n"
        : [s] "+r" (s), [n] "+r" (n), [d] "+r" (d)
        : [val] "r" (val));
  }
}
OIL_DEFINE_IMPL_FULL (addc_s16_mmx2, addc_s16, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
addc_s16_sse (int16_t *d, int16_t *s, int16_t *s2, int n)
{
  int val = s2[0];

  while(n&15) {
    d[0] = s[0] + s2[0];
    s++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 4;
    __asm__ __volatile__ ("\n"
        "  movd %[val], %%xmm0\n"
        "  pshuflw $00, %%xmm0, %%xmm0\n"
        "  pshufd $00, %%xmm0, %%xmm0\n"
        "1:\n"
        "  movdqu 0(%[s]), %%xmm1\n"
        "  paddw %%xmm0, %%xmm1\n"
        "  movdqu %%xmm1, 0(%[d])\n"
        "   movdqu 16(%[s]), %%xmm2\n"
        "   paddw %%xmm0, %%xmm2\n"
        "   movdqu %%xmm2, 16(%[d])\n"
        "  add $32, %[s]\n"
        "  add $32, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        : [s] "+r" (s), [n] "+r" (n), [d] "+r" (d)
        : [val] "r" (val));
  }
}
OIL_DEFINE_IMPL_FULL (addc_s16_sse, addc_s16, OIL_IMPL_FLAG_SSE|OIL_IMPL_FLAG_SSE2|OIL_IMPL_FLAG_MMXEXT);

void
addc_rshift_s16_mmx (int16_t *d, int16_t *s, int16_t *s2, int n)
{
  int val = s2[0];
  int shift = s2[1];
  int16_t x;

  while(n&15) {
    x = s[0] + s2[0];
    d[0] = x>>shift;
    s++;
    d++;
    n--;
  }
  if (n > 0) {
    n >>= 4;
    __asm__ __volatile__ ("\n"
        "  movd %[val], %%mm0\n"
        "  pshufw $00, %%mm0, %%mm0\n"
        "  movd %[shift], %%mm7\n"
        "1:\n"
        "  movq 0(%[s]), %%mm1\n"
        "  paddw %%mm0, %%mm1\n"
        "  psraw %%mm7, %%mm1\n"
        "  movq %%mm1, 0(%[d])\n"
        "   movq 8(%[s]), %%mm2\n"
        "   paddw %%mm0, %%mm2\n"
        "   psraw %%mm7, %%mm2\n"
        "   movq %%mm2, 8(%[d])\n"
        "    movq 16(%[s]), %%mm3\n"
        "    paddw %%mm0, %%mm3\n"
        "    psraw %%mm7, %%mm3\n"
        "    movq %%mm3, 16(%[d])\n"
        "     movq 24(%[s]), %%mm4\n"
        "     paddw %%mm0, %%mm4\n"
        "     psraw %%mm7, %%mm4\n"
        "     movq %%mm4, 24(%[d])\n"
        "  add $32, %[s]\n"
        "  add $32, %[d]\n"
        "  decl %[n]\n"
        "  jnz 1b\n"
        "  emms\n"
        : [s] "+r" (s), [n] "+r" (n), [d] "+r" (d)
        : [val] "r" (val), [shift] "r" (shift));
  }
}
OIL_DEFINE_IMPL_FULL (addc_rshift_s16_mmx, addc_rshift_s16, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

