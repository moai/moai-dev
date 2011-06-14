package = "LuaSQL-SQLite3"
version = "cvs-1"
source = {
  url = "git://github.com/keplerproject/luasql.git"
}
description = {
   summary = "Database connectivity for Lua (SQLite3 driver)",
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
      header = "sqlite3.h"
   }
}
build = {
   type = "builtin",
   modules = {
     ["luasql.sqlite3"] = {
       sources = { "src/luasql.c", "src/ls_sqlite3.c" },
       libraries = { "sqlite3" },
       incdirs = { "$(SQLITE_INCDIR)" },
       libdirs = { "$(SQLITE_LIBDIR)" }
     }
   }
}
