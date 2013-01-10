#ifndef MOAIFMODMUSICENTITY__H
#define MOAIFMODMUSICENTITY__H

#include <moaicore/MOAILua.h>
#include "fmodinc/fmod.hpp"
#include "fmoddesignerinc/fmod_event.hpp"

class MOAIFmodMusicEntity :
	public virtual MOAILuaObject {

friend class MOAIFmodMusicSystem;

private:

	static int _getID(lua_State* L);
	static int _getName(lua_State* L);

	FMOD_MUSIC_ID mID;
	const char* mName;

public:
	DECL_LUA_FACTORY(MOAIFmodMusicEntity)

	MOAIFmodMusicEntity() : mID(0), mName(0) {
		// register all classes MOAIFoo derives from
		// we need this for custom RTTI implementation
		RTTI_BEGIN
			RTTI_EXTEND ( MOAILuaObject )
		
			// and any other objects from multiple inheritance...
			// RTTI_EXTEND ( MOAIFooBase )
		RTTI_END
	}
	MOAIFmodMusicEntity(const FMOD_MUSIC_ITERATOR& it) : mID(it.value->id), mName(it.value->name) {
		// register all classes MOAIFoo derives from
		// we need this for custom RTTI implementation
		RTTI_BEGIN
			RTTI_EXTEND ( MOAILuaObject )
		
			// and any other objects from multiple inheritance...
			// RTTI_EXTEND ( MOAIFooBase )
		RTTI_END
	}
	~MOAIFmodMusicEntity() {}
	
	void RegisterLuaFuncs(MOAILuaState& state);
	
	
	
};

#endif