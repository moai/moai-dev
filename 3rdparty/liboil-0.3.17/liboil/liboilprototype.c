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
#include <config.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>
#include <liboil/liboildebug.h>

/**
 * SECTION:liboilprototype
 * @title:OilPrototype
 * @short_description:
 * Functions to manipulate prototype information.
 *
 */

/**
 * SECTION:liboilparameter
 * @title: OilParameter
 * @short_description:
 * Functions to manipulate prototype parameter information.
 *
 */

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct _OilString OilString;
struct _OilString {
  int len;
  int alloc_len;
  char *data;
};

static OilType oil_type_from_string (const char *s, int ptr);

static OilString *oil_string_new (const char *a);
static void oil_string_append (OilString *s, const char *a);
static char * oil_string_free (OilString *s, int free_seg);

static char * parse_string (const char *s, const char **endptr);

static int oil_prototype_check_sanity (OilPrototype *proto);

/**
 * oil_prototype_append_param:
 * @proto: the OilPrototype
 * @param: an OilParameter
 *
 * Appends @param to the list of parameters contained in @proto.  The
 * contents of @param are copied.
 */
void
oil_prototype_append_param (OilPrototype *proto, OilParameter *param)
{
  proto->n_params++;
  proto->params = realloc(proto->params,
      sizeof(OilParameter) * proto->n_params);
  proto->params[proto->n_params - 1] = *param;
}

/**
 * oil_prototype_to_string:
 * @proto: the OilPrototype
 *
 * Converts a prototype into the corresponding C style declaration.
 *
 * Returns: a string that should be freed using free() when it is no
 * longer needed.
 */
char *
oil_prototype_to_string (OilPrototype *proto)
{
  OilString *string;
  OilParameter *param;
  int i;

  string = oil_string_new ("");

  for(i=0;i<proto->n_params;i++){
    param = proto->params + i;

    if (param->parameter_type == OIL_ARG_SRC1 ||
        param->parameter_type == OIL_ARG_SRC2 ||
        param->parameter_type == OIL_ARG_SRC3 ||
        param->parameter_type == OIL_ARG_SRC4 ||
        param->parameter_type == OIL_ARG_SRC5) {
      oil_string_append (string, "const ");
    }

    if (param->type != OIL_TYPE_UNKNOWN) {
      oil_string_append (string, oil_type_name (param->type));
    } else {
      oil_string_append (string, param->type_name);
    }
    oil_string_append (string, " ");
    oil_string_append (string, param->parameter_name);

    if(i<proto->n_params-1){
      oil_string_append (string, ", ");
    }
  }

  return oil_string_free (string, FALSE);
}

/**
 * oil_prototype_to_arg_string:
 * @proto: the OilPrototype
 *
 * Converts a prototype into the corresponding C argument list.
 *
 * Returns: a string that should be freed using free() when it is no
 * longer needed.
 */
char *
oil_prototype_to_arg_string (OilPrototype *proto)
{
  OilString *string;
  OilParameter *param;
  int i;

  string = oil_string_new ("");

  for(i=0;i<proto->n_params;i++){
    param = proto->params + i;

    oil_string_append (string, param->parameter_name);

    if(i<proto->n_params-1){
      oil_string_append (string, ", ");
    }
  }

  return oil_string_free (string, FALSE);
}

/**
 * oil_prototype_from_string:
 * @s: a string
 *
 * Converts the string @s containing C prototype that follows
 * Liboil parameter naming rules into a OilPrototype.  If the
 * string cannot be converted, NULL is returned.
 *
 * Returns: the OilPrototype.  When it is no longer needed, free
 * the prototype using oil_prototype_free().
 */
OilPrototype *
oil_prototype_from_string (const char *s)
{
  OilPrototype *proto;
  OilParameter param = { 0 };
  char *type_name;
  char *parameter_name;
  int ptr;
  int order = 0;

  proto = malloc (sizeof(OilPrototype));
  memset (proto, 0, sizeof(OilPrototype));

  while (isspace((int)*s))s++;
  while (*s) {
    type_name = parse_string (s, &s);

    while (isspace((int)*s))s++;

    ptr = FALSE;
    if(s[0] == '*'){
      ptr = TRUE;
      s++;
    }
    while (isspace((int)*s))s++;
    parameter_name = parse_string (s, &s);

    while (isspace((int)*s))s++;

    if(s[0] == ','){
      s++;
    }
    while (isspace((int)*s))s++;

    param.type = oil_type_from_string (type_name, ptr);
    param.type_name = type_name;
    param.parameter_name = parameter_name;
    oil_param_from_string (&param, parameter_name);
    param.order = order;
    oil_prototype_append_param (proto, &param);

    if (param.type == OIL_TYPE_UNKNOWN ||
        param.parameter_type == OIL_ARG_UNKNOWN) {
      OIL_ERROR ("bad type (%s) or parameter (%s)", type_name, parameter_name);
      oil_prototype_free (proto);
      return NULL;
    }

    order++;
  }

  if (!oil_prototype_check_sanity (proto)) {
    OIL_ERROR ("prototype failed sanity check");
    oil_prototype_free (proto);
    return NULL;
  }

  return proto;
}

