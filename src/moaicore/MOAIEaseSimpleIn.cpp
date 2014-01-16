//
//  MOAIEaseSimpleIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseSimpleIn.h"

//================================================================//
// MOAIEaseSimpleIn
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseSimpleIn::DistortedTime(float inputTime){
	return powf(inputTime, this->mRate);
}

//----------------------------------------------------------------//
MOAIEaseSimpleIn::MOAIEaseSimpleIn(){
	RTTI_SINGLE(MOAIEaseSimpleBase)
}

//----------------------------------------------------------------//
MOAIEaseSimpleIn::~MOAIEaseSimpleIn(){
}

//----------------------------------------------------------------//
void MOAIEaseSimpleIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseSimpleBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseSimpleIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseSimpleBase::RegisterLuaFuncs(state);
}