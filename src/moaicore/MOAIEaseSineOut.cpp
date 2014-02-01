//
//  MOAIEaseSineOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseSineOut.h"

//================================================================//
// MOAIEaseSineOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseSineOut::DistortedTime(float inputTime){
	return sinf(inputTime * (float)M_PI_2);
}

//----------------------------------------------------------------//
MOAIEaseSineOut::MOAIEaseSineOut(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseSineOut::~MOAIEaseSineOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseSineOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseSineOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}