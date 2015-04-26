
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboil.h>
#include <liboil/liboilprofile.h>
#include <liboil/liboilfunction.h>

#define ALIGN(ptr,n) ((void *)((unsigned long)(ptr) & (~(unsigned long)(n-1))))

int
main(int argc, char *argv[])
{
  char *s, *d;
  uint32_t *src, *dest;
  OilProfile prof;
  double ave, std;
  int i,j;
  double cpufreq;
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  int the_class;

  the_class = 2;

  oil_init ();

  cpufreq = 1788e6;

  s = malloc(1024*1024*64+1024);
  d = malloc(1024*1024*64+1024);
  src = ALIGN(s,128);
  dest = ALIGN(d,128);

  switch(the_class) {
    case 0:
      klass = oil_class_get ("splat_u32_ns");
      break;
    case 1:
      klass = oil_class_get ("copy_u8");
      break;
    case 2:
      klass = oil_class_get ("sum_s16");
      break;
  }

  for(impl=klass->first_impl;impl;impl=impl->next) {
    printf("impl %s\n", impl->name);

    if (!oil_impl_is_usable(impl)) {
      printf("  not usable\n");
      continue;
    }

    oil_class_choose_by_name (klass, impl->name);

    for(i=10;i<24;i++){
      oil_profile_init (&prof);
      for(j=0;j<10;j++){
        switch(the_class) {
          case 0:
            oil_profile_start(&prof);
            oil_splat_u32_ns (dest, src, 1<<(i-2));
            oil_profile_stop(&prof);
            break;
          case 1:
            oil_profile_start(&prof);
            oil_memcpy (dest, src, 1<<i);
            oil_profile_stop(&prof);
            break;
          case 2:
            oil_profile_start(&prof);
            oil_sum_s16 ((int16_t *)dest, (int16_t *)src, 1<<(i-1));
            oil_profile_stop(&prof);
            break;
        }
      }

      oil_profile_get_ave_std (&prof, &ave, &std);

      printf("%d: %10.4g %10.4g %10.4g %10.4g\n", i, ave, std,
          ave/(1<<i), cpufreq/(ave/(1<<i)));
    }
  }

  return 0;
}

