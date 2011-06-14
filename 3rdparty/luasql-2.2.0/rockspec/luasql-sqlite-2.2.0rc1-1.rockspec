package = "LuaSQL-SQLite"
version = "2.2.0rc1-1"
source = {
   url = "http://luasql.luaforge.net/luasql-2.2.0rc1.tar.gz",
}
description = {
   summary = "Database connectivity for Lua (SQLite driver)",
   detailed = [[
      LuaSQL is a simple interface from Lua to a DBMS. It enables a
      Lua program to connect to databases, execute arbitrary SQL statements
      and retrieve results in a row-by-row cursor fashion.
   ]],
   license = "MIT/X11",
   homepage = "http://www.keplerproject.org/luasql/"
}
dependencies = {
   "lua >= 5.1"
}
external_dependencies = {
   SQLITE = {
      header = "sqlite.h"
   }
}
build = {
   type = "make",
   variables = {
      T="sqlite",
      LIB_OPTION = "$(LIBFLAG) -L$(SQLITE_LIBDIR) -lsqlite",
      CFLAGS = "$(CFLAGS) -I$(LUA_INCDIR) -I$(SQLITE_INCDIR)"
   },
   build_variables = {
      DRIVER_LIBS="",
   },
   install_variables = {
      LUA_LIBDIR = "$(LIBDIR)",
   }
}
