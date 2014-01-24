/*
** $Id: luaconf.c,v 1.4 2008-08-09 02:46:48 leonb Exp $
** Support routines for the torch version of luaconf.h
*/


#include "luaconf.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>



/* ------------------------------------------------------ */
/* Utils to manipulate strings */

#define SBINCREMENT 256

typedef struct {
  char *buffer;
  int maxlen;
  int len;
} SB;

static void 
sbinit(SB *sb)
{
  sb->buffer = (char*)malloc(SBINCREMENT);
  sb->maxlen = SBINCREMENT;
  sb->len = 0;
}

static char *
sbfree(SB *sb)
{
  if (sb->buffer)
    free(sb->buffer);
  sb->buffer = 0;
  return 0;
}

static void
sbgrow(SB *sb, int n)
{
  if (sb->buffer && sb->len + n > sb->maxlen)
    {
      int nlen = sb->maxlen;
      while (sb->len + n > nlen)
        nlen += SBINCREMENT;
      sb->buffer = (char*)realloc(sb->buffer, nlen);
      sb->maxlen = nlen;
    }
}

static void
sbadd1(SB *sb, char c)
{
  sbgrow(sb, 1);
  if (sb->buffer)
    sb->buffer[sb->len++] = c;
}

static void
sbaddn(SB *sb, const char *s, int n)
{
  sbgrow(sb, n);
  if (sb->buffer && s && n)
    memcpy(sb->buffer + sb->len, s, n);
  else if (sb->buffer && n)
    sbfree(sb);
  sb->len += n;
}

static void
sbaddsf(SB *sb, char *s)
{
  if (s)
    sbaddn(sb, s, strlen(s));
  else
    sbfree(sb);
  if (s)
    free((void*)s);
}




/* ------------------------------------------------------ */
/* find executable directory */

#ifdef LUA_WIN

#include <windows.h>

