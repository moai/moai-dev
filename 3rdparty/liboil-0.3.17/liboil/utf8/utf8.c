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
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include "utf8.h"


/*
 * Little explanation:
 *  0x00-0x7f  ASCII, one byte character
 *  0x80-0xbf  continuation byte, not a valid start byte
 *  0xc0-0xdf  2-byte character
 *  0xe0-0xef  3-byte character
 *  0xf0-0xf7  4-byte character
 *  0xf8-0xff  reserved (illegal at the present time)
 */
static void
utf8_validate_test (OilTest *test)
{
  int i;
  int n = test->n;
  uint8_t *ptr = oil_test_get_source_data (test, OIL_ARG_SRC1);
  int x;
  int extra_chars = 0;

  for (i=0;i<n;i++){
    if (i >= n-16) {
      /* if it's close to the end, we'll randomly drop in a bad
       * byte from either the 0x80-0xbf or 0xf8-0xff segments */
      x = oil_rand_u8();
      if (x < 16) {
        x = oil_rand_u8();
        if (extra_chars>0) {
          /* this might not actually be a bad char */
          ptr[i] = x;
          extra_chars--;
        } else {
          if (x & 0x80) {
            ptr[i] = 0x80 | (x&0x3f);
          } else {
            ptr[i] = 0xf8 | (x&0x07);
          }
        }
        continue;
      }
    }
    if (extra_chars > 0) {
      ptr[i] = 0x80 | (oil_rand_u8() & 0x3f);
      extra_chars--;
    } else {
      /* otherwise, we'll do a low probability of a multibyte char */
      x = oil_rand_u8() & 0xf;
      if (x == 0) {
        ptr[i] = 0xc0 | (oil_rand_u8() & 0x1f);
        extra_chars = 1;
      } else if (x == 1) {
        ptr[i] = 0xe0 | (oil_rand_u8() & 0x0f);
        extra_chars = 2;
      } else if (x == 2) {
        ptr[i] = 0xf0 | (oil_rand_u8() & 0x07);
        extra_chars = 3;
      } else {
        ptr[i] = oil_rand_u8() & 0x7f;
      }
    }
  }

}

/**
 * oil_utf8_validate:
 * @d_1:
 * @s:
 * @n:
 *
 * Checks @s for valid UTF-8 characters.  If the entire @s array
 * represents valid UTF-8 characters, @n is written to @d_1.
 * Otherwise, the index in the array of the beginning of the first
 * invalid UTF-8 character is written to @d_1.
 */
OIL_DEFINE_CLASS_FULL (utf8_validate, "int32_t *d_1, uint8_t *s, int n",
    utf8_validate_test);


static void
utf8_validate_ref (int32_t *d_1, uint8_t *s, int n)
{
  int i;
  int extra_bytes;
  int mask;

  for(i=0;i<n;i++){
    extra_bytes = 0;
    if (s[i] < 128) continue;
    if ((s[i] & 0xe0) == 0xc0) {
      extra_bytes = 1;
      mask = 0x7f;
    } else if ((s[i] & 0xf0) == 0xe0) {
      extra_bytes = 2;
      mask = 0x1f;
    } else if ((s[i] & 0xf8) == 0xf0) {
      extra_bytes = 3;
      mask = 0x0f;
    } else {
      goto error;
    }
    if (i + extra_bytes >= n) goto error;
    while(extra_bytes--) {
      i++;
      if ((s[i] & 0xc0) != 0x80) goto error;
    }
  }

error:
  d_1[0] = i;
}

OIL_DEFINE_IMPL_REF (utf8_validate_ref, utf8_validate);

