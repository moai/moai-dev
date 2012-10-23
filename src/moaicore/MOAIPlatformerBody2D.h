// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERBODY2D_H
#define	MOAIPLATFORMERBODY2D_H

#include <moaicore/MOAIProp.h>

class MOAISurfaceBuffer2D;
class MOAIPartition;
class MOAIGrid;

//================================================================//
// MOAIPlatformerBody2D
//================================================================//
// TODO: doxygen
class MOAIPlatformerBody2D :
	public virtual MOAIProp {
private:
	
	friend class MOAIPlatformerFsm2D;
	
	float		mFloorAngle;
	float		mFloorCos;
	
	float		mCeilAngle;
	float		mCeilCos;
	
	USVec2D		mMove;
	float		mHRad;
	float		mVRad;
	
	float		mSkirt;
	
	u32			mSteps;
	bool		mCompleted;
	
	u32			mDetachMode;
	
	bool		mIsStanding;
	
	//----------------------------------------------------------------//
	static int		_getStatus				( lua_State* L );
	static int		_setCeilingAngle		( lua_State* L );
	static int		_setEllipse				( lua_State* L );
	static int		_setFloorAngle			( lua_State* L );
	static int		_setMove				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				GatherSurfacesForBounds		( MOAISurfaceBuffer2D& buffer, const USBox& bounds );
	void				GatherSurfacesForMove		( MOAISurfaceBuffer2D& buffer, USVec2D& move );
	void				GetRect						( USRect& rect );
	void				GetSweptRect				( USVec2D& move, USRect& rect );
	void				GetTouching					( USVec2D& loc, MOAISurfaceSampler2D& sampler );
	USRect				GetUnitRectForWorldBounds	( const USBox& bounds );
	USAffine3D			GetUnitToWorldMtx			();
	USAffine3D			GetWorldToUnitMtx			();
	void				SetCeilingAngle				( float angle );
	void				SetFloorAngle				( float angle );

public:
	
	enum {
		DETACH_MODE_NONE,
		DETACH_ON_UP,
		DETACH_ON_ANY,
	};
	
	DECL_LUA_FACTORY ( MOAIPlatformerBody2D )
	
	GET_BOOL ( IsStanding, mIsStanding )
	
	//----------------------------------------------------------------//
	void			Draw					( int subPrimID );
	u32				GetPropBounds			( USBox& bounds );
					MOAIPlatformerBody2D	();
					~MOAIPlatformerBody2D	();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetMove					( float x, float y );
};

#endif
