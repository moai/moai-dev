//
//  MOAIEaseSineInOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseSineInOut.h"

//================================================================//
// MOAIEaseSineInOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseSineInOut::DistortedTime(float inputTime){
	return -0.5f * (cosf((float)M_PI * inputTime) - 1.0f);
}

//----------------------------------------------------------------//
MOAIEaseSineInOut::MOAIEaseSineInOut(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseSineInOut::~MOAIEaseSineInOut(){
}

//----------------------------------------------------------------//
void MOAIEaseSineInOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseSineInOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}