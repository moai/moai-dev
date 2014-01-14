//
//  MOAIEaseExponentialOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseExponentialOut.h"

//================================================================//
// MOAIEaseExponentialOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseExponentialOut::DistortedTime(float inputTime){
	if (inputTime == 1.0f){
		return 1.0f;
	}
	return -powf(2.0f, -10.0f * inputTime) + 1.0f;
}

//----------------------------------------------------------------//
MOAIEaseExponentialOut::MOAIEaseExponentialOut(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseExponentialOut::~MOAIEaseExponentialOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseExponentialOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseExponentialOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}
