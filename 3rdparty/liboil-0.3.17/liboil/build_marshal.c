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

void print_header (void);
void print_footer (void);
void add_pointer_mask (unsigned int mask);

unsigned int proto_masks[1000];
int n_for_mask[1000];
int n_proto_masks;

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilPrototype *proto;
  int i;
  int j;
  int n;
  unsigned int pointer_mask;

  oil_init_no_optimize ();

  print_header ();

  n = oil_class_get_n_classes ();
  for (i=0;i<n; i++ ){
    klass = oil_class_get_by_index (i);

    if(klass->prototype) {
      proto = oil_prototype_from_string (klass->prototype);
      if (proto) {
        pointer_mask = 1;

        for (j=0;j<proto->n_params;j++){
          pointer_mask <<= 1;
          if (proto->params[j].is_pointer) pointer_mask |= 1;
        }

        add_pointer_mask (pointer_mask);

        oil_prototype_free (proto);
      } else {
        printf("/* ERROR: could not parse %s(%s) */\n", klass->name, klass->prototype);
      }
    }
  }

  for(i=0;i<n_proto_masks;i++){
    unsigned int bit;
    unsigned int hibit;

    pointer_mask = proto_masks[i];
    for(hibit=1;hibit<=pointer_mask;hibit<<=1);
    hibit>>=2;

    printf("    case 0x%04x:\n", pointer_mask);
    printf("      oil_profile_start (prof);\n");
    printf("      ((void (*)(");
    if(hibit == 0) {
      printf("void");
    } else {
      for(bit=hibit;bit;bit >>= 1) {
        if (pointer_mask & bit) {
          printf("void *");
        } else {
          printf("int");
        }
        if (bit > 1) {
          printf(",");
        }
      }
    }
    printf("))func)\n");
    printf("        (");
    j=0;
    for(bit=hibit;bit;bit >>= 1) {
      if (pointer_mask & bit) {
        printf("(void *)args[%d]", j);
      } else {
        printf("(int)args[%d]", j);
      }
      if (bit > 1) {
        printf(",");
      }
      j++;
    }
    printf(");\n");
    printf("      oil_profile_stop (prof);\n");
    printf("      break;\n");
  }

  print_footer ();

  return 0;
}

void
add_pointer_mask (unsigned int mask)
{
  int i;
  for(i=0;i<n_proto_masks;i++){
    if (proto_masks[i] == mask) {
      n_for_mask[i]++;
      return;
    }
  }
  proto_masks[n_proto_masks] = mask;
  n_for_mask[n_proto_masks]++;
  n_proto_masks++;
}

void print_header (void)
{
  printf ("/*\n");
  printf (" * LIBOIL - Library of Optimized Inner Loops\n");
  printf (" * Copyright (c) 2004 David A. Schleef <ds@schleef.org>\n");
  printf (" * All rights reserved.\n");
  printf (" *\n");
  printf (" * Redistribution and use in source and binary forms, with or without\n");
  printf (" * modification, are permitted provided that the following conditions\n");
  printf (" * are met:\n");
  printf (" * 1. Redistributions of source code must retain the above copyright\n");
  printf (" *    notice, this list of conditions and the following disclaimer.\n");
  printf (" * 2. Redistributions in binary form must reproduce the above copyright\n");
  printf (" *    notice, this list of conditions and the following disclaimer in the\n");
  printf (" *    documentation and/or other materials provided with the distribution.\n");
  printf (" * \n");
  printf (" * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR\n");
  printf (" * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n");
  printf (" * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n");
  printf (" * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,\n");
  printf (" * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n");
  printf (" * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n");
  printf (" * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n");
  printf (" * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n");
  printf (" * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\n");
  printf (" * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n");
  printf (" * POSSIBILITY OF SUCH DAMAGE.\n");
  printf (" */\n");
  printf ("\n");
  printf ("/* This file is automatically generated.  Do not edit. */\n");
  printf ("\n");
  printf ("#ifdef HAVE_CONFIG_H\n");
  printf ("#include <config.h>\n");
  printf ("#endif\n");
  printf ("\n");
  printf ("#include <liboil/liboiltest.h>\n");
  printf ("#include <liboil/liboildebug.h>\n");
  printf ("#include <liboil/liboilprofile.h>\n");
  printf ("\n");
  printf ("void\n");
  printf ("_oil_test_marshal_function (void *func, unsigned long *args, int n_args,\n");
  printf ("    unsigned int pointer_mask, OilProfile *prof)\n");
  printf ("{\n");
  printf ("  switch (pointer_mask) {\n");
}

void print_footer (void)
{
  printf ("    default:\n");
  printf ("      OIL_ERROR (\"unhandled marshal case\");\n");
  printf ("  }\n");
  printf ("}\n");
  printf ("\n");
}

