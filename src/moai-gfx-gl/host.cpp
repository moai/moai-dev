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
void AKUGfxGLContextInitialize ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	AKUGfxContextInitialize ( contextID );

	context->Affirm < MOAIDrawGL >();
	context->Affirm < MOAIGfxMgrGL >();
	context->Affirm < MOAIShaderMgrGL >();

	context->RegisterAlias < MOAIGfxMgrGL, MOAIGfxMgr >();

	context->Get < MOAIGfxMgrGL >().SetDefaultFrameBuffer ( new MOAIFrameBufferGL ( *context ));

	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIFrameBufferGL, 		"MOAIFrameBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIGfxMgrGL, 				"MOAIGfxMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIImageTextureGL, 		"MOAIImageTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIIndexBufferGL, 		"MOAIIndexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIMeshGL, 				"MOAIMesh" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIRenderBufferGL,		"MOAIRenderBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIShaderGL, 				"MOAIShader" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIShaderMgrGL, 			"MOAIShaderMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIShaderProgramGL, 		"MOAIShaderProgram" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAITexture2DGL,			"MOAITexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIVertexBufferGL, 		"MOAIVertexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( context, MOAIVertexFormatGL, 		"MOAIVertexFormat" )
}

//----------------------------------------------------------------//
void AKUDetectFramebuffer ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;
	MOAIGfxMgrGL& gfxMgr = context->Get < MOAIGfxMgrGL >();

	context->Get < MOAIGfxMgrGL >().DetectFramebuffer ();
}

//----------------------------------------------------------------//
void AKUDetectGfxContext ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().DetectContext ();
	context->Get < MOAIShaderMgrGL >().AffirmAll ();
//	gfxMgr.RenewResources (); // TODO: ZLGfx
}

//----------------------------------------------------------------//
void AKUDiscardGfxResources ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().DiscardResources ();
}

//----------------------------------------------------------------//
void AKUDisplayListBeginPhase ( AKUContextID contextID, int phase ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().BeginPhase ( phase );
}

//----------------------------------------------------------------//
void AKUDisplayListEnable ( AKUContextID contextID, int list ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().EnableQueue ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListEndPhase ( AKUContextID contextID, int phase ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().EndPhase ( phase );
}

//----------------------------------------------------------------//
bool AKUDisplayListHasContent ( AKUContextID contextID, int list ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	return context->Get < MOAIGfxMgrGL >().HasContent ( list );
}

//----------------------------------------------------------------//
bool AKUDisplayListIsEnabled ( AKUContextID contextID, int list ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	return context->Get < MOAIGfxMgrGL >().IsQueueEnabled ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListProcess ( AKUContextID contextID, int list ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Get < MOAIGfxMgrGL >().ProcessQueue ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListPublishAndReset ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;
	MOAIGfxMgrGL& gfxMgr = context->Get < MOAIGfxMgrGL >();

	gfxMgr.PublishAndReset ( MOAIGfxMgrGL::LOADING_QUEUE );
	gfxMgr.PublishAndReset ( MOAIGfxMgrGL::DRAWING_QUEUE );
}
