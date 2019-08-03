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
	mOverwrite ( true ),
	mGlobals ( NULL ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractMaterialInterface )
	RTTI_END
	
	this->Clear ( ALL_FLAGS, true );
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {

	this->Clear ();
	if ( this->mGlobals ) {
		delete this->mGlobals;
	}
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
MOAIMaterial& MOAIMaterial::MOAIAbstractMaterialInterface_AffirmMaterial () {
	return *this;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterial::MOAIAbstractMaterialInterface_GetMaterial () {
	return this;
}
