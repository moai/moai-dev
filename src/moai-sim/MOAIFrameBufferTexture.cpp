// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
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
}

//----------------------------------------------------------------//
MOAIFrameBufferTexture::MOAIFrameBufferTexture () :
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFrameBuffer )
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
 
	this->mDebugName = "(texture from MOAIFrameBufferTexture)";
}

//----------------------------------------------------------------//
MOAIFrameBufferTexture::~MOAIFrameBufferTexture () {

	this->OnGPUDeleteOrDiscard ( true );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnGPUBind () {

	this->NeedsClear ( true );
	MOAITextureBase::OnGPUBind ();
}

//----------------------------------------------------------------//
bool MOAIFrameBufferTexture::OnGPUCreate () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
		return false;
	}
	
	this->mBufferWidth = this->mWidth;
	this->mBufferHeight = this->mHeight;
	
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
	
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
		
		this->OnGPUUpdate ();
		
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
void MOAIFrameBufferTexture::OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	MOAIGfxResourceClerk::DeleteOrDiscard ( this->mGLFrameBuffer, shouldDelete );
	MOAIGfxResourceClerk::DeleteOrDiscard ( this->mGLColorBuffer, shouldDelete );
	MOAIGfxResourceClerk::DeleteOrDiscard ( this->mGLDepthBuffer, shouldDelete );
	MOAIGfxResourceClerk::DeleteOrDiscard ( this->mGLStencilBuffer, shouldDelete );

	this->MOAITextureBase::OnGPUDeleteOrDiscard ( shouldDelete );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIFrameBuffer::RegisterLuaClass ( state );
	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIFrameBuffer::RegisterLuaFuncs ( state );
	MOAITextureBase::RegisterLuaFuncs ( state );	

	luaL_Reg regTable [] = {
		{ "init",						_init },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAITextureBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBase::SerializeOut ( state, serializer );
}
