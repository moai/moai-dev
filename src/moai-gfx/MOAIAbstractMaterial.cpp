// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIAbstractMaterial.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIMaterialGlobals
//================================================================//

//----------------------------------------------------------------//
u32 MOAIMaterialGlobals::GetNamedGlobalID ( MOAILuaState& state, int idx ) {

	return state.GetValue < u32 >( idx++, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1;
}

//================================================================//
// MOAIAbstractMaterial
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractMaterial::MOAIAbstractMaterial () :
	mOverwrite ( true ) {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIAbstractMaterial )
		RTTI_EXTEND ( MOAIAbstractMaterialInterface )
	RTTI_END
	
	this->Clear ( ALL_FLAGS, true );
}

//----------------------------------------------------------------//
MOAIAbstractMaterial::~MOAIAbstractMaterial () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractMaterial& MOAIAbstractMaterial::MOAIAbstractMaterialInterface_AffirmMaterial () {
	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractMaterial* MOAIAbstractMaterial::MOAIAbstractMaterialInterface_GetMaterial () {
	return this;
}
