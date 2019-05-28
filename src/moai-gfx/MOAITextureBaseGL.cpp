// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Returns the width and height of the texture's source image.
			Avoid using the texture width and height to compute UV
			coordinates from pixels, as this will prevent texture
			resolution swapping.
	
	@in		MOAITextureBaseGL self
	@out	number width
	@out	number height
*/
int MOAITextureBaseGL::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureBaseGL, "U" )
	
	self->DoCPUCreate ();
	
	lua_pushnumber ( state, self->mWidth );
	lua_pushnumber ( state, self->mHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	release
	@text	Releases any memory associated with the texture.
	
	@in		MOAITextureBaseGL self
	@out	nil
*/
int MOAITextureBaseGL::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureBaseGL, "U" )
	
	self->Destroy ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDebugName
	@text	Set a name for the texture to use during memory logging.
	
	@in		MOAITextureBaseGL self
	@in		string debugName
	@out	nil
*/
int MOAITextureBaseGL::_setDebugName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureBaseGL, "U" )

	self->mDebugName = state.GetValue < cc8* >( 2, "" );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFilter
	@text	Set default filtering mode for texture.
	
	@in		MOAITextureBaseGL self
	@in		number min			One of MOAITextureBaseGL.GL_LINEAR, MOAITextureBaseGL.GL_LINEAR_MIPMAP_LINEAR, MOAITextureBaseGL.GL_LINEAR_MIPMAP_NEAREST,
								MOAITextureBaseGL.GL_NEAREST, MOAITextureBaseGL.GL_NEAREST_MIPMAP_LINEAR, MOAITextureBaseGL.GL_NEAREST_MIPMAP_NEAREST
	@opt	number mag			Defaults to value passed to 'min'.
	@out	nil
*/
int MOAITextureBaseGL::_setFilter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureBaseGL, "UN" )

	int min = state.GetValue < int >( 2, ZGL_SAMPLE_LINEAR );
	int mag = state.GetValue < int >( 3, min );

	MOAITextureBaseGL::CheckFilterModes ( min, mag );

	self->SetFilter ( min, mag );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setWrap
	@text	Set wrapping mode for texture.
	
	@in		MOAITextureBaseGL self
	@in		boolean wrap		Texture will wrap if true, clamp if not.
	@out	nil
*/
int MOAITextureBaseGL::_setWrap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextureBaseGL, "UB" )
	
	bool wrap = state.GetValue < bool >( 2, false );
	
	self->mWrap = wrap ? ZGL_WRAP_MODE_REPEAT : ZGL_WRAP_MODE_CLAMP;

	return 0;
}

//================================================================//
// MOAITextureBaseGL
//================================================================//

////----------------------------------------------------------------//
//MOAILuaObject& MOAITextureBaseGL::AsLuaObject () {
//	return *this;
//}

//----------------------------------------------------------------//
MOAITextureBaseGL::MOAITextureBaseGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( ZLTextureGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureBaseGL::~MOAITextureBaseGL () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
}

//----------------------------------------------------------------//
void MOAITextureBaseGL::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GL_LINEAR",					( u32 )ZGL_SAMPLE_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_LINEAR",		( u32 )ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_LINEAR_MIPMAP_NEAREST",	( u32 )ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_NEAREST",					( u32 )ZGL_SAMPLE_NEAREST );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_LINEAR",	( u32 )ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR );
	state.SetField ( -1, "GL_NEAREST_MIPMAP_NEAREST",	( u32 )ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST );
	
	state.SetField ( -1, "GL_RGBA4",					( u32 )ZGL_PIXEL_FORMAT_RGBA4 );
	state.SetField ( -1, "GL_RGB5_A1",					( u32 )ZGL_PIXEL_FORMAT_RGB5_A1 );
	state.SetField ( -1, "GL_DEPTH_COMPONENT16",		( u32 )ZGL_PIXEL_FORMAT_DEPTH_COMPONENT16 );
	//***state.SetField ( -1, "GL_DEPTH_COMPONENT24",	( u32 )GL_DEPTH_COMPONENT24 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX1",		( u32 )GL_STENCIL_INDEX1 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX4",		( u32 )GL_STENCIL_INDEX4 );
	state.SetField ( -1, "GL_STENCIL_INDEX8",			( u32 )ZGL_PIXEL_FORMAT_STENCIL_INDEX8 );
	//***state.SetField ( -1, "GL_STENCIL_INDEX16",		( u32 )GL_STENCIL_INDEX16 );
	
	// TODO:
	#ifdef MOAI_OS_ANDROID
		state.SetField ( -1, "GL_RGB565",				( u32 )ZGL_PIXEL_FORMAT_RGB565 );
	#else
		state.SetField ( -1, "GL_RGBA8",				( u32 )ZGL_PIXEL_FORMAT_RGBA8 );
	#endif
}

//----------------------------------------------------------------//
void MOAITextureBaseGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getSize",				_getSize },
		{ "release",				_release },
		{ "setDebugName",			_setDebugName },
		{ "setFilter",				_setFilter },
		{ "setWrap",				_setWrap },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
