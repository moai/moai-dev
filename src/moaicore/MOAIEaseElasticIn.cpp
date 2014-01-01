//
//  MOAIEaseElasticIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseElasticIn.h"


float MOAIEaseElasticIn::DistortedTime(float inputTime){
	float period = this->mPeriod;
	float s = period * 0.25f;
	
	if (inputTime == 0.0f || inputTime == 1.0f) {
		return inputTime;
	}
	
	float invertedTime = inputTime - 1.0f;
	return -powf(2.0f, 10.0f * invertedTime) * sinf(invertedTime - s) * M_PI * 2.0f / period;
}

MOAIEaseElasticIn::MOAIEaseElasticIn()
{
	RTTI_SINGLE(MOAIEaseElasticBase)
}

MOAIEaseElasticIn::~MOAIEaseElasticIn(){
	
}


void MOAIEaseElasticIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaClass(state);
}

void MOAIEaseElasticIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaFuncs(state);
}