static int
oil_prototype_check_sanity (OilPrototype *proto)
{
  int i;
  int has_n;

  has_n = 0;
  for (i=0;i<proto->n_params;i++){
    if (proto->params[i].parameter_type == OIL_ARG_N) {
      has_n = 1;
    }
  }

  if (has_n == 0) {
    /* check that no parameter depends on n */
    for(i=0;i<proto->n_params;i++){
      if (proto->params[i].is_stride) continue;
      if (proto->params[i].parameter_type == OIL_ARG_N) continue;
      if (proto->params[i].parameter_type == OIL_ARG_M) continue;

      if (proto->params[i].prestride_var == 1) return 0;
      if (proto->params[i].poststride_var == 1) return 0;
    }
  }

  return 1;
}

static char * parse_string (const char *s, const char **endptr)
{
  const char *s0;

  s0 = s;
  while(isalnum((int)*s) || *s=='_') {
    s++;
  }
  *endptr = s;

  return _strndup(s0, s - s0);
}

/**
 * oil_prototype_free:
 * @proto: the OilPrototype
 *
 * Frees memory associated with @proto.
 */
void
oil_prototype_free (OilPrototype *proto)
{
  int i;

  if (proto->params) {
    for(i=0;i<proto->n_params;i++) {
      if (proto->params[i].parameter_name) {
        free (proto->params[i].parameter_name);
      }
      if (proto->params[i].type_name) {
        free (proto->params[i].type_name);
      }
    }
    free (proto->params);
  }
  free (proto);
}



/* OilString */

static OilString *
oil_string_new (const char *a)
{
  OilString *s;

  s = malloc (sizeof(OilString));
  memset (s, 0, sizeof(OilString));

  if (a) {
    oil_string_append (s, a);
  }

  return s;

}

static void
oil_string_append (OilString *s, const char *a)
{
  int len = strlen (a);

  if (s->len + len + 1 > s->alloc_len) {
    s->alloc_len = s->len + len + 1;
    s->data = realloc (s->data, s->alloc_len);
  }
  memcpy (s->data + s->len, a, len + 1);
  s->len += len;
}

static char *
oil_string_free (OilString *s, int free_seg)
{
  char *data = s->data;
  free(s);
  if (free_seg) {
    free (data);
    return NULL;
  }
  return data;
}


/**
 * oil_type_sizeof:
 * @type: the OilType
 *
 * Looks up the size of the OilType represented by @type.
 *
 * Returns: the size of @type
 */
int oil_type_sizeof (OilType type)
{
  switch (type) {
    case OIL_TYPE_UNKNOWN:
    case OIL_TYPE_INT:
      return 0;
    case OIL_TYPE_s8:
    case OIL_TYPE_u8:
    case OIL_TYPE_s8p:
    case OIL_TYPE_u8p:
      return 1;
    case OIL_TYPE_s16:
    case OIL_TYPE_u16:
    case OIL_TYPE_s16p:
    case OIL_TYPE_u16p:
      return 2;
    case OIL_TYPE_s32:
    case OIL_TYPE_u32:
    case OIL_TYPE_f32:
    case OIL_TYPE_s32p:
    case OIL_TYPE_u32p:
    case OIL_TYPE_f32p:
      return 4;
    case OIL_TYPE_s64:
    case OIL_TYPE_u64:
    case OIL_TYPE_f64:
    case OIL_TYPE_s64p:
    case OIL_TYPE_u64p:
    case OIL_TYPE_f64p:
      return 8;
  }
  return 0;
}

