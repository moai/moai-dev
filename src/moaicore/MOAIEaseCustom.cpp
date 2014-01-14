//
//  MOAIEaseCustom.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseCustom.h"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setFunction
 @text	Sets the lua callback function to use for this ease's interpolation.
 
 @in		MOAIEaseCustom self
 @in		function easeFunction
 @out	nil
 */
int MOAIEaseCustom::_setFunction(lua_State *L){
	MOAI_LUA_SETUP(MOAIEaseCustom, "U");
	
	self->mFunction.SetStrongRef(state, 2);
	
	return 0;
}

//================================================================//
// MOAIEaseCustom
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseCustom::DistortedTime(float inputTime){
	
	if (this->mFunction) {
		// mFunction takes a single number parameter and returns a number
		float value;
		
		MOAILuaStateHandle state = this->mFunction.GetSelf();
		lua_pushnumber(state, inputTime); // add parameter variable t to stack
		state.DebugCall(1, 1); // the function should take one argument and return one number
		
		int top = state.GetTop();
		value = state.GetValue < float >( top, 0.0f);
		
		return value;
	}
	
	
	return inputTime;
}

//----------------------------------------------------------------//
MOAIEaseCustom::MOAIEaseCustom(){
	RTTI_SINGLE(MOAIEase);
}

//----------------------------------------------------------------//
MOAIEaseCustom::~MOAIEaseCustom(){
	
}

//----------------------------------------------------------------//
void MOAIEaseCustom::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseCustom::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setFunction",				_setFunction },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
	
}