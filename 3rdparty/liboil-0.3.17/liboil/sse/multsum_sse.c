#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilclasses.h>
#include <liboil/liboilfunction.h>
#include <emmintrin.h>

#ifdef HAVE_I386
#define SSE_FUNCTION __attribute__((force_align_arg_pointer))
#else
#define SSE_FUNCTION
#endif

#define MULTSUM_SSE2_NSTRIDED(i) { \
  t1 = _mm_load_pd(&OIL_GET(src1, i, double)); \
  t2 = _mm_load_pd(&OIL_GET(src2, i, double)); \
  t1 = _mm_mul_pd(t1,t2); \
  sum.reg = _mm_add_pd(sum.reg,t1); \
}
#define MULTSUM_SSE2_NSTRIDEDP(i) { \
  t1 = _mm_load_pd(&OIL_GET(src1, i*sstr1, double)); \
  t2 = _mm_loadl_pd(t2, &OIL_GET(src2, i*sstr2, double)); \
  t2 = _mm_loadh_pd(t2, &OIL_GET(src2, (i+1)*sstr2, double)); \
  t1 = _mm_mul_pd(t1,t2); \
  sum.reg = _mm_add_pd(sum.reg,t1); \
}
#define MULTSUM_SSE2_STRIDED(i) { \
  t1 = _mm_loadl_pd(t1, &OIL_GET(src1, i*sstr1, double)); \
  t1 = _mm_loadh_pd(t1, &OIL_GET(src1, (i+1)*sstr1, double)); \
  t2 = _mm_loadl_pd(t2, &OIL_GET(src2, i*sstr2, double)); \
  t2 = _mm_loadh_pd(t2, &OIL_GET(src2, (i+1)*sstr2, double)); \
  t1 = _mm_mul_pd(t1,t2); \
  sum.reg = _mm_add_pd(sum.reg,t1); \
}


#ifdef ENABLE_BROKEN_IMPLS
SSE_FUNCTION static void
multsum_f64_sse2_unroll4(double *dest,
     const double *src1, int sstr1,
     const double *src2, int sstr2,
     int n)
{
  __m128d t1, t2;
  union {
    __m128d reg;
    double vals[2];
  } sum;
  int i = 0;

  sum.reg = _mm_setzero_pd();
  while (i < n-3) {
    MULTSUM_SSE2_STRIDED(0);
    MULTSUM_SSE2_STRIDED(2);

    OIL_INCREMENT(src1, 4*sstr1);
    OIL_INCREMENT(src2, 4*sstr2);
    i += 4;
  }
  while (i < n-1) {
    MULTSUM_SSE2_STRIDED(0);

    OIL_INCREMENT(src1, 2*sstr1);
    OIL_INCREMENT(src2, 2*sstr2);
    i+=2;
  }
  *dest = sum.vals[0] + sum.vals[1];
  if (i < n) {
    *dest += (OIL_GET(src1,0,double)*OIL_GET(src2,0,double));
  }
}
OIL_DEFINE_IMPL_FULL (multsum_f64_sse2_unroll4, multsum_f64, OIL_IMPL_FLAG_SSE2);
#endif

