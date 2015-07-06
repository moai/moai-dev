// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIFrameBufferTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initializes frame buffer.
	
	@in		MOAIFrameBufferTexture self
	@in		number width
	@in		number height
	@opt	number colorFormat
	@opt	number depthFormat
	@opt	number stencilFormat
	@out	nil
*/
int MOAIFrameBufferTexture::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferTexture, "UNN" )
	
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
// MOAIFrameBufferTexture
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::Init ( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat ) {

	this->Destroy ();

	this->mWidth			= width;
	this->mHeight			= height;
	this->mColorFormat		= colorFormat;
	this->mDepthFormat		= depthFormat;
	this->mStencilFormat	= stencilFormat;
	
	this->FinishInit ();
	this->DoCPUAffirm ();
}

//----------------------------------------------------------------//
MOAIFrameBufferTexture::MOAIFrameBufferTexture () :
	mGLColorBufferID ( 0 ),
	mGLDepthBufferID ( 0 ),
	mGLStencilBufferID ( 0 ),
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFrameBuffer )
		RTTI_EXTEND ( MOAISingleTexture )
	RTTI_END
 
	this->mDebugName = "(texture from MOAIFrameBufferTexture)";
}

//----------------------------------------------------------------//
MOAIFrameBufferTexture::~MOAIFrameBufferTexture () {

	this->OnGPUDestroy ();
}

//----------------------------------------------------------------//
bool MOAIFrameBufferTexture::OnGPUCreate () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
		return false;
	}
	
	this->mBufferWidth = this->mWidth;
	this->mBufferHeight = this->mHeight;
	
	ZLGfx& gfx = MOAIGfxDevice::GetAPI ();
	
	// bail and retry (no error) if GL cannot generate buffer ID
	this->mGLFrameBufferID = gfx.CreateFramebuffer ();
	if ( !this->mGLFrameBufferID ) return false;
	
	if ( this->mColorFormat ) {
		this->mGLColorBufferID = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLColorBufferID );
		gfx.RenderbufferStorage ( this->mColorFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mDepthFormat ) {
		this->mGLDepthBufferID = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLDepthBufferID );
		gfx.RenderbufferStorage ( this->mDepthFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mStencilFormat ) {
		this->mGLStencilBufferID = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLStencilBufferID );
		gfx.RenderbufferStorage ( this->mStencilFormat, this->mWidth, this->mHeight );
	}
	
	gfx.BindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, this->mGLFrameBufferID );
	
	if ( this->mGLColorBufferID ) {
		gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLColorBufferID );
	}
	
	if ( this->mGLDepthBufferID ) {
		gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH, this->mGLDepthBufferID );
	}
	
	if ( this->mGLStencilBufferID ) {
		gfx.FramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL, this->mGLStencilBufferID );
	}
	
	gfx.PushSection ();
	
	// TODO: handle error; clear
	gfx.CheckFramebufferStatus ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ );
	
	bool status = false;
	
	if ( gfx.PushSuccessHandler ()) {
	
		this->mGLTexID = gfx.CreateTexture ();
		gfx.BindTexture ( this->mGLTexID );
		gfx.TexImage2D ( 0, ZGL_PIXEL_FORMAT_RGBA, this->mWidth, this->mHeight, ZGL_PIXEL_FORMAT_RGBA, ZGL_PIXEL_TYPE_UNSIGNED_BYTE, 0 );
		gfx.FramebufferTexture2D ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLTexID, 0 );
				
		// refresh tex params on next bind
		this->mIsDirty = true;
		
        // clearing framebuffer because it might contain garbage
        gfx.ClearColor ( 0, 0, 0, 0 );
        gfx.Clear ( ZGL_CLEAR_COLOR_BUFFER_BIT | ZGL_CLEAR_STENCIL_BUFFER_BIT | ZGL_CLEAR_DEPTH_BUFFER_BIT );
		
		status = true;
	}
	
	if ( gfx.PushErrorHandler ()) {
	
		gfx.DeleteHandle ( this->mGLFrameBufferID );
		gfx.DeleteHandle ( this->mGLColorBufferID );
		gfx.DeleteHandle ( this->mGLDepthBufferID );
		gfx.DeleteHandle ( this->mGLStencilBufferID );
	}

	gfx.PopSection ();
	
	return status;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnGPUDestroy () {

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mGLFrameBufferID );
	this->mGLFrameBufferID = 0;

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mGLColorBufferID );
	this->mGLColorBufferID = 0;

	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mGLDepthBufferID );
	this->mGLDepthBufferID = 0;
	
	MOAIGfxResourceMgr::Get ().PushDeleter ( this->mGLStencilBufferID );
	this->mGLStencilBufferID = 0;
	
	this->MOAISingleTexture::OnGPUDestroy ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnGPULost () {

	this->mGLFrameBufferID = 0;
	this->mGLColorBufferID = 0;
	this->mGLDepthBufferID = 0;
	this->mGLStencilBufferID = 0;

	this->MOAISingleTexture::OnGPULost ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIFrameBuffer::RegisterLuaClass ( state );
	MOAISingleTexture::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIFrameBuffer::RegisterLuaFuncs ( state );
	MOAISingleTexture::RegisterLuaFuncs ( state );	

	luaL_Reg regTable [] = {
		{ "init",						_init },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::Render () {

	if ( this->PrepareForBind ()) {
		MOAIFrameBuffer::Render ();
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAISingleTexture::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAISingleTexture::SerializeOut ( state, serializer );
}