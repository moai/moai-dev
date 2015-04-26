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

#include <stdio.h>
#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef MAP_POPULATE
#define MAP_POPULATE 0
#endif

#ifdef WORDS_BIGENDIAN
#define uint32_from_host(a) \
    ((((a)&0xff)<<24)|(((a)&0xff00)<<8)|(((a)&0xff0000)>>8)|(((a)>>24)&0xff))
#else
#define uint32_from_host(a) (a)
#endif

#if 0
int main(int argc, char *argv[])
{
  int n;
  uint64_t n_bytes = 0;
  char buffer[64];
  uint32_t state[4];

  oil_init();

  if (argc > 1) {
    OilFunctionClass *klass;

    klass = oil_class_get ("md5");
    oil_class_choose_by_name (klass, argv[1]);
  }

  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;

  while (1) {
    n = fread (buffer, 1, 64, stdin);
    if (n == 64) {
      oil_md5 (state, (uint32_t *)buffer);
      n_bytes += 64;
    } else {
      break;
    }
  }
  
  n_bytes += n;
  buffer[n] = 0x80;
  n++;
  if (n >= 56) {
    for(;n<64;n++) buffer[n] = 0;
    oil_md5 (state, (uint32_t *)buffer);
    n=0;
  }

  for(;n<56;n++) buffer[n] = 0;

  *(uint64_t *)(buffer + 56) = n_bytes << 3;
  oil_md5 (state, (uint32_t *)buffer);

  printf("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
      state[0]&0xff, (state[0]>>8)&0xff, (state[0]>>16)&0xff,
      (state[0]>>24)&0xff,
      state[1]&0xff, (state[1]>>8)&0xff, (state[1]>>16)&0xff,
      (state[1]>>24)&0xff,
      state[2]&0xff, (state[2]>>8)&0xff, (state[2]>>16)&0xff,
      (state[2]>>24)&0xff,
      state[3]&0xff, (state[3]>>8)&0xff, (state[3]>>16)&0xff,
      (state[3]>>24)&0xff);

  return 0;
}
#endif

int main(int argc, char *argv[])
{
  int n;
  uint64_t n_bytes;
  char buffer[64];
  uint32_t state[4];
  char *ptr;
  int ret;
  int fd;
  struct stat st;
  int offset;

  oil_init();

  if (argc < 1) {
    printf("md5sum <filename> [implementation]\n");
    exit(0);
  }

  if (argc > 2) {
    OilFunctionClass *klass;

    klass = oil_class_get ("md5");
    oil_class_choose_by_name (klass, argv[2]);
  }

  fd = open (argv[1], O_RDONLY);
  if (fd < 0) {
    printf("could not open file\n");
    exit(0);
  }

  ret = fstat (fd, &st);

  n_bytes = st.st_size;
#ifdef HAVE_MMAP
  while (1) {
    ptr = mmap (NULL, n_bytes, PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0);
    getpid();
    if (ptr == MAP_FAILED) {
      if (errno == EAGAIN) {
        printf("egain\n");

      } else {
        perror ("mmap failed");
        exit(1);
      }
    } else {
      break;
    }
  }
#else
  printf ("no mmap\n");
  exit(1);
#endif

  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;

  for (offset = 0; offset + 64 <= n_bytes; offset += 64) {
    oil_md5 (state, (uint32_t *)(ptr + offset));
  }

  n = n_bytes - offset;
  memcpy (buffer, ptr + offset, n);

  buffer[n] = 0x80;
  n++;
  if (n >= 56) {
    for(;n<64;n++) buffer[n] = 0;
    oil_md5 (state, (uint32_t *)buffer);
    n=0;
  }

  for(;n<56;n++) buffer[n] = 0;

  *(uint32_t *)(buffer + 56) = uint32_from_host (n_bytes << 3);
  *(uint32_t *)(buffer + 60) = uint32_from_host (n_bytes >>29);
  oil_md5 (state, (uint32_t *)buffer);

  printf("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
      state[0]&0xff, (state[0]>>8)&0xff, (state[0]>>16)&0xff,
      (state[0]>>24)&0xff,
      state[1]&0xff, (state[1]>>8)&0xff, (state[1]>>16)&0xff,
      (state[1]>>24)&0xff,
      state[2]&0xff, (state[2]>>8)&0xff, (state[2]>>16)&0xff,
      (state[2]>>24)&0xff,
      state[3]&0xff, (state[3]>>8)&0xff, (state[3]>>16)&0xff,
      (state[3]>>24)&0xff);

  return 0;
}

