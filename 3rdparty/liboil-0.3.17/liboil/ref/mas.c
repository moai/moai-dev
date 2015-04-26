
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>

static void
mas_test (OilTest *test)
{
  int16_t *data;
  int i;
  int n;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16() >> 4;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC3);
  if (n == 2) {
    data[0] = 1;
    data[1] = 1;

    data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
    data[0] = 1;
    data[1] = 1;
  } else {
    for(i=0;i<n;i++){
      data[i] = (oil_rand_s16()>>4)/n;
    }

    data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
    data[0] = (1<<11);
    data[1] = 12;
  }
}

static void
mas2_across_test (OilTest *test)
{
  int16_t *data;
  int i;
  int n;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16()>>4;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  for(i=0;i<test->n;i++){
    data[i] = oil_rand_s16()>>4;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC4);
  for(i=0;i<n;i++){
    data[i] = (oil_rand_s16()>>4)/n;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC5);
  data[0] = (1<<11);
  data[1] = 12;
}

static void
mas48_across_test (OilTest *test)
{
  int16_t *data;
  int stride;
  int i;
  int j;
  int n;
  int m;
  int sum;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  for(i=0;i<test->n;i++){
    //data[i] = oil_rand_s16()>>1;
    data[i] = 0;
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  m = oil_test_get_arg_post_n (test, OIL_ARG_SRC2);
  stride = oil_test_get_arg_stride (test, OIL_ARG_SRC2);
  for(j=0;j<m;j++){
    for(i=0;i<test->n;i++){
      data[i] = oil_rand_s16()>>12;
    }
    data = OIL_OFFSET(data, stride);
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC3);
  sum = 0;
#if 0
  if (n==8) {
    for(i=0;i<8;i++){
      data[i] = 1;
    }
  } else {
    data[0] = -1;
    data[1] = 9;
    data[2] = 9;
    data[3] = -1;
  }
#endif
  for(i=0;i<n-1;i++){
    data[i] = (oil_rand_s16()>>8)/n;
    sum += data[i];
  }
  data[i] = 128 - sum;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC4);
  data[0] = (1<<6);
  data[1] = 7;
#if 0
  data[0] = (1<<(4-1));
  data[1] = 4;
#endif
}

static void
mas10_u8_test (OilTest *test)
{
  static const int taps[] = { 3, -11, 25, -56, 167, 167, -56, 25, -11, 3 };
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<10;i++){
    data[i] = taps[i];
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  data[0] = 128;
  data[1] = 8;
}

static void
mas10_u8_l15_test (OilTest *test)
{
  static const int taps[] = { 0, -1, 3, -7, 21, 21, -7, 3, -1, 0 };
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<10;i++){
    data[i] = taps[i];
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  data[0] = 32;
  data[1] = 6;
}

static void
mas8_u8_test (OilTest *test)
{
  static const int taps[] = { -1, 3, -7, 21, 21, -7, 3, -1 };
  int16_t *data;
  int i;
#if 0
  int n;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC1);
  for(i=0;i<n;i++){
    data[i] = 100*((i%8)==4);
  }
#endif

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<8;i++){
    data[i] = taps[i];
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  data[0] = 32;
  data[1] = 6;

}

static void
mas12_test (OilTest *test)
{
  const int taps[12] = { 4, -4, -8, 4, 46, 86, 86, 46, 4, -8, -4, 4 };
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<12;i++){
    data[i] = taps[i];
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  data[0] = (1<<11);
  data[1] = 12;
}

static void
mas8_test (OilTest *test)
{
  const int taps[8] = { -8, 4, 46, 86, 86, 46, 4, -8 };
  int16_t *data;
  int i;

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  for(i=0;i<8;i++){
    data[i] = taps[i];
  }

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC3);
  data[0] = (1<<11);
  data[1] = 12;
}

