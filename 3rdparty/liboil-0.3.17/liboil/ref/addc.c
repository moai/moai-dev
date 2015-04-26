
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>

static void
shift_test (OilTest *test)
{
  int16_t *data;
  int i;
  int n;

if (0) {
  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC1);
  n = oil_test_get_arg_post_n (test, OIL_ARG_SRC1);
  for(i=0;i<n;i++){
    data[i] = CLAMP(data[i], -4096, 4095);
  }
}

  data = (int16_t *)oil_test_get_source_data (test, OIL_ARG_SRC2);
  //data[0] = 10000;
  data[1] = 8;
}

OIL_DEFINE_CLASS (addc_s16,
    "int16_t *d1, int16_t *s1, int16_t *s2_1, int n");
OIL_DEFINE_CLASS_FULL (addc_rshift_s16,
    "int16_t *d1, int16_t *s1, int16_t *s2_2, int n", shift_test);

void
addc_rshift_s16_ref (int16_t *d1, int16_t *s1, int16_t *s2_2, int n)
{
  int i;
  int16_t x;

  for(i=0;i<n;i++){
    x = s1[i] + s2_2[0];
    d1[i] = x>>s2_2[1];
  }
}
OIL_DEFINE_IMPL_REF (addc_rshift_s16_ref, addc_rshift_s16);

void
addc_s16_ref (int16_t *d1, uint16_t *s1, int16_t *s2_1, int n)
{
  int i;
  for(i=0;i<n;i++){
    d1[i] = s1[i] + s2_1[0];
  }
}
OIL_DEFINE_IMPL_REF (addc_s16_ref, addc_s16);

