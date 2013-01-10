// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFmodMusicSystem.h>
#include <moaicore/MOAIFmodMusicEntity.h>
#include <moaicore/MOAIFmodMusicInfo.h>
#include <moaicore/MOAIFmodReverbChannelproperties.h>
#include <iostream>

//================================================================//
// lua
//================================================================//

// example static function
//int MOAIFmodMusicSystem::_classHello ( lua_State* L ) {
//	UNUSED ( L );
//	
//	printf ( "MOAIFmodMusicSystem class!\n" );
//	
//	return 0;
//}


void FmodErrorCheck(FMOD_RESULT result)	// this is an error handling function
{						// for FMOD errors
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}


MOAIFmodMusicSystem::MOAIFmodMusicSystem() : mSoundSystem(0), mEventSystem(0) {

	
	//FmodErrorCheck(mEventSystem->init(64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
	// register all classes MOAIFoo derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND(MOAIFmodMusicSystem)
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooBase )
	RTTI_END

	//FmodErrorCheck(FMOD::EventSystem_Create(&mEventSystem));
}

MOAIFmodMusicSystem::~MOAIFmodMusicSystem() {
}

int MOAIFmodMusicSystem::_freeSoundData(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UB")
	bool waitUntilReady	= state.GetValue<bool>(2, false);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->freeSoundData(waitUntilReady);

		if(res == FMOD_OK) {
			std::cout << "sound data freed" << std::endl;
		}
		else {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}

	return 0;
}


