// lua
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

// temporarilty replace calls here; TODO: roll into MOAILuaState
#if LUA_VERSION_NUM >= 502
	#define lua_objlen(L,idx) lua_rawlen(L,idx)
	#define LUA_HOOKTAILRET LUA_HOOKTAILCALL
#endif

#include <zl-util/headers.h>
#include <moai_config.h>
