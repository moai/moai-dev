// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialMgr.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::Clear () {

	this->ClearNamedGlobalList < MOAILight >( this->mLights );
	this->ClearNamedGlobalList < MOAITexture >( this->mTextures );
	
	this->MOAIMaterialBase::Clear ();
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal < MOAILight >* global = this->FindNamedGlobal < MOAILight >( this->mLights, name );
	return global ? ( MOAILight* )global->mValue : 0;
}

//----------------------------------------------------------------//
u32 MOAIMaterial::GetNamedGlobalID ( MOAILuaState& state, int idx ) {

	return state.GetValue < u32 >( idx++, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1;
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterial::GetTexture () {

	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterial::GetTexture ( u32 name ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITexture >* global = this->FindNamedGlobal < MOAITexture >( this->mTextures, name );
		return global ? ( MOAITexture* )global->mValue : 0;
	}
	return this->MOAIMaterialBase::GetTexture ();
}

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mLights ( 0 ),
	mTextures ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialInterface )
	RTTI_END
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
void MOAIMaterial::SetTexture ( MOAITexture* texture ) {

	this->MOAIMaterialBase::SetTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name ) {

	this->SetTexture ( name, 0 );
}

//----------------------------------------------------------------//
void MOAIMaterial::SetTexture ( u32 name, MOAITexture* texture ) {

	if ( name < MOAIMaterialMgr::MAX_GLOBAL_TEXTURES ) {
		this->SetNamedGlobal < MOAITexture >( this->mTextures, name, texture );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialInterface::MOAILuaObject_RegisterLuaClass ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterial::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialInterface::MOAILuaObject_RegisterLuaFuncs ( composer, state );
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterial::MOAIMaterialInterface_AffirmMaterial () {

	return *this;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterial::MOAIMaterialInterface_GetMaterial () {

	return this;
}