static const char *oil_type_names [] = {
  "unknown",
  "int",
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "int64_t",
  "uint64_t",
  "float",
  "double",
  "int8_t *",
  "uint8_t *",
  "int16_t *",
  "uint16_t *",
  "int32_t *",
  "uint32_t *",
  "int64_t *",
  "uint64_t *",
  "float *",
  "double *"
};
static const char *oil_type_names_2 [] = {
  "int8_t",
  "uint8_t",
  "int16_t",
  "uint16_t",
  "int32_t",
  "uint32_t",
  "int64_t",
  "uint64_t",
  "float",
  "double"
};
static const char *oil_type_names_3 [] = {
  "oil_type_s8",
  "oil_type_u8",
  "oil_type_s16",
  "oil_type_u16",
  "oil_type_s32",
  "oil_type_u32",
  "oil_type_s64",
  "oil_type_u64",
  "oil_type_f32",
  "oil_type_f64"
};
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

/**
 * oil_type_name:
 * @type: an OilType
 *
 * Looks up the name of the OilType given by @type.
 *
 * Returns: a string containing the name of the OilType @type.
 */
const char * oil_type_name (OilType type)
{

  if (type < 0 || type >= ARRAY_LENGTH(oil_type_names))
    return "unknown";

  return oil_type_names[type];
}

static OilType oil_type_from_string (const char *s, int ptr)
{
  int i;

  if (ptr) {
    for(i=0;i<ARRAY_LENGTH(oil_type_names_2);i++){
      if (strcmp(s,oil_type_names_2[i]) == 0){
        return OIL_TYPE_s8p + i;
      }
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_3);i++){
      if (strcmp(s,oil_type_names_3[i]) == 0){
        return OIL_TYPE_s8p + i;
      }
    }
  } else {
    if (strcmp (s,"int") == 0) {
      return OIL_TYPE_INT;
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_2);i++){
      if (strcmp(s,oil_type_names_2[i]) == 0){
        return OIL_TYPE_s8 + i;
      }
    }
    for(i=0;i<ARRAY_LENGTH(oil_type_names_3);i++){
      if (strcmp(s,oil_type_names_3[i]) == 0){
        return OIL_TYPE_s8 + i;
      }
    }
  }
  return OIL_TYPE_UNKNOWN;
}

/**
 * oil_arg_type_name:
 * @type: an OilArgType
 *
 * Looks up the name of the OilArgType specified by @type.
 *
 * Returns: a string containing the name
 */
const char * oil_arg_type_name (OilArgType type)
{
  switch (type) {
    case OIL_ARG_N:
      return "n";
    case OIL_ARG_M:
      return "m";
    case OIL_ARG_DEST1:
      return "dest1";
    case OIL_ARG_DSTR1:
      return "dstr1";
    case OIL_ARG_DEST2:
      return "dest2";
    case OIL_ARG_DSTR2:
      return "dstr2";
    case OIL_ARG_DEST3:
      return "dest3";
    case OIL_ARG_DSTR3:
      return "dstr3";
    case OIL_ARG_SRC1:
      return "src1";
    case OIL_ARG_SSTR1:
      return "sstr1";
    case OIL_ARG_SRC2:
      return "src2";
    case OIL_ARG_SSTR2:
      return "sstr2";
    case OIL_ARG_SRC3:
      return "src3";
    case OIL_ARG_SSTR3:
      return "sstr3";
    case OIL_ARG_SRC4:
      return "src4";
    case OIL_ARG_SSTR4:
      return "sstr4";
    case OIL_ARG_SRC5:
      return "src5";
    case OIL_ARG_SSTR5:
      return "sstr5";
    case OIL_ARG_INPLACE1:
      return "inplace1";
    case OIL_ARG_ISTR1:
      return "istr1";
    case OIL_ARG_INPLACE2:
      return "inplace2";
    case OIL_ARG_ISTR2:
      return "istr2";
    default:
      return "INVALID";
  }
}

static const
struct {
  char *from;
  char *to;
} arg_aliases[] = {
  { "dest", "d1" },
  { "dest1", "d1" },
  { "dstr", "ds1" },
  { "dstr1", "ds1" },
  { "dest2", "d2" },
  { "dstr2", "ds2" },
  { "dest3", "d3" },
  { "dstr3", "ds3" },
  { "src", "s1" },
  { "src1", "s1" },
  { "sstr", "ss1" },
  { "sstr1", "ss1" },
  { "src2", "s2" },
  { "sstr2", "ss2" },
  { "src3", "s3" },
  { "sstr3", "ss3" },
  { NULL, NULL }
};

