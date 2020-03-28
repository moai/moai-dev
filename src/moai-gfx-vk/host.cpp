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
void AKUDetectFramebuffer () {

//	MOAIGfxMgrVK::Get ().DetectFramebuffer ();
}

//----------------------------------------------------------------//
void AKUDetectGfxContext ( int width, int height, bool enableValidation ) {

	MOAIGfxMgrVK::Get ().DetectContext ( width, height, enableValidation );
	MOAIShaderMgrVK::Get ().AffirmAll ();
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

//	MOAIGfxMgrVK::Get ().EnableQueue ( list );
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

//	return MOAIGfxMgrVK::Get ().IsQueueEnabled ( list );
	return false;
}

//----------------------------------------------------------------//
void AKUDisplayListProcess ( int list ) {

//	MOAIGfxMgrVK::Get ().ProcessQueue ( list );
}

//----------------------------------------------------------------//
void AKUDisplayListPublishAndReset () {

//	MOAIGfxMgrVK::Get ().PublishAndReset ( MOAIGfxMgrVK::LOADING_QUEUE );
//	MOAIGfxMgrVK::Get ().PublishAndReset ( MOAIGfxMgrVK::DRAWING_QUEUE );
}

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

	MOAIShaderMgrVK::Affirm ();

	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferVK, 			"MOAIFrameBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIFrameBufferTextureVK, 	"MOAIFrameBufferTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIGfxMgrVK, 				"MOAIGfxMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIGfxScriptRetainedVK,	"MOAIGfxScriptRetained" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIImageTextureVK, 		"MOAIImageTexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIIndexBufferVK, 			"MOAIIndexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderVK, 				"MOAIShaderVK" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderMgrVK, 			"MOAIShaderMgr" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIShaderProgramVK, 		"MOAIShaderProgram" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAITexture2DVK,			"MOAITexture" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexBufferVK, 		"MOAIVertexBuffer" )
	REGISTER_LUA_CLASS_WITH_ALIAS ( MOAIVertexFormatVK, 		"MOAIVertexFormat" )
}

//----------------------------------------------------------------//
AKU_API void AKUGfxVKSetFunc_CreateSurface ( AKUGfxVKFunc_CreateSurface func, void* userdata ) {

	MOAIGfxMgrVK::Get ().SetHostCreateSurfaceFunc ( MOAIGfxMgrVK::HostCreateSurfaceFunc ( func, userdata ));
}

//----------------------------------------------------------------//
AKU_API void AKUGfxVKSetFunc_GetInstanceExtensions ( AKUGfxVKFunc_GetInstanceExtensions func, void* userdata ) {

	MOAIGfxMgrVK::Get ().SetHostGetInstanceExtensionsFunc ( MOAIGfxMgrVK::HostGetInstanceExtensionsFunc ( func, userdata ));
}
