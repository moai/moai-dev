// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <aku/AKU.h>
#include <aku/AKU-particles.h>

#include <cassert>
#include <moaicore/moaicore.h>

//================================================================//
// AKU-particles
//================================================================//

//----------------------------------------------------------------//
void AKUNewParticlePlugin ( lua_State* L, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size ) {
	UNUSED ( L );
	UNUSED ( initFunc );
	UNUSED ( renderFunc );
	UNUSED ( size );
	
	MOAILuaState state ( L );
	
	MOAIParticleCallbackPlugin* plugin = new MOAIParticleCallbackPlugin ();
	plugin->Init ( initFunc, renderFunc, size );
	plugin->PushLuaUserdata ( state );
}

//----------------------------------------------------------------//
void AKUSetParticlePreset ( const char* presetTable, const char* presetName, AKUParticleInitFunc initFunc, AKUParticleRenderFunc renderFunc, int size ) {

	lua_State* L = AKUGetLuaState ();
	
	lua_getglobal ( L, presetTable );
	
	if ( lua_isnil ( L, -1 )) {
		lua_newtable ( L );
		lua_setglobal ( L, presetTable );
	}
	
	lua_getglobal ( L, presetTable );
	assert ( lua_isnil ( L, -1 ) == false );
	
	AKUNewParticlePlugin ( L, initFunc, renderFunc, size );
	lua_setfield ( L, -2, presetName );
	
	lua_pop ( L, 1 );
}
