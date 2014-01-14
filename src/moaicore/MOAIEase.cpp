//
//  MOAIEase.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEase.h"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getValueAtTime
 @text	Return the interpolated value given a point in time along the ease.
 
 @in		MOAIEase self
 @in		number time
 @out	number interpolated value
 */
int MOAIEase::_getValueAtTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEase, "UN" );
	
	float time = state.GetValue < float >( 2, 0 );
	float value = self->DistortedTime ( time );
	state.Push ( value );
	return 1;
}

//================================================================//
// MOAIEase
//================================================================//

//----------------------------------------------------------------//
float MOAIEase::DistortedTime(float inputTime){
	return inputTime;
}

//----------------------------------------------------------------//
MOAIEase::MOAIEase(){
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIEase::~MOAIEase(){
	
}

//----------------------------------------------------------------//
void MOAIEase::RegisterLuaClass(MOAILuaState &state){
	UNUSED(state);
}

//----------------------------------------------------------------//
void MOAIEase::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "getValueAtTime",				_getValueAtTime },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
