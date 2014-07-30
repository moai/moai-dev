//
//  MOAICCParticleSystem.cpp
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#include "pch.h"
#include <moaicore/MOAICCParticleSystem.h>


MOAICCParticleSystem::MOAICCParticleSystem() {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIProp)
	RTTI_END
}

MOAICCParticleSystem::~MOAICCParticleSystem	(){
	
}

void MOAICCParticleSystem::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIProp::RegisterLuaClass ( state );
}

void MOAICCParticleSystem::RegisterLuaFuncs( MOAILuaState &state ) {
	MOAIProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{NULL, NULL}
	};
	
	luaL_register ( state, 0, regTable );
}