// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLECALLBACKPLUGIN_H
#define	MOAIPARTICLECALLBACKPLUGIN_H

#include <moaicore/MOAIParticlePlugin.h>

//================================================================//
// MOAIParticleCallbackPlugin
//================================================================//
/**	@name	MOAIParticleCallbackPlugin
	@text	Allows custom particle processing via C language callbacks.
*/
class MOAIParticleCallbackPlugin :
	public virtual MOAIParticlePlugin {
private:

	AKUParticleInitFunc			mInitFunc;
	AKUParticleRenderFunc		mRenderFunc;

public:
	
	DECL_LUA_FACTORY ( MOAIParticleCallbackPlugin )
	
	//----------------------------------------------------------------//
	void			Init							( AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size );
					MOAIParticleCallbackPlugin		();
					~MOAIParticleCallbackPlugin		();
	void			OnInit							( float* particle, float* registers );
	void			OnRender						( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term );
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
