// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIFrameBufferGL.h>
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAIShaderMgrGL.h>
#include <moai-gfx/MOAITextureGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgrGL::_enablePipelineLogging ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrGL, "" )

	MOAIGfxMgrGL::Get ().EnablePipelineLogging ( state.GetValue < bool >( 1, false ));

	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getFrameBuffer
	@text	Returns the frame buffer associated with the device.

	@out	MOAIFrameBufferGL frameBuffer
*/
int MOAIGfxMgrGL::_getFrameBuffer ( lua_State* L ) {

	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().GetDefaultFrameBuffer ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureSize
	@text	Returns the maximum texture size supported by device
 
	@out	number maxTextureSize
*/
int MOAIGfxMgrGL::_getMaxTextureSize ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgrGL::Get ().mMaxTextureSize );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTextureUnits
	@text	Returns the total number of texture units available on the device.

	@out	number maxTextureUnits
*/
int MOAIGfxMgrGL::_getMaxTextureUnits ( lua_State* L ) {

	lua_pushnumber ( L, ( double )MOAIGfxMgrGL::Get ().CountTextureUnits ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewSize
	@text	Returns the width and height of the view
 
	@out	number width
	@out	number height
*/
int MOAIGfxMgrGL::_getViewSize ( lua_State* L  ) {

	ZLFrameBufferGL* frameBuffer = MOAIGfxMgrGL::Get ().GetCurrentFrameBuffer ();
	
	lua_pushnumber ( L, frameBuffer->GetBufferWidth ());
	lua_pushnumber ( L, frameBuffer->GetBufferHeight ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	purgeResources
	@text	Purges all resources older that a given age (in render cycles).
			If age is 0 then all resources are purged.
 
	@opt	number age		Default value is 0.
	@out	nil
*/
int MOAIGfxMgrGL::_purgeResources ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 age = state.GetValue < u32 >( 1, 0 );

	MOAIGfxMgrGL::Get ().PurgeResources ( age );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxMgrGL::_renewResources ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIGfxMgrGL::Get ().RenewResources ();
	
	return 0;
}

//================================================================//
// MOAIGfxMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrGL::MOAIGfxMgrGL () {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxMgrGL )
	RTTI_END
	
	this->SetDefaultFrameBuffer ( new MOAIFrameBufferGL ());
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

//----------------------------------------------------------------//
void MOAIGfxMgrGL::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
	
	state.SetField ( -1, "DRAWING_PIPELINE",	( u32 )ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",	( u32 )ZLGfxPipelineClerkGL::LOADING_PIPELINE );

	luaL_Reg regTable [] = {
		{ "enablePipelineLogging",		_enablePipelineLogging },
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgrGL > },
		{ "getMaxTextureSize",			_getMaxTextureSize },
		{ "getMaxTextureUnits",			_getMaxTextureUnits },
		{ "getViewSize",				_getViewSize },
		{ "purgeResources",				_purgeResources },
		{ "renewResources",				_renewResources },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgrGL > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// virtuals
//================================================================//

//----------------------------------------------------------------//
ZLAbstractShader* MOAIGfxMgrGL::MOAIAbstractGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

	MOAIShaderGL* shader = 0;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = MOAIShaderMgrGL::Get ().GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgrGL::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShaderGL >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
ZLAbstractTexture* MOAIGfxMgrGL::MOAIAbstractGfxMgr_AffirmTexture ( MOAILuaState& state, int idx ) const {

	MOAITextureBaseGL* textureBase = 0;
	
	textureBase = state.GetLuaObject < MOAITextureBaseGL >( idx, false );
	if ( textureBase ) return textureBase;
	
	MOAITextureGL* texture = new MOAITextureGL ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = 0;
	}
	return texture;
}
