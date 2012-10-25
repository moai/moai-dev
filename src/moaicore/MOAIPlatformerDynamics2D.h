// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERDYNAMICS2D_H
#define	MOAIPLATFORMERDYNAMICS2D_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIFsm.h>

class MOAIPlatformerBody2D;

//================================================================//
// MOAIPlatformerArc
//================================================================//
class MOAIPlatformerArc {
private:

	//----------------------------------------------------------------//
	static float		EvalCurve1D		( const USVec2D& v0, const USVec2D& v1, float t );
	static USVec2D		EvalCurve2D		( const USVec2D& v0, const USVec2D& v1, float t );
	static float		FindYForX		( const USVec2D& v0, const USVec2D& v1, float x );
	void				GetPoints		( USVec2D* points ); // array of 4 points

public:

	USVec2D		mMidHandle;
	USVec2D		mTopHandle;
	float		mDuration;
	bool		mReverse;

	//----------------------------------------------------------------//
	void		Draw					( u32 resolution, float xOff, float yOff, float xScl, float yScl );
	void		DrawHull				( float xOff, float yOff, float xScl, float yScl );
	void		DrawPoints				( float xOff, float yOff, float xScl, float yScl );
	float		Evaluate				( float time );
				MOAIPlatformerArc		();
				~MOAIPlatformerArc		();
	void		SetDuration				( float duration, bool reverse );
	void		SetMidHandle			( float time, float value );
	void		SetTopHandle			( float time, float value );
};

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//
// TODO: doxygen
class MOAIPlatformerDynamics2D :
	public virtual MOAIAction {
private:
	
	friend class MOAIPlatformerDynamics2D_Fall;
	friend class MOAIPlatformerDynamics2D_Idle;
	friend class MOAIPlatformerDynamics2D_Jump;
	
	MOAIFsm		mFSM;
	
	MOAIPlatformerArc	mJumpArc;
	MOAIPlatformerArc	mFallArc;
	
	USVec2D		mFallMidHandle;
	USVec2D		mFallTopHandle;
	float		mFallDuration;
	
	float		mStateTimer;
	float		mStateY;
	
	float		mWalkAcceleration;
	float		mWalkMax;
	float		mWalkDrag;
	
	float		mXMove;
	USVec2D		mMove;
	
	MOAILuaSharedPtr < MOAIPlatformerBody2D > mBody;
	
	//----------------------------------------------------------------//
	static int		_drawJumpArc					( lua_State* L );
	static int		_drawJumpHull					( lua_State* L );
	static int		_drawJumpPoints					( lua_State* L );
	static int		_jump							( lua_State* L );
	static int		_setBody						( lua_State* L );
	static int		_setFallParams					( lua_State* L );
	static int		_setJumpParams					( lua_State* L );
	static int		_setWalkParams					( lua_State* L );

	//----------------------------------------------------------------//
	void			AccumulateMove					( float x, float y );
	static float	ApplyDrag						( float n, float d );
	void			UpdateXMove						( float step );

public:
	
	DECL_LUA_FACTORY ( MOAIPlatformerDynamics2D )
	
	//----------------------------------------------------------------//
	void			DrawJumpArc						( u32 resolution, float xMove );
	void			DrawJumpHull					( float xMove );
	void			DrawJumpPoints					( float xMove );
	bool			IsDone							();
					MOAIPlatformerDynamics2D		();
					~MOAIPlatformerDynamics2D		();
	void			OnUpdate						( float step );
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
