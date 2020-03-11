// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLETIMEDEMITTER_H
#define	MOAIPARTICLETIMEDEMITTER_H

#include <moai-sim/MOAIParticleEmitter.h>

//================================================================//
// MOAIParticleTimedEmitter
//================================================================//
/**	@lua	MOAIParticleTimedEmitter
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
	static int		_setFrequency					( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetRandomFrequency				();
	
	void			MOAIAction_Update				( double step );
	void			_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAIParticleTimedEmitter )

	//----------------------------------------------------------------//
					MOAIParticleTimedEmitter		();
					~MOAIParticleTimedEmitter		();
	void			SetFrequencyRange				( float min, float max );
};

#endif
