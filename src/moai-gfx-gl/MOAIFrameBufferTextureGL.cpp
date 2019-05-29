// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferTextureGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initializes frame buffer.
 
	@in		MOAIFrameBufferTextureGL self
	@in		number width
	@in		number height
	@opt	number colorFormat
	@opt	number depthFormat
	@opt	number stencilFormat
	@out	nil
*/
int MOAIFrameBufferTextureGL::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferTextureGL, "UNN" )
	
	u32 width				= state.GetValue < u32 >( 2, 0 );
	u32 height				= state.GetValue < u32 >( 3, 0 );
	
	// TODO: fix me
	#if defined ( MOAI_OS_ANDROID ) || defined ( MOAI_OS_HTML )
		u32 colorFormat		= state.GetValue < u32 >( 4, ZGL_PIXEL_FORMAT_RGB565 );
	#else
		u32 colorFormat		= state.GetValue < u32 >( 4, ZGL_PIXEL_FORMAT_RGBA8 );
	#endif
	
	u32 depthFormat		= state.GetValue < u32 >( 5, 0 );
	u32 stencilFormat	= state.GetValue < u32 >( 6, 0 );
	
	self->Init ( width, height, colorFormat, depthFormat, stencilFormat );
	
	return 0;
}

//================================================================//
// MOAIFrameBufferTextureGL
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::Init ( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat ) {

	this->Destroy ();

	this->mWidth			= width;
	this->mHeight			= height;
	this->mColorFormat		= colorFormat;
	this->mDepthFormat		= depthFormat;
	this->mStencilFormat	= stencilFormat;
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
MOAIFrameBufferTextureGL::MOAIFrameBufferTextureGL () :
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFrameBufferGL )
		RTTI_EXTEND ( MOAITextureBaseGL )
	RTTI_END
 
	this->mDebugName = "(texture from MOAIFrameBufferTextureGL)";
}

//----------------------------------------------------------------//
MOAIFrameBufferTextureGL::~MOAIFrameBufferTextureGL () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFrameBufferGL::RegisterLuaClass ( state );
	MOAITextureBaseGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIFrameBufferGL::RegisterLuaFuncs ( state );
	MOAITextureBaseGL::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "init",						_init },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAITextureBaseGL::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBaseGL::SerializeOut ( state, serializer );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUBind () {

	this->NeedsClear ( true );
	MOAITextureBaseGL::ZLAbstractGfxResource_OnGPUBind ();
}

//----------------------------------------------------------------//
bool MOAIFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUCreate () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
		return false;
	}
	
	this->mBufferWidth = this->mWidth;
	this->mBufferHeight = this->mHeight;
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
	// bail and retry (no error) if GL cannot generate buffer ID
	this->mGLFrameBuffer = gfx.CreateFramebuffer ();
	
	// TODO: error handling
	//if ( !this->mGLFrameBuffer ) return false;
	
	if ( this->mColorFormat ) {
		this->mGLColorBuffer = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLColorBuffer );
		gfx.RenderbufferStorage ( this->mColorFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mDepthFormat ) {
		this->mGLDepthBuffer = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLDepthBuffer );
		gfx.RenderbufferStorage ( this->mDepthFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mStencilFormat ) {
		this->mGLStencilBuffer = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLStencilBuffer );
		gfx.RenderbufferStorage ( this->mStencilFormat, this->mWidth, this->mHeight );
	}
	
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, this->mGLFrameBuffer );
	
	gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLColorBuffer );
	gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH, this->mGLDepthBuffer );
	gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL, this->mGLStencilBuffer );
	
	gfx.PushSection ();
	
	// TODO: handle error; clear
	gfx.CheckFramebufferStatus ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ );
	
	bool status = false;
	
	if ( gfx.PushSuccessHandler ()) {
	
		this->mGLTexture = gfx.CreateTexture ();
		gfx.BindTexture ( this->mGLTexture );
		gfx.TexImage2D ( 0, ZGL_PIXEL_FORMAT_RGBA, this->mWidth, this->mHeight, ZGL_PIXEL_FORMAT_RGBA, ZGL_PIXEL_TYPE_UNSIGNED_BYTE, 0 );
		gfx.FramebufferTexture2D ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLTexture, 0 );
		
        // clearing framebuffer because it might contain garbage
        gfx.ClearColor ( 0, 0, 0, 0 );
        gfx.Clear ( ZGL_CLEAR_COLOR_BUFFER_BIT | ZGL_CLEAR_STENCIL_BUFFER_BIT | ZGL_CLEAR_DEPTH_BUFFER_BIT );
		
		this->ZLAbstractGfxResource_OnGPUUpdate ();
		
		status = true;
	}
	
	if ( gfx.PushErrorHandler ()) {
	
		gfx.DeleteResource ( this->mGLFrameBuffer );
		gfx.DeleteResource ( this->mGLColorBuffer );
		gfx.DeleteResource ( this->mGLDepthBuffer );
		gfx.DeleteResource ( this->mGLStencilBuffer );
	}

	gfx.PopSection ();
	
	return status;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mGLFrameBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLColorBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLDepthBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLStencilBuffer, shouldDelete );

	this->MOAITextureBaseGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( shouldDelete );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureGL::ZLFrameBuffer_AffirmBuffers () {

	this->Affirm ();
}
