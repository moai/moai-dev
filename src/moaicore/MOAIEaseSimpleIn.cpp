//
//  MOAIEaseSimpleIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseSimpleIn.h"


float MOAIEaseSimpleIn::DistortedTime(float inputTime){
	return powf(inputTime, this->mRate);
}

MOAIEaseSimpleIn::MOAIEaseSimpleIn()
//: mRate(2.0f)
{
	RTTI_SINGLE(MOAIEaseRate)
}

MOAIEaseSimpleIn::~MOAIEaseSimpleIn(){
	
}


void MOAIEaseSimpleIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaClass(state);
}

void MOAIEaseSimpleIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseRate::RegisterLuaFuncs(state);
}