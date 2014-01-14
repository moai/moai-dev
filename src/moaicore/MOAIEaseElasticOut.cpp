//
//  MOAIEaseElasticOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#include "pch.h"
#include "MOAIEaseElasticOut.h"

//================================================================//
// MOAIEaseElasticOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseElasticOut::DistortedTime(float inputTime){
	if (inputTime == 0.0f || inputTime == 1.0f){
		return inputTime;
	}
	
	float period = this->mPeriod;
	float s = period / 4.0f;
	return powf(2.0f, -10.0f * inputTime) * sinf((inputTime - s) * M_PI * 2.0f / period) + 1.0f;
}

//----------------------------------------------------------------//
MOAIEaseElasticOut::MOAIEaseElasticOut(){
	RTTI_SINGLE(MOAIEaseElasticBase)
}

//----------------------------------------------------------------//
MOAIEaseElasticOut::~MOAIEaseElasticOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseElasticOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseElasticOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseElasticBase::RegisterLuaFuncs(state);
}
