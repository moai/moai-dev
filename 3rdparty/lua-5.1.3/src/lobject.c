/*
** $Id: lobject.c,v 2.22.1.1 2007/12/27 13:02:25 roberto Exp $
** Some generic functions over Lua objects
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lobject_c
#define LUA_CORE

#include "lua.h"

#include "ldo.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lvm.h"



const TValue luaO_nilobject_ = {{NULL}, LUA_TNIL};


/*
** converts an integer to a "floating point byte", represented as
** (eeeeexxx), where the real value is (1xxx) * 2^(eeeee - 1) if
** eeeee != 0 and (xxx) otherwise.
*/
int luaO_int2fb (unsigned int x) {
  int e = 0;  /* expoent */
  while (x >= 16) {
    x = (x+1) >> 1;
    e++;
  }
  if (x < 8) return x;
  else return ((e+1) << 3) | (cast_int(x) - 8);
}


/* converts back */
int luaO_fb2int (int x) {
  int e = (x >> 3) & 31;
  if (e == 0) return x;
  else return ((x & 7)+8) << (e - 1);
}


int luaO_log2 (unsigned int x) {
  static const lu_byte log_2[256] = {
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
  };
  int l = -1;
  while (x >= 256) { l += 8; x >>= 8; }
  return l + log_2[x];

}


int luaO_rawequalObj (const TValue *t1, const TValue *t2) {
  if (ttype(t1) != ttype(t2)) return 0;
  else switch (ttype(t1)) {
    case LUA_TNIL:
      return 1;
    case LUA_TNUMBER:
      return luai_numeq(nvalue(t1), nvalue(t2));
    case LUA_TBOOLEAN:
      return bvalue(t1) == bvalue(t2);  /* boolean true must be 1 !! */
    case LUA_TLIGHTUSERDATA:
      return pvalue(t1) == pvalue(t2);
    default:
      lua_assert(iscollectable(t1));
      return gcvalue(t1) == gcvalue(t2);
  }
}


int luaO_str2d (const char *s, lua_Number *result) {
  char *endptr;
  *result = lua_str2number(s, &endptr);
  if (endptr == s) return 0;  /* conversion failed */
  if (*endptr == 'x' || *endptr == 'X')  /* maybe an hexadecimal constant? */
    *result = cast_num(strtoul(s, &endptr, 16));
  if (*endptr == '\0') return 1;  /* most common case */
  while (isspace(cast(unsigned char, *endptr))) endptr++;
  if (*endptr != '\0') return 0;  /* invalid trailing characters? */
  return 1;
}



static void pushstr (lua_State *L, const char *str) {
  setsvalue2s(L, L->top, luaS_new(L, str));
  incr_top(L);
}


/* this function handles only `%d', `%c', %f, %p, and `%s' formats */
const char *luaO_pushvfstring (lua_State *L, const char *fmt, va_list argp) {
  int n = 1;
  pushstr(L, "");
  for (;;) {
    const char *e = strchr(fmt, '%');
    if (e == NULL) break;
    setsvalue2s(L, L->top, luaS_newlstr(L, fmt, e-fmt));
    incr_top(L);
    switch (*(e+1)) {
      case 's': {
        const char *s = va_arg(argp, char *);
        if (s == NULL) s = "(null)";
        pushstr(L, s);
        break;
      }
      case 'c': {
        char buff[2];
        buff[0] = cast(char, va_arg(argp, int));
        buff[1] = '\0';
        pushstr(L, buff);
        break;
      }
      case 'd': {
        setnvalue(L->top, cast_num(va_arg(argp, int)));
        incr_top(L);
        break;
      }
      case 'f': {
        setnvalue(L->top, cast_num(va_arg(argp, l_uacNumber)));
        incr_top(L);
        break;
      }
      case 'p': {
        char buff[4*sizeof(void *) + 8]; /* should be enough space for a `%p' */
        sprintf(buff, "%p", va_arg(argp, void *));
        pushstr(L, buff);
        break;
      }
      case '%': {
        pushstr(L, "%");
        break;
      }
      default: {
        char buff[3];
        buff[0] = '%';
        buff[1] = *(e+1);
        buff[2] = '\0';
        pushstr(L, buff);
        break;
      }
    }
    n += 2;
    fmt = e+2;
  }
  pushstr(L, fmt);
  luaV_concat(L, n+1, cast_int(L->top - L->base) - 1);
  L->top -= n;
  return svalue(L->top - 1);
}


