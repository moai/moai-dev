//
//  MOAIEaseRate.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseRate.h"

int MOAIEaseRate::_setRate( lua_State *L ){
	MOAI_LUA_SETUP( MOAIEaseRate, "UN" );
	
	float rate = state.GetValue < float >( 2, 0 );
	self->SetRate(rate);
	
	return 0;
}

MOAIEaseRate::MOAIEaseRate()
	:mRate(2.0f)
{
	RTTI_SINGLE(MOAIEase)
}

MOAIEaseRate::~MOAIEaseRate(){
	
}

void MOAIEaseRate::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

void MOAIEaseRate::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setRate",				_setRate },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIEaseRate::SetRate(float inputRate){
	this->mRate = inputRate;
}