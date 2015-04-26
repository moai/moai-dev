/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilprofile.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

char *message = "liboil md5 test\n";
//char *message = "";

#ifdef WORDS_BIGENDIAN
#define uint32_from_host(a) \
  ((((a)&0xff)<<24)|(((a)&0xff00)<<8)|(((a)&0xff0000)>>8)|(((a)>>24)&0xff))
#else
#define uint32_from_host(a) (a)
#endif

void test(void)
{
  int i;
  uint32_t state[4];
  uint32_t src[16];
  int len;
  OilProfile prof;

  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;

  memset (src, 0, 64);
  len = strlen (message);
  memcpy (src, message, len);
  ((uint8_t *)src)[len] = 0x80;
  src[14] = uint32_from_host(len << 3);
  src[15] = 0;

  oil_profile_init(&prof);
  for(i=0;i<1000;i++){
    oil_profile_start(&prof);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_md5 (state, src);
    oil_profile_stop(&prof);
  }
  //oil_profile_print(&prof);

  printf("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
      state[0]&0xff, (state[0]>>8)&0xff, (state[0]>>16)&0xff,
      (state[0]>>24)&0xff,
      state[1]&0xff, (state[1]>>8)&0xff, (state[1]>>16)&0xff,
      (state[1]>>24)&0xff,
      state[2]&0xff, (state[2]>>8)&0xff, (state[2]>>16)&0xff,
      (state[2]>>24)&0xff,
      state[3]&0xff, (state[3]>>8)&0xff, (state[3]>>16)&0xff,
      (state[3]>>24)&0xff);
}

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;

  oil_init ();

  klass = oil_class_get ("md5");
  oil_class_optimize(klass);

  printf("class=%s\n", klass->name);
  for (impl = klass->first_impl; impl; impl=impl->next) {
    printf("impl=%p\n", impl);
    printf("  func=%p\n", impl->func);
    printf("  name=%s\n", impl->name);
    printf("  flags=%08x\n", impl->flags);
  }

  oil_class_choose_by_name (klass, "md5_c");
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);
  test();

  oil_class_choose_by_name (klass, "md5_asm1");
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);
  test();

  oil_class_choose_by_name (klass, "md5_asm2");
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);
  test();

  oil_class_choose_by_name (klass, "md5_asm3");
  impl = klass->chosen_impl;
  printf("chosen=%p\n", impl);
  impl = klass->reference_impl;
  printf("ref=%p\n", impl);
  test();

  return 0;
}

