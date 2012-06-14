// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEDISTANCEEMITTER_H
#define	MOAIPARTICLEDISTANCEEMITTER_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIParticleEmitter.h>

//================================================================//
// MOAIParticleDistanceEmitter
//================================================================//
/**	@name	MOAIParticleDistanceEmitter
	@text	Particle emitter.
*/
class MOAIParticleDistanceEmitter :
	public MOAIParticleEmitter {
private:
	
	bool		mReset;
	
	USVec3D		mEmitLoc;			// location of the last emission
	
	float		mMinDistance;
	float		mMaxDistance;
	
	float		mEmitDistance;		// distance at which next emission will occur

	//----------------------------------------------------------------//
	static int		_reset					( lua_State* L );
	static int		_setDistance			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetRandomDistance		();
	void			OnDepNodeUpdate			();
	void			OnUpdate				( float step );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleDistanceEmitter )

	//----------------------------------------------------------------//
					MOAIParticleDistanceEmitter		();
					~MOAIParticleDistanceEmitter	();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
	void			SetDistanceRange				( float min, float max );
};

#endif
