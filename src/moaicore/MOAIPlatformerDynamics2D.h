// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPLATFORMERDYNAMICS2D_H
#define	MOAIPLATFORMERDYNAMICS2D_H

#include <moaicore/MOAIAction.h>

class MOAIPlatformerBody2D;

//================================================================//
// MOAIPlatformerDynamics2D
//================================================================//
// TODO: doxygen
class MOAIPlatformerDynamics2D :
	public virtual MOAIAction {
private:
	
	float		mJumpMid;
	float		mJumpTop;
	float		mJumpDuration;
	float		mJumpElapsed;
	u32			mJumpMode;
	
	float		mFallMid;
	float		mFallBottom;
	float		mFallDuration;
	float		mFallTerminal;
	u32			mFallMode;
	
	u32			mFlags;
	
	enum {
		JUMP			= 1 << 0,
		JUMP_BOOST		= 1 << 1,
	};
	
	MOAILuaSharedPtr < MOAIPlatformerBody2D > mBody; // bounds override
	
	//----------------------------------------------------------------//
	static int		_drawJumpArc					( lua_State* L );
	static int		_setBody						( lua_State* L );
	static int		_setFallParams					( lua_State* L );
	static int		_setJumpParams					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIPlatformerDynamics2D )
	
	//----------------------------------------------------------------//
	void			DrawJumpArc						( u32 resolution, float xMove );
	float			EvalFall						( float t );
	float			EvalJump						( float t );
	bool			IsDone							();
					MOAIPlatformerDynamics2D		();
					~MOAIPlatformerDynamics2D		();
	void			OnUpdate						( float step );
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
