//
//  MOAIAnimCurveCustom.cpp
//  libmoai
//
//  Created by Aaron Barrett on 5/29/13.
//
//

#include "pch.h"
#include <moaicore/MOAIAnimCurveCustom.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**  @name  setCallback
  + @text  Sets or clears the custom interpolation callback.
  +
  + @in    MOAICustomAnimCurve self
  + @opt  function callback    Default value is nil.
  + @out  nil
  + */
int MOAIAnimCurveCustom::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveCustom, "U" )

	self->mCallback.SetStrongRef ( state, 2 );

	return 0;
}

//================================================================//
// MOAIAnimCurveCustom
//================================================================//

//----------------------------------------------------------------//
MOAIAnimCurveCustom::MOAIAnimCurveCustom () {
	
	RTTI_SINGLE ( MOAIAnimCurve )
}

//----------------------------------------------------------------//
MOAIAnimCurveCustom::~MOAIAnimCurveCustom () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveCustom::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAnimCurve::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveCustom::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAnimCurve::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setCallback",    _setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}