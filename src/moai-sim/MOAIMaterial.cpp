// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAIMaterial.h>
#include <moai-sim/MOAIMaterialMgr.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialNamedGlobal& MOAIMaterial::AffirmNamedGlobal ( ZLLeanArray < MOAIMaterialNamedGlobal >& array, u32 name ) {
	
	MOAIMaterialNamedGlobal* global = this->FindNamedGlobal ( array, name );
	
	if ( !global ) {
	
		size_t idx = array.Size ();
		array.Grow ( name, 1 );
		MOAIMaterialNamedGlobal& global = array [ idx ];
		
		global.mName = name;
		global.mPtr = 0;
	}
	return *global;
}

//----------------------------------------------------------------//
MOAIMaterialNamedGlobal* MOAIMaterial::FindNamedGlobal ( ZLLeanArray < MOAIMaterialNamedGlobal >& array, u32 name ) {
	
	for ( u32 i = 0; i < array.Size (); ++i ) {
		MOAIMaterialNamedGlobal& global = array [ i ];
		if ( global.mName == name ) {
			return &global;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal* global = this->FindNamedGlobal ( this->mLights, name );
	return global ? global->mLight : 0;
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterial::GetTexture () {

	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterial::GetTexture ( u32 name ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal* global = this->FindNamedGlobal ( this->mTextures, name );
		return global ? global->mTexture : 0;
	}
	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {

	this->ReserveLights ( 0 );
	this->ReserveTextures ( 0 );
}

//----------------------------------------------------------------//
void MOAIMaterial::ReserveLights ( u32 n ) {

	n = n < MOAIMaterialMgr::MAX_GLOBAL_LIGHTS ? n : MOAIMaterialMgr::MAX_GLOBAL_LIGHTS;

	for ( u32 i = 0; i < this->mLights.Size (); ++i ) {
		ZLRelease ( this->mLights [ i ].mLight );
	}
	this->mLights.Init ( n );
}

//----------------------------------------------------------------//
void MOAIMaterial::ReserveTextures ( u32 n ) {

	n = n < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ? n : MOAIMaterialMgr::MAX_GLOBAL_TEXTURES;

	for ( u32 i = 0; i < this->mTextures.Size (); ++i ) {
		ZLRelease ( this->mTextures [ i ].mTexture );
	}
	this->mTextures.Init ( n );
}


//----------------------------------------------------------------//
void MOAIMaterial::SetLight ( u32 name, MOAILight* light ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_LIGHTS ) {

		ZLRetain ( light );
		MOAIMaterialNamedGlobal& global = this->AffirmNamedGlobal ( this->mLights, name );
		ZLRelease ( global.mLight );
		global.mLight = light;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( MOAITextureBase* texture ) {

	this->MOAIMaterialBase::SetTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name, MOAITextureBase* texture ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
	
		ZLRetain ( texture );
		MOAIMaterialNamedGlobal& global = this->AffirmNamedGlobal ( this->mTextures, name );
		ZLRelease ( global.mTexture );
		global.mTexture = texture;
	}
	else {
		this->MOAIMaterialBase::SetTexture ( texture );
	}
}
