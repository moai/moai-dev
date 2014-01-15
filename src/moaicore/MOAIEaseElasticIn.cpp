//
//  MOAIEaseElasticIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseElasticIn.h"

//================================================================//
// MOAIEaseElasticIn
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseElasticIn::DistortedTime(float inputTime){
	float period = this->mPeriod;
	float s = period / 4.0f;
	float adjustedTime = inputTime - 1.0f;
	return -powf(2.0f, 10.0f * adjustedTime) * sinf((adjustedTime - s) * M_PI * 2.0f / period);
}

//----------------------------------------------------------------//
MOAIEaseElasticIn::MOAIEaseElasticIn(){
	RTTI_SINGLE(MOAIEaseElasticBase)
}

//----------------------------------------------------------------//
MOAIEaseElasticIn::~MOAIEaseElasticIn(){
	
}

//----------------------------------------------------------------//
void MOAIEaseElasticIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseElasticIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaFuncs(state);
}
