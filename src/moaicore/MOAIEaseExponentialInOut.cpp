//
//  MOAIEaseExponentialInOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseExponentialInOut.h"

//================================================================//
// MOAIEaseExponentialInOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseExponentialInOut::DistortedTime(float inputTime){
	float doubleTime = inputTime * 2.0f;
	if (doubleTime < 1.0f){
		return 0.5f * powf(2.0f, 10.0f * (doubleTime - 1.0f));
	}
	return 0.5f * (-powf(2.0f, -10.0f * (doubleTime - 1.0f)) + 2.0f);
}

//----------------------------------------------------------------//
MOAIEaseExponentialInOut::MOAIEaseExponentialInOut(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseExponentialInOut::~MOAIEaseExponentialInOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseExponentialInOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseExponentialInOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}
