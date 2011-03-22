// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFORCEACTION_H
#define MOAIFORCEACTION_H

#include <moaicore/MOAIAction.h>

class MOAINode;

//================================================================//
// MOAIForceAttr
//================================================================//
class MOAIForceAttr {
public:
	USRef < MOAINode >	mTarget;
	u32					mAttrID;
	float				mForce;
	u32					mMode;
};

//================================================================//
// MOAIForceAction
//================================================================//
/**	@brief Simple animation class.
*/
class MOAIForceAction :
	public MOAIAction {
private:

	USLeanArray < MOAIForceAttr > mForces;
	
	float				mTime;
	float				mDelay;
	u32					mMode;

	//----------------------------------------------------------------//
	static int		_reserveForces		( lua_State* L );
	static int		_setDelay			( lua_State* L );
	static int		_setForce			( lua_State* L );
	static int		_setMode			( lua_State* L );

	//----------------------------------------------------------------//
	void			Apply				( float step );

public:

	DECL_LUA_DATA ( MOAIForceAction )

	SET ( float, Delay, mDelay )
	SET ( u32, Mode, mMode )

	//----------------------------------------------------------------//
	bool			IsBusy				();
					MOAIForceAction		();
					~MOAIForceAction	();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			ReserveForces		( u32 total );
	void			SetForce			( u32 idx, MOAINode* target, u32 attrID, float force, u32 mode );
	void			SetForce			( u32 mode );
	STLString		ToString			();
};

#endif
