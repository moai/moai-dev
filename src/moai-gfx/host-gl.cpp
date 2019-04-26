// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/host-gl.h>
#include <moai-gfx/headers-gl.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUGfxGLAppFinalize () {
}

//----------------------------------------------------------------//
void AKUGfxGLAppInitialize () {
}

//----------------------------------------------------------------//
void AKUGfxGLContextInitialize () {

	MOAIGfxMgrGL::Affirm ();
	MOAIGfxMgrGL::RegisterAlias < MOAIGfxMgr >();

	MOAIVertexFormatMgr::Affirm ();
	MOAIShaderMgr::Affirm ();
	
	MOAIRenderMgr::Affirm ();

	REGISTER_LUA_CLASS ( MOAIFrameBuffer )
	REGISTER_LUA_CLASS ( MOAIFrameBufferTexture )
	REGISTER_LUA_CLASS ( MOAIGfxMgrGL )
	REGISTER_LUA_CLASS ( MOAIImageTexture )
	REGISTER_LUA_CLASS ( MOAIIndexBuffer )
	REGISTER_LUA_CLASS ( MOAIRenderMgr )
	REGISTER_LUA_CLASS ( MOAIShader )
	REGISTER_LUA_CLASS ( MOAIShaderMgr )
	REGISTER_LUA_CLASS ( MOAIShaderProgram )
	REGISTER_LUA_CLASS ( MOAITexture )
	REGISTER_LUA_CLASS ( MOAIVertexArray )
	REGISTER_LUA_CLASS ( MOAIVertexBuffer )
	REGISTER_LUA_CLASS ( MOAIVertexFormat )
	REGISTER_LUA_CLASS ( MOAIVertexFormatMgr )
}
