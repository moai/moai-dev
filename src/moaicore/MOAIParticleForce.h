// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEFORCE_H
#define	MOAIPARTICLEFORCE_H

#include <moaicore/MOAITransform.h>

class MOAIParticleSystem;

//================================================================//
// MOAIParticleForce
//================================================================//
/**	@brief Particle force.
*/
class MOAIParticleForce :
	public MOAITransform {
private:

	u32			mType;
	USVec2D		mVec;

	USVec2D		mWorldLoc;	
	USVec2D		mWorldVec;

	// attractor coefficients
	float		mRadius;
	float		mPull;

	//----------------------------------------------------------------//
	static int		_initAttractor			( lua_State* L );
	static int		_initLinear				( lua_State* L );
	
	//----------------------------------------------------------------//

public:
	
	enum {
		ATTRACTOR,
		LINEAR,
	};
	
	DECL_LUA_DATA ( MOAIParticleForce )

	//----------------------------------------------------------------//
	USVec2D			GetAcceleration			( const USVec2D& loc );
					MOAIParticleForce		();
					~MOAIParticleForce		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
