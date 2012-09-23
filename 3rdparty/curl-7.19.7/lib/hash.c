/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2009, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: hash.c,v 1.41 2009-06-10 02:49:43 yangtse Exp $
 ***************************************************************************/

#include "setup.h"

#include <string.h>
#include <stdlib.h>

#include "hash.h"
#include "llist.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>

#include "curl_memory.h"
/* The last #include file should be: */
#include "memdebug.h"

static void
hash_element_dtor(void *user, void *element)
{
  struct curl_hash *h = (struct curl_hash *) user;
  struct curl_hash_element *e = (struct curl_hash_element *) element;

  if(e->key)
    free(e->key);

  if(e->ptr)
    h->dtor(e->ptr);

  free(e);
}

/* return 1 on error, 0 is fine */
int
Curl_hash_init(struct curl_hash *h,
               int slots,
               hash_function hfunc,
               comp_function comparator,
               curl_hash_dtor dtor)
{
  int i;

  if(!slots || !hfunc || !comparator ||!dtor) {
    return 1; /* failure */
  }

  h->hash_func = hfunc;
  h->comp_func = comparator;
  h->dtor = dtor;
  h->size = 0;
  h->slots = slots;

  h->table = malloc(slots * sizeof(struct curl_llist *));
  if(h->table) {
    for (i = 0; i < slots; ++i) {
      h->table[i] = Curl_llist_alloc((curl_llist_dtor) hash_element_dtor);
      if(!h->table[i]) {
        while(i--)
          Curl_llist_destroy(h->table[i], NULL);
        free(h->table);
        return 1; /* failure */
      }
    }
    return 0; /* fine */
  }
  else
    return 1; /* failure */
}

struct curl_hash *
Curl_hash_alloc(int slots,
                hash_function hfunc,
                comp_function comparator,
                curl_hash_dtor dtor)
{
  struct curl_hash *h;

  if(!slots || !hfunc || !comparator ||!dtor) {
    return NULL; /* failure */
  }

  h = malloc(sizeof(struct curl_hash));
  if(h) {
    if(Curl_hash_init(h, slots, hfunc, comparator, dtor)) {
      /* failure */
      free(h);
      h = NULL;
    }
  }

  return h;
}



static struct curl_hash_element *
mk_hash_element(const void *key, size_t key_len, const void *p)
{
  struct curl_hash_element *he = malloc(sizeof(struct curl_hash_element));

  if(he) {
    void *dupkey = malloc(key_len);
    if(dupkey) {
      /* copy the key */
      memcpy(dupkey, key, key_len);

      he->key = dupkey;
      he->key_len = key_len;
      he->ptr = (void *) p;
    }
    else {
      /* failed to duplicate the key, free memory and fail */
      free(he);
      he = NULL;
    }
  }
  return he;
}

#define FETCH_LIST(x,y,z) x->table[x->hash_func(y, z, x->slots)]

/* Return the data in the hash. If there already was a match in the hash,
   that data is returned. */
void *
Curl_hash_add(struct curl_hash *h, void *key, size_t key_len, void *p)
{
  struct curl_hash_element  *he;
  struct curl_llist_element *le;
  struct curl_llist *l = FETCH_LIST (h, key, key_len);

  for (le = l->head; le; le = le->next) {
    he = (struct curl_hash_element *) le->ptr;
    if(h->comp_func(he->key, he->key_len, key, key_len)) {
      h->dtor(p);     /* remove the NEW entry */
      return he->ptr; /* return the EXISTING entry */
    }
  }

  he = mk_hash_element(key, key_len, p);
  if(he) {
    if(Curl_llist_insert_next(l, l->tail, he)) {
      ++h->size;
      return p; /* return the new entry */
    }
    /*
     * Couldn't insert it, destroy the 'he' element and the key again. We
     * don't call hash_element_dtor() since that would also call the
     * "destructor" for the actual data 'p'. When we fail, we shall not touch
     * that data.
     */
    free(he->key);
    free(he);
  }

  return NULL; /* failure */
}

