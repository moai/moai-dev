//
//  MOAIEaseBackInOut.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseBackInOut.h"

//================================================================//
// MOAIEaseBackInOut
//================================================================//

//----------------------------------------------------------------//
float MOAIEaseBackInOut::DistortedTime(float inputTime){
	float overshoot = this->mOvershoot;
	
	float fullTime = inputTime * 2.0f;
	
	if ( fullTime < 1.0f ) {
		return ( fullTime * fullTime * ( ( overshoot + 1.0f ) * fullTime - overshoot ) ) / 2.0f;
	}
	else {
		fullTime = fullTime - 2.0f;
		return ( fullTime * fullTime * ( ( overshoot + 1.0f ) * fullTime + overshoot ) ) / 2.0f + 1.0f;
	}
}

//----------------------------------------------------------------//
MOAIEaseBackInOut::MOAIEaseBackInOut(){
	RTTI_SINGLE(MOAIEaseBackBase)
}

//----------------------------------------------------------------//
MOAIEaseBackInOut::~MOAIEaseBackInOut(){
	
}

//----------------------------------------------------------------//
void MOAIEaseBackInOut::RegisterLuaClass(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseBackInOut::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEaseBackBase::RegisterLuaFuncs(state);
}
