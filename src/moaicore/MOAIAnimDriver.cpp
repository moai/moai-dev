// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIAnimDriver.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>setAnim ( self, anim )</tt>\n
\n
	Sets the anim for this anim player.
	@param self (in)
	@param anim (in) The MOAIAnim object to be bound.
*/
int MOAIAnimDriver::_setAnim ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimDriver, "UU" );

	MOAIAnim* anim = state.GetLuaData < MOAIAnim >( 2 );
	if ( !anim ) return 0;

	self->mAnim = anim;
	self->mStartTime = 0.0f;
	self->mEndTime = anim->mLength;

	return 0;
}

//================================================================//
// MOAIAnimDriver
//================================================================//

//----------------------------------------------------------------//
MOAIAnimDriver::MOAIAnimDriver () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITimer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAnimDriver::~MOAIAnimDriver () {
}

//----------------------------------------------------------------//
void MOAIAnimDriver::OnUpdate ( float step ) {

	float t0 = this->mTime;
	float t1 = t0 + this->DoStep ( step );
	
	MOAIAnim* anim = this->mAnim;
	if ( !anim ) return;
	
	anim->Apply ( t0, t1 );
}

//----------------------------------------------------------------//
void MOAIAnimDriver::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAnimDriver::RegisterLuaFuncs ( USLuaState& state ) {

	MOAITimer::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "setAnim",			_setAnim },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIAnimDriver::ToString () {

	STLString repr( MOAITimer::ToString ());

	return repr;
}
