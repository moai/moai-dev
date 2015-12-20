// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPATHSTEPPER_H
#define	MOAIPATHSTEPPER_H

#include <moai-sim/MOAIPath.h>

class MOAIPath;

//================================================================//
// MOAIPathStepper
//================================================================//
// TODO: doxygen
class MOAIPathStepper :
	public virtual MOAILuaObject {
private:

	float							mT;
	float							mStep;
	
	ZLVec2D							mSamples [ 3 ];
	ZLVec2D							mNormal;

	MOAILuaSharedPtr < MOAIPath>	mPath;

	bool							mIsStepping;
	bool							mStepByDistance;

	//----------------------------------------------------------------//
	static int		_reset				( lua_State* L );
	static int		_setPath			( lua_State* L );
	static int		_setStepSize		( lua_State* L );
	static int		_step				( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAILuaObject )
	
	//----------------------------------------------------------------//
					MOAIPathStepper		();
					~MOAIPathStepper	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Reset				();
	void			Step				();
};

#endif
