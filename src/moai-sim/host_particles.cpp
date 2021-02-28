// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/headers.h>
#include <moai-sim/host_particles.h>

//================================================================//
// aku-particles
//================================================================//

//----------------------------------------------------------------//
void AKUNewParticlePlugin ( AKUContextID contextID, lua_State* L, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size ) {
	UNUSED ( L );
	UNUSED ( initFunc );
	UNUSED ( renderFunc );
	UNUSED ( size );
	
	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;
	
	MOAILuaState state ( L );
	
	MOAIParticleCallbackPlugin* plugin = new MOAIParticleCallbackPlugin ( *context );
	plugin->Init ( initFunc, renderFunc, size );
	plugin->PushLuaUserdata ( state );
}

//----------------------------------------------------------------//
void AKUSetParticlePreset ( AKUContextID contextID, const char* presetTable, const char* presetName, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	lua_State* L = AKUGetLuaState ( contextID );
	
	lua_getglobal ( L, presetTable );
	
	if ( lua_isnil ( L, -1 )) { // TODO: nil or none?
		lua_newtable ( L );
		lua_setglobal ( L, presetTable );
	}
	
	lua_getglobal ( L, presetTable );
	assert ( lua_isnil ( L, -1 ) == false );
	
	AKUNewParticlePlugin ( contextID, L, initFunc, renderFunc, size );
	lua_setfield ( L, -2, presetName );
	
	lua_pop ( L, 1 );
}
