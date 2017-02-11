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
MOAILight* MOAIMaterial::GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal < MOAILight >* global = this->FindNamedGlobal < MOAILight >( this->mLights, name );
	return global ? ( MOAILight* )global->mValue : 0;
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterial::GetTexture () {

	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterial::GetTexture ( u32 name ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITextureBase >* global = this->FindNamedGlobal < MOAITextureBase >( this->mTextures, name );
		return global ? ( MOAITextureBase* )global->mValue : 0;
	}
	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mLights ( 0 ),
	mTextures ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {

	this->ClearNamedGlobalList < MOAILight >( this->mLights );
	this->ClearNamedGlobalList < MOAITextureBase >( this->mTextures );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetLight ( u32 name ) {

	this->SetLight ( name, 0 );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetLight ( u32 name, MOAILight* light ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_LIGHTS ) {
		this->SetNamedGlobal < MOAILight >( this->mLights, name, light );
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture () {

	this->MOAIMaterialBase::SetTexture ();
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( MOAITextureBase* texture ) {

	this->MOAIMaterialBase::SetTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name ) {

	this->SetTexture ( name, 0 );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name, MOAITextureBase* texture ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		this->SetNamedGlobal < MOAITextureBase >( this->mTextures, name, texture );
	}
}
