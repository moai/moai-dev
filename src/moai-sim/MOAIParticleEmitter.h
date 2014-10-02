// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLEEMITTER_H
#define	MOAIPARTICLEEMITTER_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAITransform.h>

class MOAIParticleSystem;

//================================================================//
// MOAIParticleEmitter
//================================================================//
/**	@lua	MOAIParticleEmitter
	@text	Particle emitter.
*/
class MOAIParticleEmitter :
	public MOAITransform,
	public MOAIAction {
protected:

	u32		mShapeID;
	ZLRect	mRect;
	
	float	mInnerRadius;
	float	mOuterRadius;
	
	u32		mMinEmission;
	u32		mMaxEmission;

	float	mMinAngle;
	float	mMaxAngle;
	
	float	mMinMagnitude;
	float	mMaxMagnitude;

	u32		mEmission;

	MOAILuaSharedPtr < MOAIParticleSystem > mSystem;

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
	void			GetRandomParticle		( ZLVec3D& loc, ZLVec3D& vec ); // in local space
	ZLVec3D			GetRandomVec			( float minAngle, float maxAngle, float min, float max );

public:
	
	enum {
		POINT,
		CIRCLE,
		RECT,
	};
	
	DECL_LUA_FACTORY ( MOAIParticleEmitter )

	SET ( ZLRect, Rect, mRect )
	SET ( u32, ShapeID, mShapeID )

	//----------------------------------------------------------------//
	bool			IsDone					();
					MOAIParticleEmitter		();
					~MOAIParticleEmitter	();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetAngleRange			( float min, float max );
	void			SetEmissionRange		( u32 min, u32 max );
	void			SetMagnitudeRange		( float min, float max );
	void			Surge					( u32 total );
};

#endif
