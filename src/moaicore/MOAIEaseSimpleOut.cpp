//
//  MOAIEaseSimpleOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseSimpleOut.h"

float MOAIEaseSimpleOut::DistortedTime(float inputTime){
	float invertedRate = 1.0f / this->mRate;
	return powf(inputTime, invertedRate);
}

MOAIEaseSimpleOut::MOAIEaseSimpleOut()
//:mRate(2.0f)
{
	RTTI_SINGLE(MOAIEaseRate)
}

MOAIEaseSimpleOut::~MOAIEaseSimpleOut(){
	
}

void MOAIEaseSimpleOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaClass(state);
}

void MOAIEaseSimpleOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaFuncs(state);
}