OIL_DEFINE_CLASS_FULL (mas2_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np1, int16_t *s3_2, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (mas4_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np3, int16_t *s3_4, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (mas8_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_np7, int16_t *s3_8, int16_t *s4_2, "
    "int n", mas_test);
OIL_DEFINE_CLASS_FULL (mas2_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3, int16_t *s4_2, "
    "int16_t *s5_2, int n", mas2_across_test);
OIL_DEFINE_CLASS_FULL (mas4_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2, int16_t *s3_4, "
    "int16_t *s4_2, int n", mas48_across_test);
OIL_DEFINE_CLASS_FULL (mas8_across_add_s16,
    "int16_t *d, int16_t *s1, int16_t *s2_nx8, int sstr2, int16_t *s3_8, "
    "int16_t *s4_2, int n", mas48_across_test);
OIL_DEFINE_CLASS_FULL (mas10_u8, "uint8_t *d, uint8_t *s1_np9, int16_t *s2_10, "
    "int16_t *s3_2, int n", mas10_u8_test);
OIL_DEFINE_CLASS_FULL (mas10_u8_l15, "uint8_t *d, uint8_t *s1_np9, int16_t *s2_10, "
    "int16_t *s3_2, int n", mas10_u8_l15_test);
OIL_DEFINE_CLASS_FULL (mas10_u8_sym_l15, "uint8_t *d, uint8_t *s1_np9, int16_t *s2_10, "
    "int16_t *s3_2, int n", mas10_u8_l15_test);
OIL_DEFINE_CLASS_FULL (mas8_u8, "uint8_t *d, uint8_t *s1_np7, int16_t *s2_8, "
    "int16_t *s3_2, int n", mas8_u8_test);
OIL_DEFINE_CLASS_FULL (mas8_u8_l15, "uint8_t *d, uint8_t *s1_np7, int16_t *s2_8, "
    "int16_t *s3_2, int n", mas8_u8_test);
OIL_DEFINE_CLASS_FULL (mas8_u8_sym_l15, "uint8_t *d, uint8_t *s1_np7, int16_t *s2_8, "
    "int16_t *s3_2, int n", mas8_u8_test);
OIL_DEFINE_CLASS_FULL (mas12_addc_rshift_decim2_u8,
    "uint8_t *d, uint8_t *s1_2xnp11, int16_t *s2_12, "
    "int16_t *s3_2, int n", mas12_test);
#if 0
OIL_DEFINE_CLASS_FULL (mas12across_addc_rshift_u8,
    "uint8_t *d, uint8_t **s1_a12, int16_t *s2_12, "
    "int16_t *s3_2, int n", mas12_test);
#endif
OIL_DEFINE_CLASS_FULL (mas8_addc_rshift_decim2_u8,
    "uint8_t *d, uint8_t *s1_2xnp9, int16_t *s2_8, "
    "int16_t *s3_2, int n", mas8_test);
OIL_DEFINE_CLASS_FULL (mas8_across_u8, "uint8_t *d, uint8_t *s1_nx8, int sstr1, "
    "int16_t *s2_8, int16_t *s3_2, int n", mas8_u8_test);

void
mas2_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_2,
    int16_t *s4_2, int n)
{
  int i;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0] + s2[i]*s3_2[0] + s2[i+1]*s3_2[1];
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas2_add_s16_ref, mas2_add_s16);

void
mas4_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_4,
    int16_t *s4_2, int n)
{
  int i;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0] + s2[i]*s3_4[0] + s2[i+1]*s3_4[1] + s2[i+2]*s3_4[2] +
        s2[i+3]*s3_4[3];
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas4_add_s16_ref, mas4_add_s16);

void
mas8_add_s16_ref(int16_t *d1, int16_t *s1, int16_t *s2, int16_t *s3_8,
    int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<8;j++){
      x += s2[i+j]*s3_8[j];
    }
    x >>= s4_2[1];
    d1[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas8_add_s16_ref, mas8_add_s16);

void
mas2_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2, int16_t *s3,
    int16_t *s4_2, int16_t *s5_2, int n)
{
  int i;
  int x;
  for(i=0;i<n;i++){
    x = s5_2[0];
    x += s2[i]*s4_2[0] + s3[i]*s4_2[1];
    x >>= s5_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas2_across_add_s16_ref, mas2_across_add_s16);

void
mas4_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2_nx4, int sstr2,
    int16_t *s3_4, int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;
  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<4;j++){
      x += OIL_GET(s2_nx4, i*sizeof(int16_t) + j*sstr2, int16_t)*s3_4[j];
    }
    x >>= s4_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas4_across_add_s16_ref, mas4_across_add_s16);

void
mas8_across_add_s16_ref (int16_t *d, int16_t *s1, int16_t *s2_nx8, int sstr2,
    int16_t *s3_8, int16_t *s4_2, int n)
{
  int i;
  int j;
  int x;
  for(i=0;i<n;i++){
    x = s4_2[0];
    for(j=0;j<8;j++){
      x += OIL_GET(s2_nx8, i*sizeof(int16_t) + j*sstr2, int16_t)*s3_8[j];
    }
    x >>= s4_2[1];
    d[i] = s1[i] + x;
  }
}
OIL_DEFINE_IMPL_REF (mas8_across_add_s16_ref, mas8_across_add_s16);


