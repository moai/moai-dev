// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial::MOAIMaterial () :
	mLights ( NULL ),
	mTextures ( NULL ) {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIMaterial )
		RTTI_EXTEND ( MOAIAbstractMaterial )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterial::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIAbstractMaterialInterface::MOAILuaObject_RegisterLuaClass ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterial::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIAbstractMaterialInterface::MOAILuaObject_RegisterLuaFuncs ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterial::MOAIAbstractMaterial_ApplyGlobals ( MOAIAbstractMaterialInterface& dest ) {

	MOAIMaterialNamedGlobal < MOAILight >* namedLightIt = this->mLights;
	for ( ; namedLightIt; namedLightIt = namedLightIt->mNext ) {
		dest.SetLight ( namedLightIt->mName, namedLightIt->mValue );
	}
	
	MOAIMaterialNamedGlobal < MOAITexture >* namedTexturelIt = this->mTextures;
	for ( ; namedTexturelIt; namedTexturelIt = namedTexturelIt->mNext ) {
		dest.SetTexture ( namedTexturelIt->mName, namedTexturelIt->mValue );
	}
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::MOAIAbstractMaterial_ClearGlobals () {

	MOAIMaterialNamedGlobal < MOAILight >::Clear ( this->mLights );
	MOAIMaterialNamedGlobal < MOAITexture >::Clear ( this->mTextures );
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterial::MOAIAbstractMaterial_GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal < MOAILight >* global = MOAIMaterialNamedGlobal < MOAILight >::FindNamedGlobal ( this->mLights, name );
	return global ? ( MOAILight* )global->mValue : 0;
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterial::MOAIAbstractMaterial_GetTexture ( u32 name ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITexture >* global = MOAIMaterialNamedGlobal < MOAITexture >::FindNamedGlobal ( this->mTextures, name );
		return global ? ( MOAITexture* )global->mValue : 0;
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::MOAIAbstractMaterial_SetLight ( u32 name, MOAILight* light ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS ) {
		MOAIMaterialNamedGlobal < MOAILight >::SetNamedGlobal ( this->mLights, name, light );
	}
}

//----------------------------------------------------------------//
void MOAIMaterial::MOAIAbstractMaterial_SetTexture ( u32 name, MOAITexture* texture ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITexture >::SetNamedGlobal ( this->mTextures, name, texture );
	}
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterial::MOAIAbstractMaterialInterface_AffirmMaterial () {
	return *this;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterial::MOAIAbstractMaterialInterface_GetMaterial () {
	return this;
}
