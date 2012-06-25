// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERBODY2D_H
#define	MOAIPLATFORMERBODY2D_H

#include <moaicore/MOAIProp.h>

class MOAISurfaceSampler2D;
class MOAIPartition;
class MOAIGrid;

//================================================================//
// MOAIPlatformerBody2D
//================================================================//
/**	@brief 2D collision feeler.
*/
class MOAIPlatformerBody2D :
	public virtual MOAIProp {
private:
	
	float		mFloorCos;	// mY must be >= to count as floor
	float		mCeilCos;	// mY must be <= to count as ceil
	
	USVec2D		mMove;
	float		mHRad;
	float		mVRad;
	
	float		mSkirt;
	float		mHat;
	
	//----------------------------------------------------------------//
	static int		_setMove				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			GatherSurfacesForMove	( MOAISurfaceSampler2D& sampler, USVec2D& move );
	void			GetRect					( USRect& rect );
	void			GetSweptRect			( USVec2D& move, USRect& rect );
	void			GetTouching				( USVec2D& loc, MOAISurfaceSampler2D& sampler );
	void			GetWorldMtx				( USAffine2D& transform );
	void			GetWorldMtxInv			( USAffine2D& transform );
	void			Move					();
	void			SetAngles				( float floorAngle, float ceilAngle );
	void			TransformToLocal		( USVec2D& point );
	void			TransformToWorld		( USVec2D& point );

public:
	
	DECL_LUA_FACTORY ( MOAIPlatformerBody2D )
	
	//----------------------------------------------------------------//
	void			Draw					( int subPrimID );
	u32				GetPropBounds			( USBox& bounds );
					MOAIPlatformerBody2D	();
					~MOAIPlatformerBody2D	();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetMove					( float x, float y );
	STLString		ToString				();
};

#endif