void
mas10_u8_ref (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
    const int16_t *s3_2, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = 0;
    for(j=0;j<10;j++){
      x += s1_np9[i+j] * s2_10[j];
    }
    d[i] = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas10_u8_ref, mas10_u8);

void
mas10_u8_l15_ref (uint8_t *d, const uint8_t *s1_np9, const int16_t *s2_10,
    const int16_t *s3_2, int n)
{
  mas10_u8_ref (d, s1_np9, s2_10, s3_2, n);
}
OIL_DEFINE_IMPL_REF (mas10_u8_l15_ref, mas10_u8_l15);

void
mas10_u8_sym_l15_ref (uint8_t *d, const uint8_t *s1_np9,
    const int16_t *s2_10, const int16_t *s3_2, int n)
{
  mas10_u8_ref (d, s1_np9, s2_10, s3_2, n);
}
OIL_DEFINE_IMPL_REF (mas10_u8_sym_l15_ref, mas10_u8_sym_l15);

void
mas8_u8_ref (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
        const int16_t *s3_2, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = 0;
    for(j=0;j<8;j++){
      x += s1_np7[i+j] * s2_8[j];
    }
    d[i] = CLAMP((x + s3_2[0])>>s3_2[1],0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas8_u8_ref, mas8_u8);

void
mas8_u8_l15_ref (uint8_t *d, const uint8_t *s1_np7, const int16_t *s2_8,
    const int16_t *s3_2, int n)
{
  mas8_u8_ref (d, s1_np7, s2_8, s3_2, n);
}
OIL_DEFINE_IMPL_REF (mas8_u8_l15_ref, mas8_u8_l15);

void
mas8_u8_sym_l15_ref (uint8_t *d, const uint8_t *s1_np7,
    const int16_t *s2_8, const int16_t *s3_2, int n)
{
  mas8_u8_ref (d, s1_np7, s2_8, s3_2, n);
}
OIL_DEFINE_IMPL_REF (mas8_u8_sym_l15_ref, mas8_u8_sym_l15);

void
mas8_across_u8_ref (uint8_t *d, uint8_t *s1_nx8, int sstr1,
    int16_t *s2_8, int16_t *s3_2, int n)
{
  int i;
  int j;
  int x;
  for(i=0;i<n;i++){
    x = s3_2[0];
    for(j=0;j<8;j++){
      x += OIL_GET(s1_nx8, i*sizeof(uint8_t) + j*sstr1, uint8_t)*s2_8[j];
    }
    x >>= s3_2[1];
    d[i] = CLAMP(x,0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas8_across_u8_ref, mas8_across_u8);

static void
mas12_addc_rshift_decim2_u8_ref (uint8_t *dest, const uint8_t *src,
    const int16_t *taps, const int16_t *offsetshift, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = 0;
    for(j=0;j<12;j++){
      x += taps[j]*src[i*2 + j];
    }
    dest[i] = CLAMP((x + offsetshift[0]) >> offsetshift[1],0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas12_addc_rshift_decim2_u8_ref,
    mas12_addc_rshift_decim2_u8);

#if 0
void
mas12across_addc_rshift_u8_ref (uint8_t *dest, uint8_t **src,
    const int16_t *taps, const int16_t *offsetshift, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = 0;
    for(j=0;j<12;j++){
      x += taps[j]*src[j][i];
    }
    dest[i] = CLAMP((x + offsetshift[0]) >> offsetshift[1],0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas12across_addc_rshift_u8_ref,
    mas12across_addc_rshift_u8);
#endif

static void
mas8_addc_rshift_decim2_u8_ref (uint8_t *dest, const uint8_t *src,
    const int16_t *taps, const int16_t *offsetshift, int n)
{
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    x = 0;
    for(j=0;j<8;j++){
      x += taps[j]*src[i*2 + j];
    }
    dest[i] = CLAMP((x + offsetshift[0]) >> offsetshift[1],0,255);
  }
}
OIL_DEFINE_IMPL_REF (mas8_addc_rshift_decim2_u8_ref,
    mas8_addc_rshift_decim2_u8);

