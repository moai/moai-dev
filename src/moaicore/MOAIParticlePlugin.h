// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEPLUGIN_H
#define	MOAIPARTICLEPLUGIN_H

#include <aku/AKU-particles.h>

//================================================================//
// MOAIParticlePlugin
//================================================================//
/**	@name	MOAIParticlePlugin
	@text	Allows custom particle processing via C language callbacks.
*/
class MOAIParticlePlugin :
	public virtual USLuaObject {
private:

	int							mSize;
	AKUParticleInitFunc			mInitFunc;
	AKUParticleRenderFunc		mRenderFunc;

	//----------------------------------------------------------------//
	static int		_getSize			( lua_State* L );

public:
	
	friend class MOAIParticleState;
	
	DECL_LUA_FACTORY ( MOAIParticlePlugin )
	
	//----------------------------------------------------------------//
	void			Init						( AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size );
					MOAIParticlePlugin			();
					~MOAIParticlePlugin			();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );
};

#endif
