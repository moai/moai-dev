/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilfunction.h>
#include <liboil/liboildebug.h>
#include <liboil/liboilutils.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int
get_file_int (const char *file, int *value)
{
  char buffer[20];
  char *endptr;
  int fd;
  int n;

  fd = open (file, O_RDONLY);
  if (fd < 0) return 0;

  n = read(fd, buffer, 19);
  close(fd);
  if (n < 0) {
    return 0;
  }
  buffer[n] = 0;

  *value = strtol (buffer, &endptr, 0);

  if (endptr[0] == 0 || endptr[0] == '\n') return 1;
  return 0;
}

char *
get_file (const char *filename)
{
  char *cpuinfo;
  int fd;
  int n;

  cpuinfo = malloc(4096);
  if (cpuinfo == NULL) return NULL;

  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    free (cpuinfo);
    return NULL;
  }

  n = read(fd, cpuinfo, 4095);
  if (n < 0) {
    free (cpuinfo);
    close (fd);
    return NULL;
  }
  cpuinfo[n] = 0;

  close (fd);

  return cpuinfo;
}

char *
get_cpuinfo_line (char *cpuinfo, const char *tag)
{
  char *flags;
  char *end;
  char *colon;

  flags = strstr(cpuinfo,tag);
  if (flags == NULL) return NULL;

  end = strchr(flags, '\n');
  if (end == NULL) return NULL;
  colon = strchr (flags, ':');
  if (colon == NULL) return NULL;
  colon++;
  if(colon >= end) return NULL;

  return _strndup (colon, end-colon);
}

char *
_strndup (const char *s, int n)
{
  char *r;
  r = malloc (n+1);
  memcpy(r,s,n);
  r[n]=0;

  return r;
}

char **
strsplit (char *s)
{
  char **list = NULL;
  char *tok;
  int n = 0;

  while (*s == ' ') s++;

  list = malloc (1 * sizeof(char *));
  while (*s) {
    tok = s;
    while (*s && *s != ' ') s++;

    list[n] = _strndup (tok, s - tok);
    while (*s && *s == ' ') s++;
    list = realloc (list, (n + 2) * sizeof(char *));
    n++;
  }

  list[n] = NULL;
  return list;
}

char *
get_tag_value (char *s, const char *tag)
{
  char *flags;
  char *end;
  char *colon;

  flags = strstr(s,tag);
  if (flags == NULL) return NULL;

  end = strchr(flags, '\n');
  if (end == NULL) return NULL;
  colon = strchr (flags, ':');
  if (colon == NULL) return NULL;
  colon++;
  if(colon >= end) return NULL;

  return _strndup (colon, end-colon);
}