const char *luaO_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *msg;
  va_list argp;
  va_start(argp, fmt);
  msg = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  return msg;
}


void luaO_chunkid (char *out, const char *source, size_t bufflen) {
  if (*source == '=') {
    strncpy(out, source+1, bufflen);  /* remove first char */
    out[bufflen-1] = '\0';  /* ensures null termination */
  }
  else {  /* out = "source", or "...source" */
    if (*source == '@') {
      size_t l;
      source++;  /* skip the `@' */
      bufflen -= sizeof(" '...' ");
      l = strlen(source);
      strcpy(out, "");
      if (l > bufflen) {
        source += (l-bufflen);  /* get last part of file name */
        strcat(out, "...");
      }
      strcat(out, source);
    }
    else {  /* out = [string "string"] */
      size_t len = strcspn(source, "\n\r");  /* stop at first newline */
      bufflen -= sizeof(" [string \"...\"] ");
      if (len > bufflen) len = bufflen;
      strcpy(out, "[string \"");
      if (source[len] != '\0') {  /* must truncate? */
        strncat(out, source, len);
        strcat(out, "...");
      }
      else
        strcat(out, source);
      strcat(out, "\"]");
    }
  }
}

#if !defined(LUA_PURE)

LUAI_FUNC void luaO_nsinit (lua_State *L, Closure *cl) {
  if (cl->c.isC)
    cl->c.nsinfo.first = cl->c.nsinfo.last = NULL;
  else
    cl->l.nsinfo.first = cl->l.nsinfo.last = NULL;
}

LUAI_FUNC void luaO_nsfree (lua_State *L, Closure *cl) {
  struct NamespaceInfo* info = cl->c.isC ? &cl->c.nsinfo : &cl->l.nsinfo;
  while (info->last != NULL)
    luaO_nspop(L, cl);
}

LUAI_FUNC void luaO_nscopy (lua_State *L, Closure *dest, Closure *src) {
  struct NamespaceInfo* info = src->c.isC ? &src->c.nsinfo : &src->l.nsinfo;
  struct NamespaceInfoNode* node = info->first;
  luaO_nsfree(L, dest);
  luaO_nsinit(L, dest);
  while (node != NULL) {
    luaO_nspush(L, dest, &node->name);
    node = node->next;
  }
}

LUAI_FUNC void luaO_nspush (lua_State *L, Closure *cl, const TValue *name) {
  struct NamespaceInfo* info;
  struct NamespaceInfoNode* node;
  lua_assert(ttisstring(name));
  info = cl->c.isC ? &cl->c.nsinfo : &cl->l.nsinfo;
  node = luaM_new(L, struct NamespaceInfoNode);
  node->next = NULL;
  node->prev = info->last;
  setobj(L, &node->name, name);
  if (info->last != NULL)
    info->last->next = node;
  info->last = node;
  if (info->first == NULL)
    info->first = node;
}

LUAI_FUNC void luaO_nspop (lua_State *L, Closure *cl) {
  struct NamespaceInfo* info = cl->c.isC ? &cl->c.nsinfo : &cl->l.nsinfo;
  struct NamespaceInfoNode* temp;
  if (info->last == NULL)
    return;
  if (info->first == info->last)
    info->first = NULL;
  temp = info->last;
  info->last = info->last->prev;
  luaM_free(L, temp);
}

LUAI_FUNC void luaO_clsinit (lua_State *L, Closure *cl) {
  if (cl->c.isC)
    cl->c.clsinfo.first = cl->c.clsinfo.last = NULL;
  else
    cl->l.clsinfo.first = cl->l.clsinfo.last = NULL;
}

LUAI_FUNC void luaO_clsfree (lua_State *L, Closure *cl) {
  struct ClassInfo* info = cl->c.isC ? &cl->c.clsinfo : &cl->l.clsinfo;
  while (info->last != NULL)
    luaO_clspop(L, cl);
}

LUAI_FUNC void luaO_clscopy (lua_State *L, Closure *dest, Closure *src) {
  struct ClassInfo* info = src->c.isC ? &src->c.clsinfo : &src->l.clsinfo;
  struct ClassInfoNode* node = info->first;
  luaO_clsfree(L, dest);
  luaO_clsinit(L, dest);
  while (node != NULL) {
    luaO_clspush(L, dest, &node->name);
    node = node->next;
  }
}

