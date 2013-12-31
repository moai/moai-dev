//
//  MOAIEase.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEase.h"

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
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}