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
#include <liboil/liboilfunction.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void parse_prototype (const char *s);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilFunctionImpl *impl;
  int i;
  int n;
  int errors = 0;

  oil_init ();

  n = oil_class_get_n_classes ();
  for (i=0;i<n; i++ ){
    int ref=0;

    klass = oil_class_get_by_index (i);

    printf ("class: %s\n", klass->name);
    for(impl = klass->first_impl; impl; impl=impl->next) {
      printf ("  %s %s %s\n", impl->name,
          (impl->flags&OIL_IMPL_FLAG_REF)?"(ref)":"",
          (impl->flags&OIL_IMPL_FLAG_OPT)?"(opt)":"");
      if (impl->flags & OIL_IMPL_FLAG_REF) {
        ref++;
      }
    }
    if (ref < 1) {
      printf("ERROR: no reference function\n");
      errors++;
    }
    if (ref > 1) {
      printf("ERROR: more than one reference function\n");
      errors++;
    }
    if (klass->prototype == NULL) {
      printf("ERROR: prototype is NULL\n");
      errors++;
    }
#if 0
    printf ("#define %s ((void (*)(%s)) \\\n\t_oil_function_%s_class.func)\n",
        klass->name, klass->prototype, klass->name);
    //printf("void %s (%s);\n", klass->name, klass->prototype);
    parse_prototype(klass->prototype);
#endif
  }

  return (errors)?1:0;
}

#ifdef unused
static char *typenames[] = {
  "type_s8",
  "type_u8",
  "type_s16",
  "type_u16",
  "type_s32",
  "type_u32",
  "type_f32",
  "type_f64",
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "float",
  "double",
  "int",
};

static int parse_type (char *s, char **endptr)
{
  int i;

  while(isspace((int)*s))s++;

  for(i=0;i<8;i++){
    if(strncmp(typenames[i],s,strlen(typenames[i]))==0){
      *endptr = s + strlen(typenames[i]);
      return OIL_TYPE_s8 + i;
    }
  }

  return OIL_TYPE_UNKNOWN;
}

static int parse_size (const char *s, char **endptr)
{
  while(isspace((int)*s))s++;

  if(s[0] == 'n'){
    *endptr = (char *)(s + 1);
    return 0;
  }
  if(isdigit((int)s[0])){
    return strtol(s,endptr,0);
  }

  return -1;
}
#endif

char *
xstrndup (const char *s, int n)
{
  char *r;

  if (strlen(s) < n) {
    n = strlen(s);
  }
  r = malloc(n+1);
  memcpy(r,s,n);
  r[n] = 0;

  return r;
}

static char * parse_string (const char *s, const char **endptr)
{
  const char *s0;

  s0 = s;
  while(isalnum((int)*s) || *s=='_') {
    s++;
  }
  *endptr = s;

  return xstrndup(s0, s - s0);
}

void parse_prototype (const char *s)
{
  char *type;
  char *name;
  int ptr = 0;

  while (isspace((int)*s))s++;
  while (*s) {
    type = parse_string (s, &s);

    while (isspace((int)*s))s++;

    if(s[0] == '*'){
      ptr = 1;
      s++;
    }
    while (isspace((int)*s))s++;
    name = parse_string (s, &s);

    while (isspace((int)*s))s++;

    if(s[0] == ','){
      s++;
    }
    while (isspace((int)*s))s++;

    printf("%s %s\n", type, name);
  }
}