LUAI_FUNC void luaO_clspush (lua_State *L, Closure *cl, const TValue *name) {
  struct ClassInfo* info;
  struct ClassInfoNode* node;
  lua_assert(ttisstring(name));
  info = cl->c.isC ? &cl->c.clsinfo : &cl->l.clsinfo;
  node = luaM_new(L, struct ClassInfoNode);
  node->next = NULL;
  node->prev = info->last;
  setobj(L, &node->name, name);
  if (info->last != NULL)
    info->last->next = node;
  info->last = node;
  if (info->first == NULL)
    info->first = node;
}

LUAI_FUNC void luaO_clsinherits (lua_State *L, Closure *cl, const TValue *name) {
  /* TODO: Do we need to keep track in the structure for this? */
}

LUAI_FUNC void luaO_clsimplements (lua_State *L, Closure *cl, const TValue *name) {
  /* TODO: Do we need to keep track in the structure for this? */
}

LUAI_FUNC void luaO_clsfunc (lua_State *L, Closure *cl, Closure *ncl) {
  /* TODO: Do we need to keep track in the structure for this? */
}

void PrintString(const TString* ts);

static int __luaO_clsconstructornew (lua_State *L) {
  int meta, instance, metainst, type, args, arglen, metatype, i;
  int hasconstructor = 0;
  const char* typename;
  
  /* pull out the metatable */
  lua_assert(lua_getmetatable(L, 1) != 0); /* table ref is at idx 1 */
  meta = lua_gettop(L);
  lua_assert(lua_istable(L, meta));

  /* get the type and argument tables */
  lua_getfield(L, meta, "type");
  type = lua_gettop(L);
  lua_getfield(L, meta, "args");
  args = lua_gettop(L);
  lua_assert(lua_istable(L, type) && lua_istable(L, args));

  /* pull out the type metatable */
  lua_getmetatable(L, type);
  metatype = lua_gettop(L);
  lua_assert(lua_istable(L, metatype));

  /* get the type name string */
  lua_getfield(L, metatype, "__type");
  typename = lua_tostring(L, -1);

  /* construct a new instance table */
  lua_newtable(L);
  instance = lua_gettop(L);

  /* construct and set up the metatable for the instance */
  lua_newtable(L);
  metainst = lua_gettop(L);
  lua_getfield(L, meta, "__type");
  lua_setfield(L, metainst, "__type");
  lua_setmetatable(L, instance);

  /* copy all of the functions defined in the type table */
  lua_pushnil(L);
  while (lua_next(L, type)) {
    /* skip over constructor */
    if (lua_isstring(L, -2) && strcmp(lua_tostring(L, -2), typename) == 0) {
      hasconstructor = 1;
      lua_pop(L, 1); /* pop value */
      continue;
    }
    /* key at -2, value at -1 (-2 after first push) */
    lua_pushvalue(L, -2);
    lua_pushvalue(L, -2);
    lua_rawset(L, instance);
    lua_pop(L, 1); /* pop value */
  }

  /* if the class defines a constructor, call it */
  if (hasconstructor) {
    hasconstructor = 0;
    /* push the constructor onto the stack */
    lua_pushnil(L);
    while (lua_next(L, type)) {
      /* skip over constructor */
      if (lua_isstring(L, -2) && strcmp(lua_tostring(L, -2), typename) == 0) {
        hasconstructor = 1;
        lua_remove(L, -2); /* remove key */
        break;
      }
      lua_pop(L, 1); /* pop value */
    }
    lua_assert(hasconstructor != 0);

    /* loop over arguments and push them onto the stack */
    arglen = lua_objlen(L, args);
    lua_pushvalue(L, instance);
    for (i = 1; i <= arglen; i++)
      lua_rawgeti(L, args, i);
    lua_call(L, arglen + 1, 0);
  }

  /* pop all values between the top of the stack and the instance
   * then remove all of the values below the instance */
  lua_pop(L, lua_gettop(L) - instance);
  while (lua_gettop(L) > 1)
    lua_remove(L, 1);
  lua_assert(lua_istable(L, 1));

  /* return new instance */
  return 1;
}

