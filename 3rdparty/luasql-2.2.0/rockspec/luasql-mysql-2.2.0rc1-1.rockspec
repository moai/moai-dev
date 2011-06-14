package = "LuaSQL-MySQL"
version = "2.2.0rc1-1"
source = {
   url = "http://luasql.luaforge.net/luasql-2.2.0rc1.tar.gz",
}
description = {
   summary = "Database connectivity for Lua (MySQL driver)",
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
   MYSQL = {
      header = "mysql/mysql.h"
   }
}
build = {
   type = "make",
   variables = {
      T="mysql",
      LIB_OPTION = "$(LIBFLAG) -L$(MYSQL_LIBDIR) -lmysqlclient -lz -lcrypt -lnsl -lm",
      CFLAGS = "$(CFLAGS) -I$(LUA_INCDIR) -I$(MYSQL_INCDIR)/mysql"
   },
   build_variables = {
      DRIVER_LIBS="",
   },
   install_variables = {
      LUA_LIBDIR = "$(LIBDIR)",
   }
}
