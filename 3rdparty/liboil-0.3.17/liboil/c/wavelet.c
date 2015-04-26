
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


void
multiply_and_acc_6xn_s16_u8_c (int16_t *i1, int is1, int16_t *s1,
    int ss1, uint8_t *s2, int ss2, int n)
{
  int j;
  for(j=0;j<n;j++){
    i1[0] += s1[0]*s2[0];
    i1[1] += s1[1]*s2[1];
    i1[2] += s1[2]*s2[2];
    i1[3] += s1[3]*s2[3];
    i1[4] += s1[4]*s2[4];
    i1[5] += s1[5]*s2[5];

    i1 = OIL_OFFSET(i1,is1);
    s1 = OIL_OFFSET(s1,ss1);
    s2 = OIL_OFFSET(s2,ss2);
  }
}
OIL_DEFINE_IMPL (multiply_and_acc_6xn_s16_u8_c,
    multiply_and_acc_6xn_s16_u8);

