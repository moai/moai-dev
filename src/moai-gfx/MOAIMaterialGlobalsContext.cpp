// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterialGlobalsContext.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIMaterial
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialGlobalsContext::MOAIMaterialGlobalsContext () :
	mLights ( NULL ),
	mTextures ( NULL ) {
}

//----------------------------------------------------------------//
MOAIMaterialGlobalsContext::~MOAIMaterialGlobalsContext () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_Apply ( MOAIAbstractMaterialGlobalsContext& dest ) {

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
MOAILight* MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_Clear () {

	MOAIMaterialNamedGlobal < MOAILight >::Clear ( this->mLights );
	MOAIMaterialNamedGlobal < MOAITexture >::Clear ( this->mTextures );
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_GetLight ( u32 name ) {

	MOAIMaterialNamedGlobal < MOAILight >* global = MOAIMaterialNamedGlobal < MOAILight >::FindNamedGlobal ( this->mLights, name );
	return global ? ( MOAILight* )global->mValue : 0;
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_GetTexture ( u32 name ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITexture >* global = MOAIMaterialNamedGlobal < MOAITexture >::FindNamedGlobal ( this->mTextures, name );
		return global ? ( MOAITexture* )global->mValue : 0;
	}
}

//----------------------------------------------------------------//
void MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_SetLight ( u32 name, MOAILight* light ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS ) {
		MOAIMaterialNamedGlobal < MOAILight >::SetNamedGlobal ( this->mLights, name, light );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext_SetTexture ( u32 name, MOAITexture* texture ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES ) {
		MOAIMaterialNamedGlobal < MOAITexture >::SetNamedGlobal ( this->mTextures, name, texture );
	}
}
