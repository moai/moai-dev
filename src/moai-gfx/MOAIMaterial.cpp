// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::Clear () {

	this->ClearNamedGlobalList < MOAILight >( this->mLights );
	this->ClearNamedGlobalList < ZLAbstractTexture >( this->mTextures );
	
	this->MOAIMaterialBase::Clear ();
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal < MOAILight >* global = this->FindNamedGlobal < MOAILight >( this->mLights, name );
	return global ? ( MOAILight* )global->mValue : 0;
}

//----------------------------------------------------------------//
ZLAbstractTexture* MOAIMaterial::GetTexture () {

	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
ZLAbstractTexture* MOAIMaterial::GetTexture ( u32 name ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < ZLAbstractTexture >* global = this->FindNamedGlobal < ZLAbstractTexture >( this->mTextures, name );
		return global ? ( ZLAbstractTexture* )global->mValue : 0;
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

	this->Clear ();
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
void MOAIMaterial::SetTexture ( ZLAbstractTexture* texture ) {

	this->MOAIMaterialBase::SetTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name ) {

	this->SetTexture ( name, 0 );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name, ZLAbstractTexture* texture ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		this->SetNamedGlobal < ZLAbstractTexture >( this->mTextures, name, texture );
	}
}
