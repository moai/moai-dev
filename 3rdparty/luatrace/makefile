LUA= $(shell echo `which lua`)
LUA_BINDIR= $(shell echo `dirname $(LUA)`)
LUA_PREFIX= $(shell echo `dirname $(LUA_BINDIR)`)
LUA_SHAREDIR=$(LUA_PREFIX)/share/lua/5.1
LUA_LIBDIR=$(LUA_PREFIX)/lib/lua/5.1
LUA_H:= $(shell echo `find $(LUA_PREFIX)/include -name "lua.h"`)
LUA_H:= $(shell echo `echo $(LUA_H) | cut -f 1 -d \ `)
LUA_INCDIR= $(shell echo `dirname $(LUA_H)`)

CC=/usr/bin/cc
CFLAGS=-O3 -Wall -Wextra -pedantic

# Guess a platform
UNAME=$(shell uname -s)
ifneq (,$(findstring Darwin,$(UNAME)))
  # OS X
  CFLAGS:=$(CFLAGS) -fPIC -arch i686 -arch x86_64 -std=c89
  SHARED=-bundle -undefined dynamic_lookup
  SO_SUFFIX=so
else
ifneq (,$(findstring MINGW,$(UNAME)))
  #MinGW
  CC=gcc
  SHARED=-shared
  LIBS=-L$(LUA_BINDIR) -llua51
  SO_SUFFIX=dll
else
ifneq (,$(findstring Linux,$(UNAME)))
  # Linux
  CFLAGS:=$(CFLAGS) -fPIC
  SHARED=-shared
  LIBS=-lrt
  SO_SUFFIX=so
else
  # BSD
  CFLAGS:=$(CFLAGS) -fPIC
  SHARED=-shared
  LIBS=-L$(LUA_LIBDIR) -llua
  SO_SUFFIX=so
endif
endif
endif

lua/luatrace/c_hook.$(SO_SUFFIX): c/c_hook.c
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@ $(LIBS) -I$(LUA_INCDIR)

install: lua/luatrace/c_hook.$(SO_SUFFIX)
	mkdir -p $(LUA_SHAREDIR)/luatrace
	mkdir -p $(LUA_SHAREDIR)/uatrace
	mkdir -p $(LUA_SHAREDIR)/jit
	mkdir -p $(LUA_LIBDIR)/luatrace
	cp lua/luatrace.lua $(LUA_SHAREDIR)
	cp lua/uatrace.lua $(LUA_SHAREDIR)
	cp lua/luatrace/*.lua $(LUA_SHAREDIR)/luatrace
	cp lua/uatrace/*.lua $(LUA_SHAREDIR)/uatrace
	-cp lua/luatrace/c_hook.so $(LUA_LIBDIR)/luatrace
	cp sh/luatrace.profile $(LUA_BINDIR)
	chmod +x $(LUA_BINDIR)/luatrace.profile
	cp lua/jit/annotate.lua $(LUA_SHAREDIR)/jit

uninstall: 
	rm -f $(LUA_SHAREDIR)/luatrace.lua
	rm -f $(LUA_SHAREDIR)/uatrace.lua
	rm -rf $(LUA_SHAREDIR)/luatrace
	rm -rf $(LUA_SHAREDIR)/uatrace
	-rm -rf $(LUA_LIBDIR)/luatrace
	rm -f $(LUA_BINDIR)/luaprofile
	rm -f $(LUA_BINDIR)/luatrace.profile
	rm -f $(LUA_SHAREDIR)/jit/annotate.lua

clean:
	rm -f lua/luatrace/c_hook.so
	find . -name "trace-out.txt" -delete
	find . -name "annotated-source.txt" -delete

