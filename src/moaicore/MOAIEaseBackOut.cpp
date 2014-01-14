//
//  MOAIEaseBackOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseBackOut.h"

//================================================================//
// MOAIEaseBackOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseBackOut::DistortedTime(float inputTime){
	float overshoot = this->mOvershoot;
	float invertedTime = inputTime - 1.0f;
	return invertedTime * invertedTime * ( ( overshoot + 1.0f ) * invertedTime + overshoot ) + 1.0f;
}

//----------------------------------------------------------------//
MOAIEaseBackOut::MOAIEaseBackOut(){
	RTTI_SINGLE(MOAIEaseBackBase)
}

//----------------------------------------------------------------//
MOAIEaseBackOut::~MOAIEaseBackOut(){
}

//----------------------------------------------------------------//
void MOAIEaseBackOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseBackOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaFuncs(state);
}
