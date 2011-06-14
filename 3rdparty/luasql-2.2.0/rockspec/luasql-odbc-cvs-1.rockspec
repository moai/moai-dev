package = "LuaSQL-ODBC"
version = "cvs-1"
source = {
  url = "git://github.com/keplerproject/luasql.git"
}
description = {
   summary = "Database connectivity for Lua (ODBC driver)",
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
   ODBC = {
      header = "sql.h"
   }
}
build = {
   type = "make",
   variables = {
      T="odbc",
      LIB_OPTION = "$(LIBFLAG) -L$(ODBC_LIBDIR) -lodbc",
      CFLAGS = "$(CFLAGS) -I$(LUA_INCDIR) -I$(ODBC_INCDIR) -DUNIXODBC"
   },
   build_variables = {
      DRIVER_LIBS = "",
   },
   install_variables = {
      LUA_LIBDIR = "$(LIBDIR)",
   }
}