static const
struct {
  OilArgType type;
  int direction;
  int is_stride;
  int index;
} arg_types[] = {
  { OIL_ARG_N, 'n', 0, 0 },
  { OIL_ARG_M, 'm', 0, 0 },
  { OIL_ARG_DEST1, 'd', 0, 1 },
  { OIL_ARG_DSTR1, 'd', 1, 1 },
  { OIL_ARG_DEST2, 'd', 0, 2 },
  { OIL_ARG_DSTR2, 'd', 1, 2 },
  { OIL_ARG_DEST3, 'd', 0, 3 },
  { OIL_ARG_DSTR3, 'd', 1, 3 },
  { OIL_ARG_SRC1, 's', 0, 1 },
  { OIL_ARG_SSTR1, 's', 1, 1 },
  { OIL_ARG_SRC2, 's', 0, 2 },
  { OIL_ARG_SSTR2, 's', 1, 2 },
  { OIL_ARG_SRC3, 's', 0, 3 },
  { OIL_ARG_SSTR3, 's', 1, 3 },
  { OIL_ARG_SRC4, 's', 0, 4 },
  { OIL_ARG_SSTR4, 's', 1, 4 },
  { OIL_ARG_SRC5, 's', 0, 5 },
  { OIL_ARG_SSTR5, 's', 1, 5 },
  { OIL_ARG_INPLACE1, 'i', 0, 1 },
  { OIL_ARG_ISTR1, 'i', 1, 1 },
  { OIL_ARG_INPLACE2, 'i', 0, 2 },
  { OIL_ARG_ISTR2, 'i', 1, 2 },
  { 0, 0, 0, 0 }
};

int
oil_param_from_string (OilParameter *p, char *s)
{
  int i;

  p->parameter_type = OIL_ARG_UNKNOWN;

  if (s[0] == 'n' && s[1] == 0) {
    p->direction = *s;
    p->is_stride = 0;
    p->is_pointer = 0;
    p->parameter_type = OIL_ARG_N;
    return 1;
  }
  if (s[0] == 'm' && s[1] == 0) {
    p->direction = *s;
    p->is_stride = 0;
    p->is_pointer = 0;
    p->parameter_type = OIL_ARG_M;
    return 1;
  }

  for(i=0;arg_aliases[i].from;i++){
    if (strcmp (s,arg_aliases[i].from) == 0) {
      s = arg_aliases[i].to;
      break;
    }
  }

  p->direction = *s;
  switch (*s) {
    case 'i':
      break;
    case 's':
      break;
    case 'd':
      break;
    default:
      return 0;
  }
  s++;

  if (*s == 's') {
    p->is_stride = 1;
    p->is_pointer = 0;
    s++;
  } else {
    p->is_stride = 0;
    p->is_pointer = 1;
  }

  if (isdigit ((int)*s)) {
    p->index = *s - '0';
    s++;
  } else {
    p->index = 1;
  }

  if (!p->is_stride && *s == '_') {
    int length;
    int var;

    s++;

    if (isdigit ((int)*s)) {
      length = strtoul (s, &s, 10);
      var = 0;
    } else if (*s == 'n' || *s == 'm') {
      var = (*s == 'n') ? 1 : 2;
      s++;
      if (*s == 'p') {
        s++;
        length = strtoul (s, &s, 10);
      } else {
        length = 0;
      }
    } else {
      return 0;
    }

    if (*s == 'x') {
      s++;
      p->prestride_length = length;
      p->prestride_var = var;

      if (isdigit ((int)*s)) {
        p->poststride_length = strtoul (s, &s, 10);
        p->poststride_var = 0;
      } else if (*s == 'n' || *s == 'm') {
        p->poststride_var = (*s == 'n') ? 1 : 2;
        s++;
        if (*s == 'p') {
          s++;
          p->poststride_length = strtoul (s, &s, 10);
        } else {
          p->poststride_length = 0;
        }
      } else {
        return 0;
      }

    } else {
      p->poststride_length = length;
      p->poststride_var = var;
      p->prestride_length = 1;
      p->prestride_var = 0;
    }

  } else {
    p->poststride_length = 0;
    p->poststride_var = 1;
    p->prestride_length = 1;
    p->prestride_var = 0;
  }
  if (*s != 0) {
    return 0;
  }

  p->parameter_type = OIL_ARG_UNKNOWN;
  for (i=0;arg_types[i].type != OIL_ARG_UNKNOWN; i++) {
    if (p->direction == arg_types[i].direction &&
        p->is_stride == arg_types[i].is_stride &&
        p->index == arg_types[i].index) {
      p->parameter_type = arg_types[i].type;
      break;
    }
  }

  if (p->parameter_type == OIL_ARG_UNKNOWN) {
    return 0;
  }

  return 1;
}

void *oil_param_get_source_data (OilParameter *param)
{
  uint8_t *ptr;
 
  ptr = param->src_data;
  ptr += param->test_header;

  return ptr;
}

