AC_INIT(lua-sqlite3, 0.4, mroth@nessie.de)
AC_COPYRIGHT([Copyright (c) 2006 Michael Roth <mroth@nessie.de>])


AC_PROG_CC()
AC_CHECK_HEADERS()
AC_PROG_EGREP()


AC_MSG_CHECKING([whether linking with rpath is requested])
AC_ARG_ENABLE(rpath,
  AC_HELP_STRING([--enable-rpath], [link with rpath option]),
  rpath=$enableval,
  rpath=no
)
AC_MSG_RESULT($rpath)


lua_dir=""
lua_includedir=""
lua_libdir=""
LUA=""

AC_ARG_WITH(lua-dir,
  [AC_HELP_STRING(--with-lua-dir=DIR, [where you installed lua [EPREFIX]]) ],
  [
    lua_dir="${withval}"
    lua_includedir="${lua_dir}/include"
    lua_libdir="${lua_dir}/lib"
    LUA="${lua_dir}/bin/lua"
  ]
)

AC_ARG_WITH(lua-includedir,
  [AC_HELP_STRING(--with-lua-includedir=DIR, [where to find lua headers [LUA_DIR/include]])],
  lua_includedir="${withval}"
)

AC_ARG_WITH(lua-libdir,
  [AC_HELP_STRING(--with-lua-libdir=DIR, [where to find the lua library [LUA_DIR/lib]])],
  lua_libdir="${withval}"
)

AC_ARG_WITH(lua,
  [AC_HELP_STRING(--with-lua=FILE, [path to the 'lua' command [LUA_DIR/bin/lua]])],
  LUA="${withval}"
)

AC_PATH_PROG(LUA, lua, [], [$bindir:$exec_prefix/bin:$prefix/bin:$PATH])

if test ! -x "${LUA}" ; then
  AC_MSG_WARN([No lua interpreter found!])
fi


sqlite3_dir=""
sqlite3_includedir=""
sqlite3_libdir=""

AC_ARG_WITH(sqlite3-dir,
  [AC_HELP_STRING(--with-sqlite3-dir=DIR, [where you installed sqlite3 [EPREFIX]])],
  [
    sqlite3_dir="${withval}"
    sqlite3_includedir="${sqlite3_dir}/include"
    sqlite3_libdir="${sqlite3_dir}/lib"
  ]
)

AC_ARG_WITH(sqlite3-includedir,
  [AC_HELP_STRING(--with-sqlite3-includedir=DIR, [where to find sqlite3 headers [SQLITE3_DIR/include]])],
  sqlite3_includedir="${withval}"
)

AC_ARG_WITH(sqlite3-libdir,
  [AC_HELP_STRING(--with-sqlite3-libdir=DIR, [where to find the sqlite3 library [SQLITE3_DIR/lib]])],
  sqlite3_libdir="${withval}"
)


if test -n "$lua_includedir" ; then
  CFLAGS="-I$lua_includedir $CFLAGS"
  CPPFLAGS="-I$lua_includedir $CPPFLAGS"
fi

if test -n "$lua_libdir" ; then
  if test $rpath = yes ; then
    LDFLAGS="-Wl,-rpath,${lua_libdir}"
  fi
  LDFLAGS="-L$lua_libdir $LDFLAGS"
fi

if test -n "$sqlite3_includedir" -a "$sqlite3_includedir" != "$lua_includedir"; then
  CFLAGS="-I$sqlite3_includedir $CFLAGS"
  CPPFLAGS="-I$sqlite3_includedir $CPPFLAGS"
fi

if test -n "$sqlite3_libdir" -a "$sqlite3_libdir" != "$lua_libdir"; then
  if test $rpath = yes ; then
    LDFLAGS="-Wl,-rpath,${sqlite3_libdir}"
  fi
  LDFLAGS="-L$sqlite3_libdir $LDFLAGS"
fi


AC_CHECK_HEADER(lua.h, [], [AC_MSG_ERROR(Cannot find lua.h)])
AC_CHECK_HEADER(lauxlib.h, [], [AC_MSG_ERROR(Cannot find lauxlib.h)])
AC_CHECK_HEADER(sqlite3.h, [], [AC_MSG_ERROR(Cannot find sqlite3.h)])


AC_MSG_CHECKING(if we need -llua)
need_llua=yes
nm -D "$LUA" | $EGREP "T lua_open$" >/dev/null && need_llua=no
AC_MSG_RESULT($need_llua)
if test $need_llua = yes; then
  LIBS="-llua -lm $LIBS"
fi


AC_MSG_CHECKING(if we need -llualib)
if test $need_llua = yes; then
  AC_LINK_IFELSE([
    #include "lauxlib.h"
    int main()
    {
      luaL_Buffer buf;
      luaL_buffinit(0, &buf);
      return 0;
    }
  ], [need_llualib=no], [need_llualib=yes])
else
  need_llualib=no
fi
AC_MSG_RESULT($need_llualib)
if test $need_llualib = yes; then
  LIBS="-llualib $LIBS"
fi

LIBS="-lsqlite3 $LIBS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
