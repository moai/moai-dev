// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEEMITTER_H
#define	MOAIPARTICLEEMITTER_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAITransform.h>

class MOAIParticleSystem;

//================================================================//
// MOAIParticleEmitter
//================================================================//
/**	@name	MOAIParticleEmitter
	@text	Particle emitter.
*/
class MOAIParticleEmitter :
	public MOAITransform,
	public MOAIAction {
protected:

	u32		mShapeID;
	USRect	mRect;
	
	float	mInnerRadius;
	float	mOuterRadius;
	
	u32		mMinEmission;
	u32		mMaxEmission;

	float	mMinAngle;
	float	mMaxAngle;
	
	float	mMinMagnitude;
	float	mMaxMagnitude;

	USRef < MOAIParticleSystem > mSystem;

	//----------------------------------------------------------------//
	static int		_setAngle				( lua_State* L );
	static int		_setEmission			( lua_State* L );
	static int		_setMagnitude			( lua_State* L );
	static int		_setRadius				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setSystem				( lua_State* L );
	static int		_surge					( lua_State* L );
	
	//----------------------------------------------------------------//
	u32				GetRandomEmission		();
	void			GetRandomParticle		( USVec2D& loc, USVec2D& vec ); // in local space
	USVec2D			GetRandomVec			( float minAngle, float maxAngle, float min, float max );

public:
	
	enum {
		POINT,
		CIRCLE,
		RECT,
	};
	
	DECL_LUA_FACTORY ( MOAIParticleEmitter )

	SET ( USRect, Rect, mRect )
	SET ( u32, ShapeID, mShapeID )
	SET ( MOAIParticleSystem*, System, mSystem )

	//----------------------------------------------------------------//
	bool			IsDone					();
					MOAIParticleEmitter		();
					~MOAIParticleEmitter	();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SetAngleRange			( float min, float max );
	void			SetEmissionRange		( u32 min, u32 max );
	void			SetMagnitudeRange		( float min, float max );
	void			Surge					( u32 total );
	STLString		ToString				();
};

#endif
