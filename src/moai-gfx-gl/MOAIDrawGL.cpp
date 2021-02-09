// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>

//================================================================//
// MOAIDrawGL
//================================================================//

//----------------------------------------------------------------//
MOAIDrawGL::MOAIDrawGL () {

	RTTI_BEGIN ( MOAIDrawGL )
		RTTI_EXTEND ( MOAIAbstractCmdHandler )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDrawGL::~MOAIDrawGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawGL::MOAIAbstractCmdHandler_HandleCommand ( u32 cmd, const void* param ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	switch ( cmd ) {
	
		case MOAIDrawAPIGL::LOAD_SHADER_UNIFORM_GL: {
			assert ( false );
			break;
		}
	}
}
