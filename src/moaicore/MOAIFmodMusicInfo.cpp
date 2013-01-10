#include "pch.h"
#include <moaicore/MOAIFmodMusicInfo.h>

// register member functions
void MOAIFmodMusicInfo::RegisterLuaFuncs(MOAILuaState& state) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "getID", _starving },
		{ "getName", _allSamplesLoaded },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

int MOAIFmodMusicInfo::_starving(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicInfo, "U")
		state.Push(self->mStarving);
	return 1;
}

int MOAIFmodMusicInfo::_allSamplesLoaded(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicInfo, "U")
		state.Push(self->mAllSamplesLoaded);
	return 1;
}