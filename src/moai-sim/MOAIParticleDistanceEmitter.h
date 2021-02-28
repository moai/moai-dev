// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEDISTANCEEMITTER_H
#define	MOAIPARTICLEDISTANCEEMITTER_H

#include <moai-sim/MOAIParticleEmitter.h>

//================================================================//
// MOAIParticleDistanceEmitter
//================================================================//
/**	@lua	MOAIParticleDistanceEmitter
	@text	Particle emitter.
*/
class MOAIParticleDistanceEmitter :
	public MOAIParticleEmitter {
private:
	
	bool		mReset;
	
	ZLVec3D		mEmitLoc;			// location of the last emission
	
	float		mMinDistance;
	float		mMaxDistance;
	
	float		mEmitDistance;		// distance at which next emission will occur

	//----------------------------------------------------------------//
	static int		_reset					( lua_State* L );
	static int		_setDistance			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetRandomDistance		();
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAction_Update		( double step );
	void			MOAINode_Update			();

public:
	
	DECL_LUA_FACTORY ( MOAIParticleDistanceEmitter )

	//----------------------------------------------------------------//
					MOAIParticleDistanceEmitter		( ZLContext& context );
					~MOAIParticleDistanceEmitter	();
	void			SetDistanceRange				( float min, float max );
};

#endif
