// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLFrameBufferTextureGL.h>

//================================================================//
// ZLFrameBufferTextureGL
//================================================================//

//----------------------------------------------------------------//
void ZLFrameBufferTextureGL::Init ( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat ) {

	this->Destroy ();

	this->mWidth			= width;
	this->mHeight			= height;
	this->mColorFormat		= colorFormat;
	this->mDepthFormat		= depthFormat;
	this->mStencilFormat	= stencilFormat;
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
ZLFrameBufferTextureGL::ZLFrameBufferTextureGL () :
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( ZLFrameBufferGL )
		RTTI_EXTEND ( ZLTextureGL )
	RTTI_END
 
	this->mDebugName = "(texture from ZLFrameBufferTextureGL)";
}

//----------------------------------------------------------------//
ZLFrameBufferTextureGL::~ZLFrameBufferTextureGL () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void ZLFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUBind () {

	this->NeedsClear ( true );
	ZLTextureBaseGL::ZLAbstractGfxResource_OnGPUBind ();
}

//----------------------------------------------------------------//
bool ZLFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUCreate () {
	
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
void ZLFrameBufferTextureGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mGLFrameBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLColorBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLDepthBuffer, shouldDelete );
	this->mGfxMgr->DeleteOrDiscard ( this->mGLStencilBuffer, shouldDelete );

	this->ZLTextureGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( shouldDelete );
}

//----------------------------------------------------------------//
void ZLFrameBufferTextureGL::ZLFrameBuffer_AffirmBuffers () {

	this->Affirm ();
}
