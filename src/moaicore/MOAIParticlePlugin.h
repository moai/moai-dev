// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEPLUGIN_H
#define	MOAIPARTICLEPLUGIN_H

#include <moaicore/MOAILua.h>
#include <aku/AKU-particles.h>

//================================================================//
// MOAIParticlePlugin
//================================================================//
/**	@name	MOAIParticlePlugin
	@text	Allows custom particle processing.
*/
class MOAIParticlePlugin :
	public virtual MOAILuaObject {
protected:

	int				mSize;

	//----------------------------------------------------------------//
	static int		_getSize			( lua_State* L );

public:
	
	//----------------------------------------------------------------//
					MOAIParticlePlugin			();
					~MOAIParticlePlugin			();	
	virtual void	OnInit						( float* particle, float* registers ) = 0;
	virtual void	OnRender					( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term ) = 0;
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