static int __luaO_clsconstructor (lua_State *L) {
  int empty, meta, argc, args, i;

  /* count total number of arguments */
  argc = lua_gettop(L) - 1; /* table ref is at idx 1 */

  /* create an empty table */
  lua_newtable(L);
  empty = lua_gettop(L);

  /* create the metatable to hold data */
  lua_newtable(L);
  meta = lua_gettop(L);

  /* push the new C function for __new */
  lua_pushcfunction(L, __luaO_clsconstructornew);
  lua_setfield(L, meta, "__new");

  /* push the reference to the type */
  lua_pushvalue(L, 1);
  lua_setfield(L, meta, "type");

  /* push the C function for __type */
  lua_getmetatable(L, 1);
  lua_getfield(L, -1, "__type");
  lua_setfield(L, meta, "__type");
  lua_pop(L, 1);

  /* push the table of arguments into the metatable */
  lua_newtable(L);
  args = lua_gettop(L);
  for (i = 0; i < argc; i++) {
    lua_pushvalue(L, i + 2);
    lua_rawseti(L, args, i + 1);
  }
  lua_setfield(L, meta, "args");

  /* protect the metatable */
  //lua_pushstring(L, "generated constructor metatable is protected");
  //lua_setfield(L, meta, "__metatable");

  /* associate metatable */
  lua_setmetatable(L, empty);

  /* we return the table */
  return 1;
}

LUAI_FUNC void luaO_clspop (lua_State *L, Closure *cl) {
  struct ClassInfo* info = cl->c.isC ? &cl->c.clsinfo : &cl->l.clsinfo;
  struct ClassInfoNode* temp;
  struct NamespaceInfo* nsinfo = cl->c.isC ? &cl->c.nsinfo : &cl->l.nsinfo;
  struct NamespaceInfoNode* nstemp;
  TValue global, current;
  TValue metatable, key;
  TValue entry;
  Closure *fcl;
  /* local global env */
  sethvalue(L, &global, cl->c.isC ? cl->c.env : cl->l.env);
  /* load namespace */
  if (nsinfo->first != NULL) {
    lua_assert(ttisstring(&nsinfo->first->name));
    luaV_gettable(L, &global, &nsinfo->first->name, &current);
    nstemp = nsinfo->first->next;
    while (nstemp != NULL) {
      lua_assert(ttisstring(&nstemp->name));
      luaV_gettable(L, &current, &nstemp->name, &current);
      nstemp = nstemp->next;
    }
  }
  /* load class */
  lua_assert(ttisstring(&info->first->name));
  if (nsinfo->first == NULL)
    luaV_gettable(L, &global, &info->first->name, &current);
  else
    luaV_gettable(L, &current, &info->first->name, &current);
  temp = info->first->next;
  while (temp != NULL) {
    lua_assert(ttisstring(&temp->name));
    luaV_gettable(L, &current, &temp->name, &current);
    temp = temp->next;
  }
  /* now the class table is loaded into 'current' and we can manipulate it */
  assert(ttistable(&current));
  /* create metatable */
  sethvalue(L, &metatable, luaH_new(L, 0, 0));
  /* create constructor */
  setsvalue(L, &key, luaS_new(L, "__call"));
  fcl = luaF_newCclosure(L, 0, &global);
  fcl->c.isC = 1;
  fcl->c.f = &__luaO_clsconstructor;
  setclvalue(L, &entry, fcl);
  luaV_settable(L, &metatable, &key, &entry);
  /* assign type name */
  setsvalue(L, &key, luaS_new(L, "__type"));
  luaV_settable(L, &metatable, &key, &info->last->name);
  /* TODO: assign table with full namespace structure */
  /* protect metatable */
  //setsvalue(L, &key, luaS_new(L, "__metatable"));
  //setsvalue(L, &entry, luaS_new(L, "generated type metatable is protected"));
  //luaV_settable(L, &metatable, &key, &entry);
  /* assign metatable */
  hvalue(&current)->metatable = hvalue(&metatable);
  luaC_objbarriert(L, hvalue(&current), hvalue(&metatable));
  /* pop the class from the stack */
  if (info->last == NULL)
    return;
  if (info->first == info->last)
    info->first = NULL;
  temp = info->last;
  info->last = info->last->prev;
  luaM_free(L, temp);
}

#endif