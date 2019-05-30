// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/host.h>
#include <moai-gfx-gl/headers.h>

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

	MOAIVertexFormatMgrGL::Affirm ();
	MOAIShaderMgrGL::Affirm ();

	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferGL, 			"MOAIFrameBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferTextureGL, 	"MOAIFrameBufferTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIGfxMgrGL, 				"MOAIGfxMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIImageTextureGL, 		"MOAIImageTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIIndexBufferGL, 			"MOAIIndexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderGL, 				"MOAIShaderGL" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderMgrGL, 			"MOAIShaderMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderProgramGL, 		"MOAIShaderProgram" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAITextureGL, 				"MOAITexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexArrayGL, 			"MOAIVertexArray" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexBufferGL, 		"MOAIVertexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatGL, 		"MOAIVertexFormat" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatMgrGL, 		"MOAIVertexFormatMgr" )
}
