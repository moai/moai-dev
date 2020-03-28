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
bool MOAIShader::IsReadyForUse () const {

	return this->MOAIShader_IsReadyForUse ();
}

//----------------------------------------------------------------//
MOAIShader::MOAIShader () {
	
	RTTI_BEGIN ( MOAIShader )
		RTTI_EXTEND ( MOAIHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShader::~MOAIShader () {
}
