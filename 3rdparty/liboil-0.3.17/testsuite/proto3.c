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


#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

void print_param (OilParameter *param);

/* format:
 * <isd>[s][0-9*][_[<0-9*,nm[p0-9*]>x]<0-9*,nm[p0-9*]>] */

char *good_params[] = {
  "d",
  "s",
  "i",
  "d1",
  "d2",
  "ds",
  "ss",
  "is",
  "ds1",
  "ds2",
  "d_1",
  "d_2",
  "d_4",
  "d_n",
  "d_1xn",
  "d_4xn",
  "d_nxm",
  "d_8x8",
  "d_np2",
  NULL
};

char *bad_params[] = {
  "e",
  NULL
};

int main (int argc, char *argv[])
{
  int i;
  int ret;
  int failed = 0;
  OilParameter param;

  for(i=0;good_params[i];i++){
    ret = oil_param_from_string (&param, good_params[i]);
    if (!ret) {
      printf("***ERROR***\n");
      failed = 1;
    }
    print_param (&param);
  }

  for(i=0;bad_params[i];i++){
    ret = oil_param_from_string (&param, bad_params[i]);
    if (ret) {
      printf("***ERROR***\n");
      failed = 1;
    }
  }

  return failed;
}

void print_param (OilParameter *param)
{
  if (param->is_stride) {
    printf ("  %cs%d\n", param->direction, param->index);
  } else {
    printf ("  %c%d_", param->direction, param->index);
    if (param->prestride_var > 0) {
      printf("%c", (param->prestride_var==1) ? 'n' : 'm');
      if (param->prestride_length) {
        printf("p%d", param->prestride_length);
      }
    } else {
      printf("%d", param->prestride_length);
    }
    printf("x");
    if (param->poststride_var > 0) {
      printf("%c", (param->poststride_var==1) ? 'n' : 'm');
      if (param->poststride_length) {
        printf("p%d", param->poststride_length);
      }
    } else {
      printf("%d", param->poststride_length);
    }
    printf("\n");
  }

}

