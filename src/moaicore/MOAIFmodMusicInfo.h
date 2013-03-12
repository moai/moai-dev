#ifndef MOAIFMODMUSICINFO__H
#define MOAIFMODMUSICINFO__H

#include <moaicore/MOAILua.h>
#include "fmodinc/fmod.hpp"
#include "fmoddesignerinc/fmod_event.hpp"

class MOAIFmodMusicInfo :
	public virtual MOAILuaObject {

friend class MOAIFmodMusicSystem;

private:

	static int _starving(lua_State* L);
	static int _allSamplesLoaded(lua_State* L);

	FMOD_BOOL mStarving;
	FMOD_BOOL mAllSamplesLoaded;


public:
	DECL_LUA_FACTORY(MOAIFmodMusicInfo)

	MOAIFmodMusicInfo() : mStarving(false), mAllSamplesLoaded(false) {
		// register all classes MOAIFoo derives from
		// we need this for custom RTTI implementation
		RTTI_BEGIN
			RTTI_EXTEND ( MOAILuaObject )
		
			// and any other objects from multiple inheritance...
			// RTTI_EXTEND ( MOAIFooBase )
		RTTI_END
	}

	MOAIFmodMusicInfo(FMOD_MUSIC_INFO info) : mStarving(info.starving), mAllSamplesLoaded(info.all_samples_loaded) {
		// register all classes MOAIFoo derives from
		// we need this for custom RTTI implementation
		RTTI_BEGIN
			RTTI_EXTEND ( MOAILuaObject )
		
			// and any other objects from multiple inheritance...
			// RTTI_EXTEND ( MOAIFooBase )
		RTTI_END
	}

	
	~MOAIFmodMusicInfo() {}
	
	void RegisterLuaFuncs(MOAILuaState& state);
	
	
	
};

#endif