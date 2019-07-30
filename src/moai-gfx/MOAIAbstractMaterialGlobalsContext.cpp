// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIAbstractMaterialGlobalsContext.h>

//================================================================//
// MOAIMaterialGlobals
//================================================================//

//----------------------------------------------------------------//
u32 MOAIMaterialGlobals::GetNamedGlobalID ( MOAILuaState& state, int idx ) {

	return state.GetValue < u32 >( idx++, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1;
}

//================================================================//
// MOAIAbstractMaterialGlobalsContext
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::Apply ( MOAIAbstractMaterialGlobalsContext& dest ) {

	this->MOAIAbstractMaterialGlobalsContext_Apply ( dest );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::Clear () {

	this->MOAIAbstractMaterialGlobalsContext_Clear ();
}

//----------------------------------------------------------------//
MOAILight* MOAIAbstractMaterialGlobalsContext::GetLight ( u32 name ) {

	return this->MOAIAbstractMaterialGlobalsContext_GetLight ( name );
}

//----------------------------------------------------------------//
MOAITexture* MOAIAbstractMaterialGlobalsContext::GetTexture ( u32 name ) {

	return this->MOAIAbstractMaterialGlobalsContext_GetTexture ( name );
}

//----------------------------------------------------------------//
MOAIAbstractMaterialGlobalsContext::MOAIAbstractMaterialGlobalsContext () {
}

//----------------------------------------------------------------//
MOAIAbstractMaterialGlobalsContext::~MOAIAbstractMaterialGlobalsContext () {
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::SetLight ( u32 name ) {

	this->SetLight ( name, NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::SetLight ( u32 name, MOAILight* light ) {

	this->MOAIAbstractMaterialGlobalsContext_SetLight ( name, light );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::SetTexture ( u32 name ) {

	this->SetTexture ( name, NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialGlobalsContext::SetTexture ( u32 name, MOAITexture* texture ) {

	this->MOAIAbstractMaterialGlobalsContext_SetTexture ( name, texture );
}
