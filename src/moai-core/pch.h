// lua
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lstate.h>
	#include <lualib.h>
}

// temporarilty replace calls here; TODO: roll into MOAILuaState
#if LUA_VERSION_NUM >= 502

#endif

#include <zl-util/headers.h>
#include <moai_config.h>