/* remove the identified hash entry, returns non-zero on failure */
int Curl_hash_delete(struct curl_hash *h, void *key, size_t key_len)
{
  struct curl_llist_element *le;
  struct curl_hash_element  *he;
  struct curl_llist *l = FETCH_LIST(h, key, key_len);

  for (le = l->head; le; le = le->next) {
    he = le->ptr;
    if(h->comp_func(he->key, he->key_len, key, key_len)) {
      Curl_llist_remove(l, le, (void *) h);
      return 0;
    }
  }
  return 1;
}

void *
Curl_hash_pick(struct curl_hash *h, void *key, size_t key_len)
{
  struct curl_llist_element *le;
  struct curl_hash_element  *he;
  struct curl_llist *l = FETCH_LIST(h, key, key_len);

  for (le = l->head; le; le = le->next) {
    he = le->ptr;
    if(h->comp_func(he->key, he->key_len, key, key_len)) {
      return he->ptr;
    }
  }

  return NULL;
}

#if defined(DEBUGBUILD) && defined(AGGRESIVE_TEST)
void
Curl_hash_apply(curl_hash *h, void *user,
                void (*cb)(void *user, void *ptr))
{
  struct curl_llist_element  *le;
  int                  i;

  for (i = 0; i < h->slots; ++i) {
    for (le = (h->table[i])->head;
         le;
         le = le->next) {
      curl_hash_element *el = le->ptr;
      cb(user, el->ptr);
    }
  }
}
#endif

void
Curl_hash_clean(struct curl_hash *h)
{
  int i;

  for (i = 0; i < h->slots; ++i) {
    Curl_llist_destroy(h->table[i], (void *) h);
    h->table[i] = NULL;
  }

  free(h->table);
}

void
Curl_hash_clean_with_criterium(struct curl_hash *h, void *user,
                               int (*comp)(void *, void *))
{
  struct curl_llist_element *le;
  struct curl_llist_element *lnext;
  struct curl_llist *list;
  int i;

  for (i = 0; i < h->slots; ++i) {
    list = h->table[i];
    le = list->head; /* get first list entry */
    while(le) {
      struct curl_hash_element *he = le->ptr;
      lnext = le->next;
      /* ask the callback function if we shall remove this entry or not */
      if(comp(user, he->ptr)) {
        Curl_llist_remove(list, le, (void *) h);
        --h->size; /* one less entry in the hash now */
      }
      le = lnext;
    }
  }
}

void
Curl_hash_destroy(struct curl_hash *h)
{
  if(!h)
    return;

  Curl_hash_clean(h);

  free(h);
}

size_t Curl_hash_str(void* key, size_t key_length, size_t slots_num)
{
  const char* key_str = (const char *) key;
  const char *end = key_str + key_length;
  unsigned long h = 5381;

  while(key_str < end) {
    h += h << 5;
    h ^= (unsigned long) *key_str++;
  }

  return (h % slots_num);
}

size_t Curl_str_key_compare(void*k1, size_t key1_len, void*k2, size_t key2_len)
{
  char *key1 = (char *)k1;
  char *key2 = (char *)k2;

  if(key1_len == key2_len &&
      *key1 == *key2 &&
      memcmp(key1, key2, key1_len) == 0) {
    return 1;
  }

  return 0;
}

#if 0 /* useful function for debugging hashes and their contents */
void Curl_hash_print(struct curl_hash *h,
                     void (*func)(void *))
{
  int i;
  struct curl_llist_element *le;
  struct curl_llist *list;
  struct curl_hash_element  *he;
  if(!h)
    return;

  fprintf(stderr, "=Hash dump=\n");

  for (i = 0; i < h->slots; i++) {
    list = h->table[i];
    le = list->head; /* get first list entry */
    if(le) {
      fprintf(stderr, "index %d:", i);
      while(le) {
        he = le->ptr;
        if(func)
          func(he->ptr);
        else
          fprintf(stderr, " [%p]", he->ptr);
        le = le->next;
      }
      fprintf(stderr, "\n");
    }
  }
}
#endif
