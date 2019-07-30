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
	mOverwrite ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialInterface )
	RTTI_END
	
	this->Clear ( ALL_FLAGS, true );
}

//----------------------------------------------------------------//
MOAIMaterial::~MOAIMaterial () {

	this->Clear ();
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
