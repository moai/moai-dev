//
//  MOAIEaseSimpleInOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseSimpleInOut.h"

float MOAIEaseSimpleInOut::DistortedTime(float inputTime){
	float t = inputTime * 2.0f;
	
	if (t < 1.0f){
		return 0.5 * powf(t, this->mRate);
	}
	
	return 1.0 - 0.5 * powf(2.0 - t, this->mRate);
}

MOAIEaseSimpleInOut::MOAIEaseSimpleInOut()
	//:mRate(2.0f)
{
	
}

MOAIEaseSimpleInOut::~MOAIEaseSimpleInOut(){
	
}

void MOAIEaseSimpleInOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaClass(state);
}

void MOAIEaseSimpleInOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaFuncs(state);
}