// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIEASEDRIVER_H
#define MOAIEASEDRIVER_H

#include <moaicore/MOAIAction.h>

class MOAINode;

//================================================================//
// MOAIEaseAttr
//================================================================//
class MOAIEaseAttr {
public:
	USRef < MOAINode >	mTarget;
	u32					mAttrID;
	float				mForce;
	u32					mMode;
};

//================================================================//
// MOAIEaseDriver
//================================================================//
/**	@brief Simple animation class.
*/
class MOAIEaseDriver :
	public MOAIAction {
private:

	USLeanArray < MOAIEaseAttr > mForces;
	
	float	mTime;
	float	mDelay;
	u32		mMode;

	//----------------------------------------------------------------//
	static int		_reserveForces		( lua_State* L );
	static int		_setDelay			( lua_State* L );
	static int		_setForce			( lua_State* L );
	static int		_setMode			( lua_State* L );

	//----------------------------------------------------------------//
	void			Apply				( float step );

public:

	DECL_LUA_DATA ( MOAIEaseDriver )

	SET ( float, Delay, mDelay )
	SET ( u32, Mode, mMode )

	//----------------------------------------------------------------//
	bool			IsBusy				();
					MOAIEaseDriver		();
					~MOAIEaseDriver		();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			ReserveForces		( u32 total );
	void			SetForce			( u32 idx, MOAINode* target, u32 attrID, float force, u32 mode );
	void			SetForce			( u32 mode );
	STLString		ToString			();
};

#endif
