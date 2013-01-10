// Copyright (c) 2012 Lukas Oberbichler & Christian Ondracek

#ifndef MOAIFMODMUSICSYSTEM__H
#define MOAIFMODMUSICSYSTEM__H

#include <moaicore/MOAILua.h>
#include "fmodinc/fmod.hpp"
#include "fmodinc/fmod_errors.h"
#include "fmoddesignerinc/fmod_event.hpp"
#include <vector>

//================================================================//
// MOAIFmodMusicSystem
//================================================================//

// lua c++ function wrapper returns 0 if no value is returned to lua
// and 1 if a value is returned to lua
// the actual value return is set by lua_push....()

class MOAIFmodEventSystem;
class MOAIFmodMusicPrompt;

class MOAIFmodMusicSystem :
	public virtual MOAILuaObject {
private:
	static int _freeSoundData(lua_State* L);
	static int _getCues(lua_State* L);
	static int _getInfo(lua_State* L);
	static int _getMemoryInfo(lua_State* L);
	static int _getMute(lua_State* L);
	static int _getNextCue(lua_State* L);
	static int _getNextParameter(lua_State* L);
	static int _getParameterValue(lua_State* L);
	static int _getParameters(lua_State* L);
	static int _getPaused(lua_State* L);
	static int _getReverbProperties(lua_State* L);
	static int _getVolume(lua_State* L);
	static int _loadSoundData(lua_State* L);
	static int _prepareCue(lua_State* L);
	static int _promptCue(lua_State* L);
	static int _reset(lua_State* L);
	static int _setCallback(lua_State* L);
	static int _setMute(lua_State* L);
	static int _setParameterValue(lua_State* L);
	static int _setPaused(lua_State* L);
	static int _setReverbProperties(lua_State* L);
	static int _setVolume(lua_State* L);
	static int _setEventSystem(lua_State* L);
	static int _getEventSystem(lua_State* L);
	static int _addMusicPrompt(lua_State* L);

	FMOD::MusicSystem* mSoundSystem;
	//FMOD::EventSystem* mEventSystem;

	MOAIFmodEventSystem* mEventSystem;
	std::vector<MOAIFmodMusicPrompt*> mPrompts;

public:
	
	DECL_LUA_FACTORY(MOAIFmodMusicSystem)

	//----------------------------------------------------------------//
	MOAIFmodMusicSystem();
	~MOAIFmodMusicSystem();
	void RegisterLuaClass(MOAILuaState& state);
	void RegisterLuaFuncs(MOAILuaState& state);
};

#endif
