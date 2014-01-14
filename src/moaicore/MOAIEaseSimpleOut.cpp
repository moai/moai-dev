//
//  MOAIEaseSimpleOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseSimpleOut.h"

//================================================================//
// MOAIEaseSimpleOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseSimpleOut::DistortedTime(float inputTime){
	float invertedRate = 1.0f / this->mRate;
	return powf(inputTime, invertedRate);
}

//----------------------------------------------------------------//
MOAIEaseSimpleOut::MOAIEaseSimpleOut(){
	RTTI_SINGLE(MOAIEaseSimpleBase)
}

//----------------------------------------------------------------//
MOAIEaseSimpleOut::~MOAIEaseSimpleOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseSimpleOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseSimpleBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseSimpleOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseSimpleBase::RegisterLuaFuncs(state);
}