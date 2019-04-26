// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgrGL.h>

//================================================================//
// MOAIGfxMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrGL::MOAIGfxMgrGL () {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxMgr )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxMgrGL::~MOAIGfxMgrGL () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL::OnGlobalsInitialize () {

	this->InitBuffers ();
}
