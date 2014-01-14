//
//  MOAIEaseSineIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseSineIn.h"

//================================================================//
// MOAIEaseSineIn
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseSineIn::DistortedTime(float inputTime){
	return -1.0f * cosf(inputTime * (float)M_PI_2) + 1.0f;
}

//----------------------------------------------------------------//
MOAIEaseSineIn::MOAIEaseSineIn(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseSineIn::~MOAIEaseSineIn(){
	
}

//----------------------------------------------------------------//
void MOAIEaseSineIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseSineIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}