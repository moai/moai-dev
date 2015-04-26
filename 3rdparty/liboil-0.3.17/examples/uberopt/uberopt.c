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

#include <glib.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Insn Insn;
struct _Insn {
  char *line;
  int index;
  int n_deps;
  int deps[10];
  int x;
};

Insn insns[100];

int state[100];

int max_depth = 1000;
int outputting = 1;

int n_lines;
int start_line = -1;
int end_line = -1;
int uber_start_line = -1;
int uber_end_line = -1;
int n_insns = 0;
int depth;
int n_impls = 0;
char **lines;

void output_sequence (void);
void read_file(void);

int
main (int argc, char *argv[])
{
  int i;
  int j;
  int n;

  read_file();

  if (outputting) {
    for(i=0;i<start_line;i++){
      g_print("%s\n",lines[i]);
    }
  }

  for(n=0;n<100;n++){
    g_print("/*\n");
    for(i=0;i<n_insns;i++){
      for(j=0;j<n_insns;j++){
        insns[j].x = 0; // 0 means available
      }
      for(j=0;j<i;j++){
        insns[state[j]].x = 1; // 1 means already used
      }
      for(j=0;j<n_insns;j++){
        int k;

        if (insns[j].x != 0) continue;

        for(k=0;k<insns[j].n_deps;k++){
          int l;
          l = insns[j].deps[k];
          if (insns[l].x != 1) {
            insns[j].x = 2; // 2 means deps not satisfied
          }
        }
      }
      g_print("list:\n");
      for(j=0;j<n_insns;j++){
        g_print("%d ", insns[j].x);
      }
      g_print("\n");
      while(1) {
        j = g_random_int_range (0, n_insns);
        if (insns[j].x == 0) break;
      }
      g_print("chose %d\n",j);

      state[i] = j;
    }
    g_print("*/\n");
    output_sequence ();
    n_impls++;
  }

#if 0
  depth = 0;
  while(depth >= 0) {
    int insn;

    insn = state[depth];

    /* check if we've exhausted instructions */
    if (insn >= max_depth) {
      depth--;
      if (depth >= 0) state[depth]++;
      continue;
    }
    /* check if this instruction has been used */
    for(i=0;i<depth;i++) {
      if (state[i] == insn) {
        state[depth]++;
        goto loop;
      }
    }
    /* check if this is a valid instruction to place */
    for(i=0;i<insns[insn].n_deps;i++){
      for(j=0;j<depth;j++){
        if (state[j] == insns[insn].deps[i]){
          break;
        }
      }
      if (j == depth) {
        state[depth]++;
        goto loop;
      }
    }

#if 0
    /* only move instruction a few places */
    if (depth < insn - 1 || depth > insn + 1) {
      state[depth]++;
    }
#endif

    if (depth < max_depth - 1) {
      depth++;
      state[depth]=0;
    } else {
      output_sequence ();

      n_impls++;
      state[depth]++;
    }
    loop:
    ;
  }
#endif

  if (outputting) {
    for(i=end_line+1;i<n_lines;i++){
      g_print("%s\n",lines[i]);
    }
  }

  if (!outputting) {
    g_print("n_impls = %d\n", n_impls);
  }

  return 0;
}

void read_file(void)
{
  gboolean ret;
  char *contents;
  gsize length;
  int i;

  ret = g_file_get_contents ("it.c", &contents, &length, NULL);

  lines = g_strsplit (contents, "\n", 0);
  
  for(i=0;lines[i];i++){
    char *uber;

    if (strcmp(lines[i],"UBER_START")==0) {
      start_line = i;
    }
    if (strcmp(lines[i],"UBER_END")==0) {
      end_line = i;
    }
    uber = strstr(lines[i],"UBER ");
    if (uber) {
      int n_deps = 0;
      char *end;

      if (uber_start_line < 0) uber_start_line = i;
      uber_end_line = i;

      uber+=5;

      insns[n_insns].line = lines[i];
      insns[n_insns].index = strtol(uber, &end, 10);
      uber = end;
      uber++;
      while(1) {
        while(*uber && *uber == ' ')uber++;
        insns[n_insns].deps[n_deps] = strtol(uber, &end, 10);
        if (end == uber) break;
        uber = end;
        n_deps++;
      }
      insns[n_insns].n_deps = n_deps;

      n_insns++;
    }
    n_lines++;
  }
}

void output_sequence (void)
{
  int i;

  if (outputting) {
    for(i=start_line + 1;i<uber_start_line;i++){
      char *s;

      s = strstr(lines[i],"UBER_INDEX");
      if (s) {
        g_print("%.*s%d%s\n", (int)(s-lines[i]), lines[i], n_impls, s+10);
      } else {
        g_print("%s\n",lines[i]);
      }
    }

    for(i=0;i<max_depth && i<n_insns;i++){
      g_print("%s\n",insns[state[i]].line);
    }
    for(i=max_depth;i<n_insns;i++) {
      g_print("%s\n",insns[i].line);
    }

    for(i=uber_end_line + 1;i<end_line;i++){
      char *s;

      s = strstr(lines[i],"UBER_INDEX");
      if (s) {
        g_print("%.*s%d%s\n", (int)(s-lines[i]), lines[i], n_impls, s+10);
      } else {
        g_print("%s\n",lines[i]);
      }
    }
    g_print("\n");
  }
}

