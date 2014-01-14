//
//  MOAIEaseBackIn.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseBackIn.h"


//================================================================//
// MOAIEaseBackIn
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseBackIn::DistortedTime(float inputTime){
	float overshoot = this->mOvershoot;
	return inputTime * inputTime * ( ( overshoot + 1.0f ) * inputTime - overshoot );
}

//----------------------------------------------------------------//
MOAIEaseBackIn::MOAIEaseBackIn(){
	RTTI_SINGLE(MOAIEaseBackBase)
}

//----------------------------------------------------------------//
MOAIEaseBackIn::~MOAIEaseBackIn(){
	
}

//----------------------------------------------------------------//
void MOAIEaseBackIn::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseBackIn::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaFuncs(state);
}