// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShader.h>

//================================================================//
// MOAIShader
//================================================================//

//----------------------------------------------------------------//
bool MOAIShader::HasProgram () const {

	return true;
}

//----------------------------------------------------------------//
bool MOAIShader::IsReady () const {

	return this->MOAIShader_IsReady ();
}

//----------------------------------------------------------------//
MOAIShader::MOAIShader () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIShader::~MOAIShader () {
}

//----------------------------------------------------------------//
void MOAIShader::SelectTextures () {

	this->MOAIShader_SelectTextures ();
}

//----------------------------------------------------------------//
void MOAIShader::UpdateUniforms () {

	this->MOAIShader_UpdateUniforms ();
}
