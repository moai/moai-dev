#ifndef MOAIFMODREVERBCHANNELPROPERTIES__H
#define MOAIFMODREVERBCHANNELPROPERTIES__H

#include <moaicore/MOAILua.h>
#include "fmodinc/fmod.hpp"

class MOAIFmodReverbChannelproperties :
	public virtual MOAILuaObject {

friend class MOAIFmodMusicSystem;

private:

	static int _getDirect(lua_State* L);
	static int _getRoom(lua_State* L);
	static int _getFlags(lua_State* L);

	int mDirect;
    int mRoom;
    unsigned int mFlags;
	// not used just there for the sake of completeness
	FMOD::DSP* mConnectionPoint;

public:
	DECL_LUA_FACTORY(MOAIFmodReverbChannelproperties)

	MOAIFmodReverbChannelproperties() : mDirect(0), mRoom(0), mFlags(0), mConnectionPoint(0) {
		// register all classes MOAIFoo derives from
		// we need this for custom RTTI implementation
		RTTI_BEGIN
			RTTI_EXTEND (MOAILuaObject)
		
			// and any other objects from multiple inheritance...
			// RTTI_EXTEND ( MOAIFooBase )
		RTTI_END
	}
	
	~MOAIFmodReverbChannelproperties() {}
	
	void RegisterLuaFuncs(MOAILuaState& state);

};

#endif