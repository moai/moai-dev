LUA_DIR=c:\lua5.1\lua

install:
	IF NOT EXIST $(LUA_DIR)\luasql mkdir $(LUA_DIR)\luasql
	copy src\ado\ado.lua $(LUA_DIR)\luasql

clean:

