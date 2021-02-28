// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>

//================================================================//
// MOAIDrawGL
//================================================================//

//----------------------------------------------------------------//
MOAIDrawGL::MOAIDrawGL ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIAbstractCmdHandler ( context ),
	MOAIAbstractCmdHandlerWithAPI < MOAIDrawAPIGL >( context ),
	ZLContextClass ( context ) {

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

	MOAIGfxMgr& gfxMgr = this->Get < MOAIGfxMgr >();

	switch ( cmd ) {
	
		case MOAIDrawAPIGL::LOAD_SHADER_UNIFORM_GL: {
			assert ( false );
			break;
		}
	}
}
