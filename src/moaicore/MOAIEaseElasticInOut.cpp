//
//  MOAIEaseElasticInOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseElasticInOut.h"


//================================================================//
// MOAIEaseElasticInOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseElasticInOut::DistortedTime(float inputTime){
	if (inputTime == 0.0f || inputTime == 1.0f){
		return inputTime;
	}
	
	float adjustedTime = (inputTime * 2.0f) - 1.0f;
	
	float period = this->mPeriod;
	float s = period / 4.0f;
	
	if (adjustedTime < 0.0f){
		return -0.5f * powf(2.0f, 10.0f * adjustedTime) * sinf((adjustedTime - s) * M_PI * 2.0f / period);
	}
	
	return powf(2.0f, -10.0 * adjustedTime) * sinf((adjustedTime - s) * M_PI * 2.0f / period) * 0.5f + 1.0f;
}

//----------------------------------------------------------------//
MOAIEaseElasticInOut::MOAIEaseElasticInOut(){
	RTTI_SINGLE(MOAIEaseElasticBase)
}

//----------------------------------------------------------------//
MOAIEaseElasticInOut::~MOAIEaseElasticInOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseElasticInOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseElasticInOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaFuncs(state);
}
