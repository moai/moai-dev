// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/host.h>
#include <moai-gfx-gl/host.h>
#include <moai-gfx-gl/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUGfxGLAppFinalize () {

	AKUGfxAppFinalize ();
}

//----------------------------------------------------------------//
void AKUGfxGLAppInitialize () {

	AKUGfxAppInitialize ();
}

//----------------------------------------------------------------//
void AKUGfxGLContextInitialize () {

	AKUGfxContextInitialize ();

	MOAIDrawGL::Affirm ();
	MOAIGfxMgrGL::Affirm ();
	MOAIGfxMgrGL::RegisterAlias < MOAIGfxMgr >();
	MOAIShaderMgrGL::Affirm ();

	MOAIGfxMgrGL::Get ().SetDefaultFrameBuffer ( new MOAIFrameBufferGL ());

	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferGL, 			"MOAIFrameBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIGfxMgrGL, 				"MOAIGfxMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIImageTextureGL, 		"MOAIImageTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIIndexBufferGL, 			"MOAIIndexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIMeshGL, 				"MOAIMesh" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIRenderBufferGL,			"MOAIRenderBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderGL, 				"MOAIShader" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderMgrGL, 			"MOAIShaderMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderProgramGL, 		"MOAIShaderProgram" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAITexture2DGL,			"MOAITexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexBufferGL, 		"MOAIVertexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatGL, 		"MOAIVertexFormat" )
}

//----------------------------------------------------------------//
void AKUDetectFramebuffer () {

	MOAIGfxMgrGL::Get ().DetectFramebuffer ();
}

//----------------------------------------------------------------//
void AKUDetectGfxContext () {

	MOAIGfxMgrGL::Get ().DetectContext ();
	MOAIShaderMgrGL::Get ().AffirmAll ();
//	MOAIGfxMgrGL::Get ().RenewResources (); // TODO: ZLGfx
}

//----------------------------------------------------------------//
void AKUDiscardGfxResources () {

	MOAIGfxMgrGL::Get ().DiscardResources ();
}

//----------------------------------------------------------------//
void AKUDisplayListBeginPhase ( int phase ) {

	MOAIGfxMgrGL::Get ().BeginPhase ( phase );
}

//----------------------------------------------------------------//
void AKUDisplayListEnable ( int list ) {

	MOAIGfxMgrGL::Get ().EnableQueue ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListEndPhase ( int phase ) {

	MOAIGfxMgrGL::Get ().EndPhase ( phase );
}

//----------------------------------------------------------------//
bool AKUDisplayListHasContent ( int list ) {

	return MOAIGfxMgrGL::Get ().HasContent ( list );
}

//----------------------------------------------------------------//
bool AKUDisplayListIsEnabled ( int list ) {

	return MOAIGfxMgrGL::Get ().IsQueueEnabled ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListProcess ( int list ) {

	MOAIGfxMgrGL::Get ().ProcessQueue ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListPublishAndReset () {

	MOAIGfxMgrGL::Get ().PublishAndReset ( MOAIGfxMgrGL::LOADING_QUEUE );
	MOAIGfxMgrGL::Get ().PublishAndReset ( MOAIGfxMgrGL::DRAWING_QUEUE );
}
