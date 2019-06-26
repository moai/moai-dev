// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/host.h>
#include <moai-gfx-vk/host.h>
#include <moai-gfx-vk/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUGfxVKAppFinalize () {

	AKUGfxAppFinalize ();
}

//----------------------------------------------------------------//
void AKUGfxVKAppInitialize () {

	AKUGfxAppInitialize ();
}

//----------------------------------------------------------------//
void AKUGfxVKContextInitialize () {

	AKUGfxContextInitialize ();

	MOAIGfxMgrVK::Affirm ();
	MOAIGfxMgrVK::RegisterAlias < MOAIGfxMgr >();

	MOAIVertexFormatMgrVK::Affirm ();
	MOAIShaderMgrVK::Affirm ();

	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferVK, 			"MOAIFrameBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferTextureVK, 	"MOAIFrameBufferTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIGfxMgrVK, 				"MOAIGfxMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIImageTextureVK, 		"MOAIImageTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIIndexBufferVK, 			"MOAIIndexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderVK, 				"MOAIShaderVK" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderMgrVK, 			"MOAIShaderMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderProgramVK, 		"MOAIShaderProgram" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAITexture2DVK,			"MOAITexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexArrayVK, 			"MOAIVertexArray" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexBufferVK, 		"MOAIVertexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatVK, 		"MOAIVertexFormat" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatMgrVK, 		"MOAIVertexFormatMgr" )
}

//----------------------------------------------------------------//
void AKUDetectFramebuffer () {

//	MOAIGfxMgrVK::Get ().DetectFramebuffer ();
}

//----------------------------------------------------------------//
void AKUDetectGfxContext () {

//	MOAIGfxMgrVK::Get ().DetectContext ();
//	MOAIShaderMgrVK::Get ().AffirmAll ();
//	MOAIGfxMgrVK::Get ().RenewResources (); // TODO: ZLGfx
}

//----------------------------------------------------------------//
void AKUDiscardGfxResources () {

//	MOAIGfxMgrVK::Get ().DiscardResources ();
}

//----------------------------------------------------------------//
void AKUDisplayListBeginPhase ( int phase ) {

//	MOAIGfxMgrVK::Get ().BeginPhase ( phase );
}

//----------------------------------------------------------------//
void AKUDisplayListEnable ( int list ) {

//	MOAIGfxMgrVK::Get ().EnablePipeline ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListEndPhase ( int phase ) {

//	MOAIGfxMgrVK::Get ().EndPhase ( phase );
}

//----------------------------------------------------------------//
bool AKUDisplayListHasContent ( int list ) {

//	return MOAIGfxMgrVK::Get ().HasContent ( list );
	return false;
}

//----------------------------------------------------------------//
bool AKUDisplayListIsEnabled ( int list ) {

//	return MOAIGfxMgrVK::Get ().IsPipelineEnabled ( list );
	return false;
}

//----------------------------------------------------------------//
void AKUDisplayListProcess ( int list ) {

//	MOAIGfxMgrVK::Get ().ProcessPipeline ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListPublishAndReset () {

//	MOAIGfxMgrVK::Get ().PublishAndReset ( MOAIGfxMgrVK::LOADING_PIPELINE );
//	MOAIGfxMgrVK::Get ().PublishAndReset ( MOAIGfxMgrVK::DRAWING_PIPELINE );
}
