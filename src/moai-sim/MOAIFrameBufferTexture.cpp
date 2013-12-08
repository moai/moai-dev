// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIFrameBufferTexture.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes frame buffer.
	
	@in		MOAIFrameBufferTexture self
	@in		number width
	@in		number height
	@out	nil
*/
int MOAIFrameBufferTexture::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferTexture, "UNN" )
	
	u32 width				= state.GetValue < u32 >( 2, 0 );
	u32 height				= state.GetValue < u32 >( 3, 0 );
	
	// TODO: fix me
	#ifdef MOAI_OS_ANDROID
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

	this->Clear ();

	if ( MOAIGfxDevice::Get ().IsFramebufferSupported ()) {

		this->mWidth			= width;
		this->mHeight			= height;
		this->mColorFormat		= colorFormat;
		this->mDepthFormat		= depthFormat;
		this->mStencilFormat	= stencilFormat;
		
		this->Load ();
	}
	else {
		MOAILog ( 0, MOAILogMessages::MOAITexture_NoFramebuffer );
	}
}

//----------------------------------------------------------------//
bool MOAIFrameBufferTexture::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIFrameBufferTexture::IsValid () {

	return ( this->mGLFrameBufferID != 0 );
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
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferTexture::~MOAIFrameBufferTexture () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnCreate () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
		return;
	}
	
	this->mBufferWidth = this->mWidth;
	this->mBufferHeight = this->mHeight;
	
	// bail and retry (no error) if GL cannot generate buffer ID
	this->mGLFrameBufferID = zglCreateFramebuffer ();
	if ( !this->mGLFrameBufferID ) return;
	
	if ( this->mColorFormat ) {
		this->mGLColorBufferID = zglCreateRenderbuffer ();
		zglBindRenderbuffer ( this->mGLColorBufferID );
		zglRenderbufferStorage ( this->mColorFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mDepthFormat ) {
		this->mGLDepthBufferID = zglCreateRenderbuffer ();
		zglBindRenderbuffer ( this->mGLDepthBufferID );
		zglRenderbufferStorage ( this->mDepthFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mStencilFormat ) {
		this->mGLStencilBufferID = zglCreateRenderbuffer ();
		zglBindRenderbuffer ( this->mGLStencilBufferID );
		zglRenderbufferStorage ( this->mStencilFormat, this->mWidth, this->mHeight );
	}
	
	zglBindFramebuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, this->mGLFrameBufferID );
	
	if ( this->mGLColorBufferID ) {
		zglFramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLColorBufferID );
	}
	
	if ( this->mGLDepthBufferID ) {
		zglFramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH, this->mGLDepthBufferID );
	}
	
	if ( this->mGLStencilBufferID ) {
		zglFramebufferRenderbuffer ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL, this->mGLStencilBufferID );
	}
	
	// TODO: handle error; clear
	u32 status = zglCheckFramebufferStatus ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ );
	
	if ( status == ZGL_FRAMEBUFFER_STATUS_COMPLETE ) {
	
		this->mGLTexID = zglCreateTexture ();
		zglBindTexture ( this->mGLTexID );
		zglTexImage2D ( 0, ZGL_PIXEL_FORMAT_RGBA, this->mWidth, this->mHeight, ZGL_PIXEL_FORMAT_RGBA, ZGL_PIXEL_TYPE_UNSIGNED_BYTE, 0 );
		zglFramebufferTexture2D ( ZGL_FRAMEBUFFER_TARGET_DRAW_READ, ZGL_FRAMEBUFFER_ATTACHMENT_COLOR, this->mGLTexID, 0 );
				
		// refresh tex params on next bind
		this->mIsDirty = true;
	}
	else {
		this->Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnDestroy () {

	if ( this->mGLFrameBufferID ) {
		MOAIGfxDevice::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_FRAMEBUFFER, this->mGLFrameBufferID );
		this->mGLFrameBufferID = 0;
	}
	
	if ( this->mGLColorBufferID ) {
		MOAIGfxDevice::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_RENDERBUFFER, this->mGLColorBufferID );
		this->mGLColorBufferID = 0;
	}
	
	if ( this->mGLDepthBufferID ) {
		MOAIGfxDevice::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_RENDERBUFFER, this->mGLDepthBufferID );
		this->mGLDepthBufferID = 0;
	}
	
	if ( this->mGLStencilBufferID ) {
		MOAIGfxDevice::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_RENDERBUFFER, this->mGLStencilBufferID );
		this->mGLStencilBufferID = 0;
	}
	
	this->MOAITextureBase::OnDestroy ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnInvalidate () {

	this->mGLFrameBufferID = 0;
	this->mGLColorBufferID = 0;
	this->mGLDepthBufferID = 0;
	this->mGLStencilBufferID = 0;

	this->MOAITextureBase::OnInvalidate ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::OnLoad () {
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
void MOAIFrameBufferTexture::Render () {

	if ( this->Affirm ()) {
		MOAIFrameBuffer::Render ();
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAITextureBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBase::SerializeOut ( state, serializer );
}