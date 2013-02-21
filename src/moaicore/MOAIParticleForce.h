// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEFORCE_H
#define	MOAIPARTICLEFORCE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

class MOAIParticle;
class MOAIParticleSystem;

//================================================================//
// MOAIParticleForce
//================================================================//
/**	@name	MOAIParticleForce
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
	USVec3D		mVec;

	USVec3D		mWorldLoc;	
	USVec3D		mWorldVec;

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
	void			Eval					( const USVec3D& loc, float mass, USVec3D& acceleration, USVec3D& offset );
					MOAIParticleForce		();
					~MOAIParticleForce		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
