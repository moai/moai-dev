#include <lua.h>
#include <lauxlib.h>

#include "luasocket.h"
#include "mime.h"
#include "luasocketscripts.h"
#include "fullluasocket.h"

LUAPRELOAD_API int luapreload_fullluasocket(lua_State *L) {
	luaL_findtable(L, LUA_GLOBALSINDEX, "package.preload", 3);
	
	lua_pushcfunction(L, luaopen_socket_core);
	lua_setfield(L, -2, "socket.core");
	lua_pushcfunction(L, luaopen_mime_core);
	lua_setfield(L, -2, "mime.core");
	lua_pushcfunction(L, luaopen_ltn12);
	lua_setfield(L, -2, "ltn12");
	lua_pushcfunction(L, luaopen_mime);
	lua_setfield(L, -2, "mime");
	lua_pushcfunction(L, luaopen_socket);
	lua_setfield(L, -2, "socket");
	lua_pushcfunction(L, luaopen_socket_ftp);
	lua_setfield(L, -2, "socket.ftp");
	lua_pushcfunction(L, luaopen_socket_http);
	lua_setfield(L, -2, "socket.http");
	lua_pushcfunction(L, luaopen_socket_smtp);
	lua_setfield(L, -2, "socket.smtp");
	lua_pushcfunction(L, luaopen_socket_tp);
	lua_setfield(L, -2, "socket.tp");
	lua_pushcfunction(L, luaopen_socket_url);
	lua_setfield(L, -2, "socket.url");
	
	lua_pop(L, 1);
	return 0;
}