static const char *
executable_dir(const char *progname)
{
  char *lb;
  char buff[MAX_PATH + 1];
  DWORD nsize = sizeof(buff)/sizeof(char);
  DWORD n = GetModuleFileNameA(NULL, buff, nsize);
  if (n == 0 || n == nsize || (lb = strrchr(buff, '\\')) == NULL)
    return 0;
  *lb = '\0';
  // convert to forward slashes
  for (lb = buff; *lb; lb++)
    if (*lb == '\\')
      *lb = '/';
  return _strdup(buff);
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

static char *
catpath(const char *from, const char *fname)
{
  SB sb;
  sbinit(&sb);
  if (fname && fname[0]=='/') 
    sbadd1(&sb, '/');
  else if (from)
    sbaddsf(&sb, catpath(NULL,from));
  else {
    char *cwd = 0;
    while (sb.buffer && !cwd) {
      cwd = getcwd(sb.buffer + sb.len, sb.maxlen - sb.len);
      if (cwd || errno != ERANGE) break;
      sbgrow(&sb, sb.maxlen - sb.len + SBINCREMENT);
    }
    if (cwd)
      sb.len += strlen(cwd);
    else
      sbfree(&sb);
  }
  for (;;) {
    while (fname && fname[0]=='/')
      fname++;
    if (!fname || !fname[0]) {
      while (&sb.buffer && sb.len>0 && sb.buffer[sb.len-1]=='/')
        sb.len -= 1;
      sbadd1(&sb, 0);
      return sb.buffer;
    }
    if (fname[0]=='.') {
      if (fname[1]=='/' || fname[1]==0) {
        fname +=1;
        continue;
      }
      if (fname[1]=='.')
        if (fname[2]=='/' || fname[2]==0) {
          fname +=2;
          while (sb.buffer && sb.len>0 && sb.buffer[sb.len-1]=='/')
            sb.len -= 1;
          while (sb.buffer && sb.len>0 && sb.buffer[sb.len-1]!='/')
            sb.len -= 1;
          continue;
        }
    }
    if (sb.len==0 || sb.buffer && sb.buffer[sb.len-1]!='/')
      sbadd1(&sb, '/');
    while (*fname!=0 && *fname!='/')
      sbadd1(&sb, *fname++);
  }
  sbadd1(&sb, 0);
  return sb.buffer;
}

static const char *
executable_dir(const char *progname)
{
  SB sb;
  struct stat sbuf;
  if (! progname)
    return 0;
  sbinit(&sb);
  if (progname[0]=='/')
    sbaddn(&sb, progname, strlen(progname));
  else if (strchr(progname,'/'))
    sbaddsf(&sb,catpath(NULL,progname));
  else {
    char *p = getenv("PATH");
    for (;;) {
      sb.len = 0;
      if (! (p && *p))
        return sbfree(&sb);
      while (*p && *p!=':')
        sbadd1(&sb, *p++);
      if (*p ==':')
        p++;
      sbadd1(&sb, 0);
      sb.len = 0;
      sbaddsf(&sb, catpath(sb.buffer, progname));
      sbadd1(&sb, 0);
      if (sb.buffer && stat(sb.buffer,&sbuf)>=0 && S_ISREG(sbuf.st_mode))
        if (access(sb.buffer, X_OK) >= 0)
          break;
    }
  }
#ifdef S_ISLNK
  sbadd1(&sb, 0);
  while(sb.buffer && lstat(sb.buffer,&sbuf)>=0 && S_ISLNK(sbuf.st_mode))
    {
      int h,l;
      sbaddn(&sb,"../",3);
      h = sb.len;
      while ((l=readlink(sb.buffer,sb.buffer+h,sb.maxlen-h))>=sb.maxlen-h)
        sbgrow(&sb, sb.maxlen - h + SBINCREMENT);
      if (l < 0)
        return sbfree(&sb);
      sb.len = h + l;
      sbadd1(&sb,0);
      sb.len = 0;
      sbaddsf(&sb, catpath(sb.buffer,sb.buffer+h-3));
      sbadd1(&sb,0);
    }
#endif
  while (sb.buffer && sb.len>0 && sb.buffer[sb.len-1]=='/')
    sb.len -= 1;
  while (sb.buffer && sb.len>0 && sb.buffer[sb.len-1]!='/')
    sb.len -= 1;
  while (sb.buffer && sb.len>0 && sb.buffer[sb.len-1]=='/')
    sb.len -= 1;
  sbadd1(&sb, 0);
  return sb.buffer;
}

#endif




/* ------------------------------------------------------ */
/* Relocate semi-colon separated paths */


static int
is_dot(char c)
{
  return c == '.';
}

static int
is_slash(char c)
{
#ifdef LUA_WIN
  return c == '/' || c == '\\';
#else
  return c == '/';
#endif
}


static const char *
relocate_path(const char *dir, const char *s)
{
  SB sb;
  const char *r;
  const char *execdir = LUA_EXECDIR;
  int lexecdir = strlen(execdir);
  int ldir = strlen(dir);
  sbinit(&sb);
  while ((r = strstr(s, execdir)))
    {
      sbaddn(&sb, s, r-s);
      sbaddn(&sb, dir, ldir);
      s = r + lexecdir;
      while (is_slash(s[0]))
        {
          if (is_dot(s[1]) && is_slash(s[2]))
            {
              s += 2;
              continue;
            }
          if (is_dot(s[1]) && is_dot(s[2]) && is_slash(s[3]) )
            {
              s += 3;
              while (sb.buffer && sb.len>0 && is_slash(sb.buffer[sb.len-1]))
                sb.len -= 1;
              while (sb.buffer && sb.len>0 && !is_slash(sb.buffer[sb.len-1]))
                sb.len -= 1;
              while (sb.buffer && sb.len>0 && is_slash(sb.buffer[sb.len-1]))
                sb.len -= 1;
              continue;
            }
          break;
        }
    }
  sbaddn(&sb, s, strlen(s));
  sbadd1(&sb, 0);
#ifdef LUA_WIN
  r = _strdup(sb.buffer);
#else
  r = strdup(sb.buffer);
#endif
  sbfree(&sb);
  return r;
}




/* ------------------------------------------------------ */
/* Public functions */


static const char *s_executable_dir = 0;
static const char *s_path_default = 0;
static const char *s_cpath_default = 0;


const char *
lua_executable_dir(const char *argv0)
{
  if (! s_executable_dir)
    {
      if (argv0 == 0)
        fprintf(stderr,
                "Warning: The main executable should call lua_executable_dir()\n"
                "         at least once with a non zero argument\n");
#if defined(__GLIBC__) && defined(__USE_GNU)
      if (argv0 == 0)
        argv0 = program_invocation_name;
#endif
      s_executable_dir = executable_dir(argv0);
    }
  return s_executable_dir;
}


const char *
lua_path_default()
{
  if (! s_path_default)
    {
      const char *dir = lua_executable_dir(0);
      s_path_default = LUA_PATH_DEFAULT_STATIC;
      if  (dir)
        s_path_default = relocate_path(dir, LUA_PATH_DEFAULT_STATIC);
    }
  return s_path_default;
}


const char *
lua_cpath_default()
{
  if (! s_cpath_default)
    {
      const char *dir = lua_executable_dir(0);
      s_cpath_default = LUA_CPATH_DEFAULT_STATIC;
      if (dir)
        s_cpath_default = relocate_path(dir, LUA_CPATH_DEFAULT_STATIC);
    }
  return s_cpath_default;
}





