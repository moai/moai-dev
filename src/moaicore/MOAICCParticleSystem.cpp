//
//  MOAICCParticleSystem.cpp
//  libmoai
//
//  Created by Isaac Barrett on 7/30/14.
//
//

#include "pch.h"
#include <moaicore/MOAICCParticleSystem.h>

int MOAICCParticleSystem::_initializeProperties( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	
	
	return 0;
}

int MOAICCParticleSystem::_load( lua_State *L ) {
	MOAI_LUA_SETUP( MOAICCParticleSystem, "US" )
	
	return 0;
}

int MOAICCParticleSystem::_start(lua_State *L){
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->mActive = true;
	return 0;
}

int MOAICCParticleSystem::_stop(lua_State *L){
	
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	self->mActive = false;
	
	return 0;
}

int MOAICCParticleSystem::_reset(lua_State *L){
	MOAI_LUA_SETUP( MOAICCParticleSystem, "U" )
	
	return 0;
}

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