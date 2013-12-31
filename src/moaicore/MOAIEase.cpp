//
//  MOAIEase.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEase.h"


int MOAIEase::_setEaseType(lua_State *L){
	UNUSED(L);
	return 0;
}

float MOAIEase::DistortedTime(float inputTime){
	return inputTime;
}


MOAIEase::MOAIEase(){
	RTTI_SINGLE ( MOAILuaObject )
}

MOAIEase::~MOAIEase(){
	
}


void MOAIEase::RegisterLuaClass(MOAILuaState &state){
	UNUSED(state);
}

void MOAIEase::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "setEaseType",				_setEaseType },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}