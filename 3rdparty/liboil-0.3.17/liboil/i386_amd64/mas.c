
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
mas10_u8_mmx (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
        const int16_t *s3_2, int n)
{
  int j;
  int x;

  while(n&3) {
    x = 0;
    for(j=0;j<10;j++){
      x += s1_np9[j] * s2_10[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np9++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

#define LOOP(x) \
      "  movd " #x "(%[s1_np9]), %%mm0\n" \
      "  punpcklbw %%mm7, %%mm0\n" \
      "  movq 2*" #x "(%[s2_10]), %%mm1\n" \
      "  pshufw $0x00, %%mm1, %%mm1\n" \
      "  pmullw %%mm1, %%mm0\n" \
      "  paddw %%mm0, %%mm2\n"

      LOOP(0)
      LOOP(1)
      LOOP(2)
      LOOP(3)
      LOOP(4)
      LOOP(5)
      LOOP(6)
      LOOP(7)
      LOOP(8)
      LOOP(9)
#undef LOOP

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np9]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np9] "+r" (s1_np9),
        [n] "+m" (n)
      : [s2_10] "r" (s2_10),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas10_u8_mmx, mas10_u8_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

#if 0
void
mas10_u8_mmx_2 (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
        const int16_t *s3_2, int n)
{
  int j;
  int x;
  int16_t coeff[4*10];
  int16_t *ptr;

  ptr = coeff;

  while(n&3) {
    x = 0;
    for(j=0;j<10;j++){
      x += s1_np9[j] * s2_10[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np9++;
    n--;
  }

  for(j=0;j<10;j++){
    ptr[4*j + 0] = s2_10[j];
    ptr[4*j + 1] = s2_10[j];
    ptr[4*j + 2] = s2_10[j];
    ptr[4*j + 3] = s2_10[j];
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

#define LOOP(x) \
      "  movd " #x "(%[s1_np9]), %%mm0\n" \
      "  punpcklbw %%mm7, %%mm0\n" \
      "  pmullw 8*" #x "(%[coeff]), %%mm0\n" \
      "  paddw %%mm0, %%mm2\n"

      LOOP(0)
      LOOP(1)
      LOOP(2)
      LOOP(3)
      LOOP(4)
      LOOP(5)
      LOOP(6)
      LOOP(7)
      LOOP(8)
      LOOP(9)
#undef LOOP

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np9]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np9] "+r" (s1_np9),
        [n] "+m" (n)
      : [coeff] "r" (ptr),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas10_u8_mmx_2, mas10_u8_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);
#endif

void
mas10_u8_mmx_3 (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
        const int16_t *s3_2, int n)
{
  int j;
  int x;

  while(n&3) {
    x = 0;
    for(j=0;j<10;j++){
      x += s1_np9[j] * s2_10[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np9++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_10]), %%mm3\n"
      "  movq 8(%[s2_10]), %%mm4\n"

      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

#define LOOP(x) \
      "  movd " #x "(%[s1_np9]), %%mm0\n" \
      "  punpcklbw %%mm7, %%mm0\n" \
      "  movq 2*" #x "(%[s2_10]), %%mm1\n" \
      "  pshufw $0x00, %%mm1, %%mm1\n" \
      "  pmullw %%mm1, %%mm0\n" \
      "  paddw %%mm0, %%mm2\n"

      //LOOP(0)
      "  movd 0(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(1)
      "  movd 1(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(2)
      "  movd 2(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(3)
      "  movd 3(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(4)
      "  movd 4(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(5)
      "  movd 5(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(6)
      "  movd 6(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      //LOOP(7)
      "  movd 7(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      LOOP(8)
      LOOP(9)
#undef LOOP

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np9]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np9] "+r" (s1_np9),
        [n] "+m" (n)
      : [s2_10] "r" (s2_10),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas10_u8_mmx_3, mas10_u8_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
mas10_u8_mmx_4 (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
        const int16_t *s3_2, int n)
{
  if (n == 0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      "  movd 0(%[s1_np9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmaddwd 0(%[s2_10]), %%mm0\n"

      "  movd 4(%[s1_np9]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 8(%[s2_10]), %%mm1\n"

      "  movd 8(%[s1_np9]), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmaddwd 16(%[s2_10]), %%mm2\n"

      "  paddd %%mm1, %%mm0\n"
      "  movq %%mm0, %%mm1\n"
      "  psrlq $32, %%mm0\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm2, %%mm0\n"
      "  paddd %%mm6, %%mm0\n"

      "  psrad %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $1, %[s1_np9]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np9] "+r" (s1_np9),
        [n] "+m" (n)
      : [s2_10] "r" (s2_10),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas10_u8_mmx_4, mas10_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);


void
mas8_u8_mmx_3 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int j;
  int x;

  while(n&3) {
    x = 0;
    for(j=0;j<8;j++){
      x += s1_np7[j] * s2_8[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np7++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm3\n"
      "  movq 8(%[s2_8]), %%mm4\n"

      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 1(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 2(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 3(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 4(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 5(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 6(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 7(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_mmx_3, mas8_u8_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
mas8_u8_mmx_4 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  if (n == 0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmaddwd 0(%[s2_8]), %%mm0\n"

      "  movd 4(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 8(%[s2_8]), %%mm1\n"

      "  paddd %%mm1, %%mm0\n"
      "  movq %%mm0, %%mm1\n"
      "  psrlq $32, %%mm0\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm6, %%mm0\n"

      "  psrad %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $1, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_mmx_4, mas8_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
mas8_u8_sym_mmx_3 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int j;
  int x;

  while(n&3) {
    x = 0;
    for(j=0;j<8;j++){
      x += s1_np7[j] * s2_8[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np7++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm3\n"
      "  movq 8(%[s2_8]), %%mm4\n"

      " .p2align 4,,15                  \n"
      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 7(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      //"  pshufw $0x00, %%mm3, %%mm1\n"
      //"  pmullw %%mm1, %%mm0\n"
      //"  paddw %%mm0, %%mm2\n"
      "  psubw %%mm0, %%mm2\n"

      "  movd 1(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 6(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x55*1, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 2(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 5(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x55*2, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 3(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 4(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x55*3, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_sym_mmx_3, mas8_u8_sym_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
mas8_u8_sym_mmx_41 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int j;
  int x;
  int16_t tmp[16];

  while(n&3) {
    x = 0;
    for(j=0;j<8;j++){
      x += s1_np7[j] * s2_8[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np7++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm3\n"
      "  pshufw $0x55*0, %%mm3, %%mm1\n"
      "  movq %%mm1, 0(%[coeff])\n"
      "  pshufw $0x55*1, %%mm3, %%mm1\n"
      "  movq %%mm1, 8(%[coeff])\n"
      "  pshufw $0x55*2, %%mm3, %%mm1\n"
      "  movq %%mm1, 16(%[coeff])\n"
      "  pshufw $0x55*3, %%mm3, %%mm1\n"
      "  movq %%mm1, 24(%[coeff])\n"
      :
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2),
        [coeff] "r" (tmp)
      : "ecx");

  __asm__ __volatile__("\n"
      " .p2align 4,,15                  \n"
      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 7(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pmullw 0(%[coeff]), %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 1(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 6(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pmullw 8(%[coeff]), %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 2(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 5(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pmullw 16(%[coeff]), %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 3(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 4(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  pmullw 24(%[coeff]), %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [coeff] "r" (tmp)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_sym_mmx_41, mas8_u8_sym_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);


#define PSHUFW_3210 "0xe4"
#define PSHUFW_0123 "0x1b"

void
mas8_u8_sym_mmx_5 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  if (n==0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6\n" // 01 00 01 00

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  cmpl $0, %[n]\n"
      "  jz 2f\n"

      "1:\n"
      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
#if 1
      "  movd 4(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pshufw $0x1b, %%mm1, %%mm1\n" // 00 01 10 11
      "  paddw %%mm1, %%mm0\n"
      "  pmaddwd 0(%[s2_8]), %%mm0\n"
#else
      "  pmaddwd 0(%[s2_8]), %%mm0\n"

      "  movd 4(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 8(%[s2_8]), %%mm1\n"
      "  paddd %%mm1, %%mm0\n"
#endif
      
      "  pshufw $0xee, %%mm0, %%mm1\n" // 11 10 11 10
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm6, %%mm0\n"

      "  psrad %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $1, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"

      "2:\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_sym_mmx_5, mas8_u8_sym_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

#ifdef HAVE_SSSE3_ASM
void
mas8_u8_sym_mmx_6 (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int8_t coeff[8];
  int8_t *ack;
  int i;

  for(i=0;i<8;i++){
    //coeff[i] = s2_8[i];
    coeff[i] = i;
  }
  ack = coeff;

  if (n==0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6\n" // 01 00 01 00

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm4\n"
      "  packsswb 8(%[s2_8]), %%mm4\n"

      "1:\n"
      "  movq 0(%[s1_np7]), %%mm0\n"
      "  pmaddubsw %%mm4, %%mm0\n"

#if 1
      "  pshufw $0xee, %%mm0, %%mm1\n" // 11 10 11 10
      "  paddw %%mm1, %%mm0\n"
      "  pshufw $0x55, %%mm0, %%mm1\n" // 01 01 01 01
      "  paddw %%mm1, %%mm0\n"
#else
      "  phaddw %%mm0, %%mm0\n"
      "  phaddw %%mm0, %%mm0\n"
#endif

      "  paddw %%mm6, %%mm0\n"
      "  psraw %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $1, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"

      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_sym_mmx_6, mas8_u8_sym_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT|OIL_IMPL_FLAG_SSSE3);
#endif

#ifdef ENABLE_BROKEN_IMPLS
/* This only works for the taps array: -1, 3, -7, 21, 21, -7, 3, -1 */
void
mas8_u8_supersym_mmx (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int j;
  int x;

  while(n&3) {
    x = 0;
    for(j=0;j<8;j++){
      x += s1_np7[j] * s2_8[j];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_np7++;
    n--;
  }

  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm3\n"
      "  movq 8(%[s2_8]), %%mm4\n"

      "1:\n"
      "  movd 0(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 7(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"

      "  movd 1(%[s1_np7]), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  movd 6(%[s1_np7]), %%mm3\n"
      "  punpcklbw %%mm7, %%mm3\n"
      "  paddw %%mm3, %%mm2\n"

      "  paddw %%mm2, %%mm0\n"
      "  psllw $2, %%mm2\n"
      "  psubw %%mm0, %%mm2\n"
      "      movq %%mm2, %%mm4\n"

      "  movd 2(%[s1_np7]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movd 5(%[s1_np7]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"

      "  movd 3(%[s1_np7]), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  movd 4(%[s1_np7]), %%mm3\n"
      "  punpcklbw %%mm7, %%mm3\n"
      "  paddw %%mm3, %%mm2\n"

      "  paddw %%mm2, %%mm0\n"
      "  psllw $2, %%mm2\n"
      "  psubw %%mm0, %%mm2\n"

      "  psubw %%mm2, %%mm4\n"
      "  psllw $3, %%mm2\n"
      "  paddw %%mm4, %%mm2\n"

      "  paddw %%mm6, %%mm2\n"

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      "  add $4, %[d]\n"
      "  add $4, %[s1_np7]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_np7] "+r" (s1_np7),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_u8_supersym_mmx, mas8_u8_sym_l15, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);
#endif

void
mas12_addc_rshift_decim2_u8_mmx_4 (uint8_t *d, const uint8_t *s1_2xnp11,
    const int16_t *s2_12, const int16_t *s3_2, int n)
{
  if (n == 0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      "  movd 0(%[s1_2xnp11]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmaddwd 0(%[s2_12]), %%mm0\n"

      "  movd 4(%[s1_2xnp11]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 8(%[s2_12]), %%mm1\n"
      "  paddd %%mm1, %%mm0\n"

      "  movd 8(%[s1_2xnp11]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 16(%[s2_12]), %%mm1\n"
      "  paddd %%mm1, %%mm0\n"

      "  movq %%mm0, %%mm1\n"
      "  psrlq $32, %%mm0\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm6, %%mm0\n"

      "  psrad %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $2, %[s1_2xnp11]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_2xnp11] "+r" (s1_2xnp11),
        [n] "+m" (n)
      : [s2_12] "r" (s2_12),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas12_addc_rshift_decim2_u8_mmx_4,
    mas12_addc_rshift_decim2_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

#if 0
void
mas8_addc_rshift_decim2_u8_mmx_4 (uint8_t *d, const uint8_t *s1_2xnp9,
    const int16_t *s2_8, const int16_t *s3_2, int n)
{
  if (n == 0) return;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movzwl 0(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "1:\n"
      "  movd 0(%[s1_2xnp9]), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pmaddwd 0(%[s2_8]), %%mm0\n"

      "  movd 4(%[s1_2xnp9]), %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmaddwd 8(%[s2_8]), %%mm1\n"
      "  paddd %%mm1, %%mm0\n"

      "  movq %%mm0, %%mm1\n"
      "  psrlq $32, %%mm0\n"
      "  paddd %%mm1, %%mm0\n"
      "  paddd %%mm6, %%mm0\n"

      "  psrad %%mm5, %%mm0\n"
      "  pmaxsw %%mm7, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, %%ecx\n"
      "  movb %%cl,0(%[d])\n"

      "  add $1, %[d]\n"
      "  add $2, %[s1_2xnp9]\n"
      "  decl %[n]\n"
      "  jnz 1b\n"
      "  emms\n"
      : [d] "+r" (d),
        [s1_2xnp9] "+r" (s1_2xnp9),
        [n] "+m" (n)
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");
}
OIL_DEFINE_IMPL_FULL (mas8_addc_rshift_decim2_u8_mmx_4,
    mas8_addc_rshift_decim2_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

#endif

void
mas8_across_u8_mmx_3 (uint8_t *d, const uint8_t *s1_nx8, int ss1,
    const int16_t *s2_8, const int16_t *s3_2, int n)
{
  int i;
  int x;

  while(n&3) {
    x = 0;
    for(i=0;i<8;i++){
      x += OIL_GET(s1_nx8, i*ss1, uint8_t)*s2_8[i];
    }
    *d = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
    d++;
    s1_nx8++;
    n--;
  }

  if (n == 0) return;
  n>>=2;
  __asm__ __volatile__("\n"
      "  pxor %%mm7, %%mm7\n"

      "  movd (%[s3_2]), %%mm6\n"

      "  movzwl 2(%[s3_2]), %%ecx\n"
      "  movd %%ecx, %%mm5\n"

      "  movq 0(%[s2_8]), %%mm3\n"
      "  movq 8(%[s2_8]), %%mm4\n"
      :
      : [s2_8] "r" (s2_8),
        [s3_2] "r" (s3_2)
      : "ecx");

  while (n > 0) {
    const uint8_t *p = s1_nx8;
  __asm__ __volatile__("\n"
      "1:\n"
      /* load 128 */
      "  pshufw $0x00, %%mm6, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm3, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*1, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*2, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  movd 0(%[p]), %%mm0\n"
      "  add %[ss1], %[p]\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x55*3, %%mm4, %%mm1\n"
      "  pmullw %%mm1, %%mm0\n"
      "  paddw %%mm0, %%mm2\n"

      "  psraw %%mm5, %%mm2\n"
      "  pmaxsw %%mm7, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, 0(%[d])\n"
      : [p] "+r" (p)
      : [d] "r" (d), [ss1] "r" ((long)ss1));
    d+=4;
    s1_nx8+=4;
    n--;
  }

  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (mas8_across_u8_mmx_3, mas8_across_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

