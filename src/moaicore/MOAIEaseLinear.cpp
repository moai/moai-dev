//
//  MOAIEaseLinear.cpp
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#include "pch.h"
#include "MOAIEaseLinear.h"

//================================================================//
// MOAIEaseLinear
//================================================================//

//----------------------------------------------------------------//
MOAIEaseLinear::MOAIEaseLinear(){
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseLinear::~MOAIEaseLinear(){
	
}

//----------------------------------------------------------------//
void MOAIEaseLinear::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseLinear::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
}