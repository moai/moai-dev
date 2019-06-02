// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEFORCE_H
#define	MOAIPARTICLEFORCE_H

#include <moai-sim/MOAITransform.h>

class MOAIParticle;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleForce
//================================================================//
/**	@lua	MOAIParticleForce
	@text	Particle force.

	@const	FORCE
	@const	GRAVITY
	@const	OFFSET
*/
class MOAIParticleForce :
	public MOAITransform {
private:

	u32			mShape;
	u32			mType;
	ZLVec3D		mVec;

	ZLVec3D		mWorldLoc;	
	ZLVec3D		mWorldVec;

	// attractor coefficients
	float		mRadius;
	float		mPull;

	//----------------------------------------------------------------//
	static int		_initAttractor			( lua_State* L );
	static int		_initBasin				( lua_State* L );
	static int		_initLinear				( lua_State* L );
	static int		_initRadial				( lua_State* L );
	static int		_setType				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAINode_Update						();

public:
	
	enum {
		ATTRACTOR,
		BASIN,
		LINEAR,
		RADIAL,
	};
	
	enum {
		FORCE,
		GRAVITY,
		OFFSET,
	};
	
	DECL_LUA_FACTORY ( MOAIParticleForce )

	//----------------------------------------------------------------//
	void			Eval					( const ZLVec3D& loc, float mass, ZLVec3D& acceleration, ZLVec3D& offset );
					MOAIParticleForce		();
					~MOAIParticleForce		();
};

#endif
