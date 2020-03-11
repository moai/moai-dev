// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEPLUGIN_H
#define	MOAIPARTICLEPLUGIN_H

#include <moai-sim/host_particles.h>

//================================================================//
// MOAIParticlePlugin
//================================================================//
/**	@lua	MOAIParticlePlugin
	@text	Allows custom particle processing.
*/
class MOAIParticlePlugin :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	int				mSize;

	//----------------------------------------------------------------//
	static int		_getSize			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	//----------------------------------------------------------------//
					MOAIParticlePlugin			();
					~MOAIParticlePlugin			();	
	virtual void	OnInit						( float* particle, float* registers ) = 0;
	virtual void	OnRender					( float* particle, float* registers, AKUParticleSprite* sprite, float t0, float t1, float term ) = 0;
};

#endif
