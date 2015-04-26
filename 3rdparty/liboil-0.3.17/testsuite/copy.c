
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <stdio.h>
#include <string.h>

uint8_t dest[200];
uint8_t src[200];


void test(void)
{
  int i;

  for(i=0;i<200;i++){
    dest[i]=0;
    src[i]=i;
  }

  oil_copy_u8 (dest + 8, src + 0, 64);

  for(i=0;i<100;i++){
    printf("%d\n",dest[i]);
  }
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("copy_u8");
  oil_class_optimize(klass);

  oil_class_choose_by_name (klass, "copy_u8_altivec");
  impl = klass->chosen_impl;
  if (oil_impl_is_runnable (impl)) {
    printf("chosen=%p\n", impl);
    impl = klass->reference_impl;
    printf("ref=%p\n", impl);
    test();
  }

  return 0;
}

