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

static OilParameter * get_param_by_type (OilPrototype *proto,
    OilArgType ptype);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  int i;
  int j;
  int n;

  oil_init ();

  n = oil_class_get_n_classes ();
  for (i=0;i<n; i++ ){
    klass = oil_class_get_by_index(i);

    if(klass->prototype) {
      OilPrototype *proto;
      OilParameter params[OIL_ARG_LAST];
      OilParameter *p;

      memset (params, 0, sizeof(params));
      proto = oil_prototype_from_string (klass->prototype);

      printf("%s\n", klass->name);
      if (proto) {
        for(j=0;j<proto->n_params;j++){

          printf("  (%s) (%s) %d %d [%s] [%s]\n",
              proto->params[j].type_name,
              proto->params[j].parameter_name,
              proto->params[j].type,
              proto->params[j].parameter_type,
              oil_type_name (proto->params[j].type),
              oil_arg_type_name (proto->params[j].parameter_type));
          if (proto->params[j].parameter_type == OIL_ARG_UNKNOWN) {
            printf("unknown parameter type\n");
          }
        }

        for(j=0;j<proto->n_params;j++){
          memcpy (&params[proto->params[j].parameter_type], &proto->params[j],
              sizeof (OilParameter));
        }
        /* n */
        p = get_param_by_type (proto, OIL_ARG_N);
        if (p) {
          printf("  n: yes\n");
        } else {
          printf("  n: no\n");
        }

        oil_prototype_free (proto);
      } else {
        printf("  bad prototype\n");
      }

    }
  }

  return 0;
}


static OilParameter *
get_param_by_type (OilPrototype *proto, OilArgType ptype)
{
  int i;
  for(i=0;i<proto->n_params;i++){
    if (proto->params[i].parameter_type == ptype) {
      return &proto->params[i];
    }
  }
  return NULL;
}

