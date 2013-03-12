#include "pch.h"

#include <moaicore/MOAIFmodReverbChannelproperties.h>

// register member functions
void MOAIFmodReverbChannelproperties::RegisterLuaFuncs(MOAILuaState& state) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "getDirect", _getDirect },
		{ "getRoom", _getRoom },
		{ "getFlags", _getFlags },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

int MOAIFmodReverbChannelproperties::_getDirect(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodReverbChannelproperties, "U")
		state.Push(self->mDirect);
	return 1;
}

int MOAIFmodReverbChannelproperties::_getRoom(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodReverbChannelproperties, "U")
		state.Push(self->mRoom);
	return 1;
}

int MOAIFmodReverbChannelproperties::_getFlags(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodReverbChannelproperties, "U")
		state.Push(self->mFlags);
	return 1;
}