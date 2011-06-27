// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
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
	
	USLuaState state ( L );
	
	MOAIParticlePlugin* plugin = new MOAIParticlePlugin ();
	plugin->Init ( initFunc, renderFunc, size );
	plugin->PushLuaUserdata ( state );
}
