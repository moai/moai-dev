//
//  MOAIEaseExponentialIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseExponentialIn.h"

//================================================================//
// MOAIEaseExponentialIn
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseExponentialIn::DistortedTime(float inputTime){
	if (inputTime == 0.0){
		return 0.0;
	}
	return powf(2.0f, 10.0f * (inputTime - 1.0f)) - 0.001f;
}

//----------------------------------------------------------------//
MOAIEaseExponentialIn::MOAIEaseExponentialIn(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseExponentialIn::~MOAIEaseExponentialIn(){
	
}

//----------------------------------------------------------------//
void MOAIEaseExponentialIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseExponentialIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}