int MOAIFmodMusicSystem::_getCues(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "US")
	const char* filter = state.GetValue<const char*>(2, "");

	if(self->mSoundSystem) {
		FMOD_MUSIC_ITERATOR* it;
		FMOD_RESULT res = self->mSoundSystem->getCues(it, filter);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodMusicEntity* e = new MOAIFmodMusicEntity(*it);
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			e->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_getInfo(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")
	
	if(self->mSoundSystem) {
		FMOD_MUSIC_INFO info;

		FMOD_RESULT res = self->mSoundSystem->getInfo(&info);
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodMusicInfo* i = new MOAIFmodMusicInfo(info);
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			i->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}

	return 0;
}

//int MOAIFmodMusicSystem::_getMemoryInfo(lua_State* L) {
//	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")
//	
//	// not implemented so far
//	// dont think this method is needed at MOAI side
//
//	if(self->mSoundSystem) {
//
//	}
//	else {
//		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
//	}
//
//	return 0;
//}

int MOAIFmodMusicSystem::_getMute(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")

	if(self->mSoundSystem) {
		bool* mute;
		FMOD_RESULT res = self->mSoundSystem->getMute(mute);
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
		else {
			state.Push(mute);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}

	return 0;
}

int MOAIFmodMusicSystem::_getNextCue(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "US")
	const char* filter = state.GetValue<const char*>(2, "");

	if(self->mSoundSystem) {
		FMOD_MUSIC_ITERATOR* it;
		FMOD_RESULT res = self->mSoundSystem->getNextCue(it);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodMusicEntity* e = new MOAIFmodMusicEntity(*it);
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			e->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_getNextParameter(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "US")
	const char* filter = state.GetValue<const char*>(2, "");

	if(self->mSoundSystem) {
		FMOD_MUSIC_ITERATOR* it;
		FMOD_RESULT res = self->mSoundSystem->getNextParameter(it);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodMusicEntity* e = new MOAIFmodMusicEntity(*it);
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			e->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_getParameterValue(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UN")
	
	FMOD_MUSIC_PARAM_ID id = state.GetValue<FMOD_MUSIC_PARAM_ID>(2, 0);
	if(self->mSoundSystem) {
		float* parameter;
		FMOD_RESULT res = self->mSoundSystem->getParameterValue(id, parameter);

		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			state.Push(*parameter);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;

}

int MOAIFmodMusicSystem::_getParameters(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "US")
	const char* filter = state.GetValue<const char*>(2, 0);

	if(self->mSoundSystem) {
		FMOD_MUSIC_ITERATOR* it;
		FMOD_RESULT res = self->mSoundSystem->getParameters(it, filter);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodMusicEntity* e = new MOAIFmodMusicEntity(*it);
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			e->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_getPaused(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UB")

	if(self->mSoundSystem) {
		bool* paused;
		FMOD_RESULT res = self->mSoundSystem->getPaused(paused);
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
		else {
			state.Push(paused);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}

	return 0;
}

int MOAIFmodMusicSystem::_getReverbProperties(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")
	if(self->mSoundSystem) {
		FMOD_REVERB_CHANNELPROPERTIES* props;
		FMOD_RESULT res = self->mSoundSystem->getReverbProperties(props);
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			MOAIFmodReverbChannelproperties* p = new MOAIFmodReverbChannelproperties;
			p->mDirect = props->Direct;
			p->mRoom = props->Room;
			p->mFlags = props->Flags;
			// BindToLua does all the work for us here
			// dont delete e, Lua takes care of it now!
			p->BindToLua(state);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_getVolume(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UN")
	
	FMOD_MUSIC_PARAM_ID id = state.GetValue<FMOD_MUSIC_PARAM_ID>(2, 0);
	if(self->mSoundSystem) {
		float* volume;
		FMOD_RESULT res = self->mSoundSystem->getVolume(volume);

		if(res != FMOD_OK) {
			FmodErrorCheck(res);
			return 0;
		}
		else {
			state.Push(*volume);
			return 1;
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_loadSoundData(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UNN")
	
	// FMOD_EVENT_RESOURCE is an enum
	// we get it as a number (int) from MOAI
	int resource = state.GetValue<int>(2, 0);
	FMOD_EVENT_MODE mode = state.GetValue<FMOD_EVENT_MODE>(3,0);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->loadSoundData(static_cast<FMOD_EVENT_RESOURCE>(resource), mode);

		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

// TODO
// dependend on MusicPrompt
int MOAIFmodMusicSystem::_prepareCue(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UNU")

	FMOD_MUSIC_CUE_ID id = state.GetValue<FMOD_MUSIC_CUE_ID>(2,0);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->promptCue(id);

		//MOAIFmodMusicPrompt* p = state.GetValue(2, NULL);
		//self->mSoundSystem->prepareCue(id, fmodpointertoprompt, need from MOAIFmodMusicPrompt class
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_promptCue(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UN")
	FMOD_MUSIC_CUE_ID id = state.GetValue<FMOD_MUSIC_CUE_ID>(2,0);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->promptCue(id);

		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_reset(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")
	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->reset();
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

// TODO
int MOAIFmodMusicSystem::_setCallback(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")
	if(self->mSoundSystem) {

	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_setMute(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UB")

	bool mute = state.GetValue<bool>(2, false);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->setMute(mute);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_setParameterValue(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UN")

	FMOD_MUSIC_PARAM_ID id = state.GetValue<FMOD_MUSIC_PARAM_ID>(2, 0);
	float parameter = state.GetValue<float>(3, 0.0f);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->setParameterValue(id, parameter);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_setPaused(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UB")

	bool paused = state.GetValue<bool>(2, false);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->setPaused(paused);
		
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}


int MOAIFmodMusicSystem::_setReverbProperties(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UNNN")

	int direct = state.GetValue<int>(2, 0);
	int room = state.GetValue<int>(3, 0);
	int flags = state.GetValue<int>(4, 0);

	if(self->mSoundSystem) {
		FMOD_REVERB_CHANNELPROPERTIES p;
		p.Direct = direct;
		p.Room = room;
		p.Flags = flags;
		p.ConnectionPoint = 0;

		FMOD_RESULT res = self->mSoundSystem->setReverbProperties(&p);

		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}


int MOAIFmodMusicSystem::_setVolume(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UN")

	float volume = state.GetValue(2, 0.0f);

	if(self->mSoundSystem) {
		FMOD_RESULT res = self->mSoundSystem->setVolume(volume);
		if(res != FMOD_OK) {
			FmodErrorCheck(res);
		}
	}
	else {
		std::cout << "soundsystem not initialized yet, cannot call method" << std::endl;
	}
	return 0;
}

int MOAIFmodMusicSystem::_setEventSystem(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UU")

	self->mEventSystem = reinterpret_cast<MOAIFmodEventSystem*>(state.GetUserData(2, NULL));

	return 0;
}

int MOAIFmodMusicSystem::_getEventSystem(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "U")

	// bind MOAIFmodEventSystem to LUA.. needs MOAIFmodEventSystem class
	return 1;
}


int MOAIFmodMusicSystem::_addMusicPrompt(lua_State* L) {
	MOAI_LUA_SETUP(MOAIFmodMusicSystem, "UU")

	MOAIFmodMusicPrompt* p = reinterpret_cast<MOAIFmodMusicPrompt*>(state.GetUserData(2, NULL));
	self->mPrompts.push_back(p);
}

// register static functions and constants
void MOAIFmodMusicSystem::RegisterLuaClass(MOAILuaState& state) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	//luaL_Reg regTable [] = {
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}

// register member functions
void MOAIFmodMusicSystem::RegisterLuaFuncs(MOAILuaState& state) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "freeSoundData", _freeSoundData },
		{ "getCues", _getCues },
		{ "getInfo", _getInfo },
		//{ "getMemoryInfo", _getMemoryInfo },
		{ "getMute", _getMute },
		{ "getNextCue", _getNextCue },
		{ "getNextParameter", _getNextParameter },
		{ "getParameterValue", _getParameterValue },
		{ "getParameters", _getParameters },
		{ "getPaused", _getPaused },
		{ "getReverbProperties", _getReverbProperties },
		{ "getVolume", _getVolume },
		{ "loadSoundData", _loadSoundData },
		{ "prepareCue", _prepareCue },
		{ "promptCue", _promptCue },
		{ "reset", _reset },
		//{ "setCallback", _setCallback },
		{ "setMute", _setMute },
		{ "setParameterValue", _setParameterValue },
		{ "setPaused", _setPaused },
		{ "setReverbProperties", _setReverbProperties },
		{ "setVolume", _setVolume },
		{ "setEventSystem", _setEventSystem},
		{ "getEventSystem", _getEventSystem},
		{ "addMusicPrompt", _addMusicPrompt},
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

