//
//  MOAIEaseBackBase.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseBackBase.h"

int MOAIEaseBackBase::_setOvershoot( lua_State *L ){
	MOAI_LUA_SETUP( MOAIEaseBackBase, "UN" );
	
	float overshoot = state.GetValue < float >( 2, 0 );
	self->SetOvershoot(overshoot);
	
	return 0;
}

MOAIEaseBackBase::MOAIEaseBackBase()
:mOvershoot(1.70158)
{
	RTTI_SINGLE(MOAIEase)
}

MOAIEaseBackBase::~MOAIEaseBackBase(){
	
}

void MOAIEaseBackBase::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

void MOAIEaseBackBase::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setOvershoot",				_setOvershoot },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIEaseBackBase::SetOvershoot(float overshoot){
	this->mOvershoot = overshoot;
}