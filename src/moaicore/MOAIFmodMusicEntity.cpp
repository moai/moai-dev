#include "pch.h"
#include <moaicore/MOAIFmodMusicEntity.h>

// register member functions
void MOAIFmodMusicEntity::RegisterLuaFuncs(MOAILuaState& state) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "getID", _getID },
		{ "getName", _getName },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

int MOAIFmodMusicEntity::_getID(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicEntity, "U")
		state.Push(self->mID);
	return 1;
}

int MOAIFmodMusicEntity::_getName(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicEntity, "U")
		state.Push(self->mName);
	return 1;
}