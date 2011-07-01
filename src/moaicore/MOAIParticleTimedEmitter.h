// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLETIMEDEMITTER_H
#define	MOAIPARTICLETIMEDEMITTER_H

#include <moaicore/MOAIParticleEmitter.h>

//================================================================//
// MOAIParticleTimedEmitter
//================================================================//
/**	@name	MOAIParticleTimedEmitter
	@text	Particle emitter.
*/
class MOAIParticleTimedEmitter :
	public MOAIParticleEmitter {
private:
	
	float	mTime;
	float	mEmitTime;
	
	float	mMinFrequency;
	float	mMaxFrequency;

	//----------------------------------------------------------------//
	static int		_setFrequency			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetRandomFrequency		();
	void			OnUpdate				( float step );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleTimedEmitter )


	//----------------------------------------------------------------//
					MOAIParticleTimedEmitter	();
					~MOAIParticleTimedEmitter	();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );
	void			SetFrequencyRange			( float min, float max );
	STLString		ToString					();
};

#endif
