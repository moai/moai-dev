// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIFrameBufferTexture.h>

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
		GLenum colorFormat		= state.GetValue < GLenum >( 4, GL_RGB565 );
	#else
		GLenum colorFormat		= state.GetValue < GLenum >( 4, GL_RGBA8 );
	#endif
	
	GLenum depthFormat		= state.GetValue < GLenum >( 5, 0 );
	GLenum stencilFormat	= state.GetValue < GLenum >( 6, 0 );
	
	self->Init ( width, height, colorFormat, depthFormat, stencilFormat );
	
	return 0;
}



//================================================================//
// MOAIFrameBufferTexture
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferTexture::Init ( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat ) {

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
	glGenFramebuffers ( 1, &this->mGLFrameBufferID );
	if ( !this->mGLFrameBufferID ) return;
	
	if ( this->mColorFormat ) {
		glGenRenderbuffers( 1, &this->mGLColorBufferID );
		glBindRenderbuffer ( GL_RENDERBUFFER, this->mGLColorBufferID );
		glRenderbufferStorage ( GL_RENDERBUFFER, this->mColorFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mDepthFormat ) {
		glGenRenderbuffers ( 1, &this->mGLDepthBufferID );
		glBindRenderbuffer ( GL_RENDERBUFFER, this->mGLDepthBufferID );
		glRenderbufferStorage ( GL_RENDERBUFFER, this->mDepthFormat, this->mWidth, this->mHeight );
	}
	
	if ( this->mStencilFormat ) {
		glGenRenderbuffers ( 1, &this->mGLStencilBufferID );
		glBindRenderbuffer ( GL_RENDERBUFFER, this->mGLStencilBufferID );
		glRenderbufferStorage ( GL_RENDERBUFFER, this->mStencilFormat, this->mWidth, this->mHeight );
	}
	
	glBindFramebuffer ( GL_FRAMEBUFFER, this->mGLFrameBufferID );
	
	if ( this->mGLColorBufferID ) {
		glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->mGLColorBufferID );
	}
	
	if ( this->mGLDepthBufferID ) {
		glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->mGLDepthBufferID );
	}
	
	if ( this->mGLStencilBufferID ) {
		glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->mGLStencilBufferID );
	}
	
	// TODO: handle error; clear
	GLenum status = glCheckFramebufferStatus ( GL_FRAMEBUFFER );
	
	if ( status == GL_FRAMEBUFFER_COMPLETE ) {
	
		glGenTextures ( 1, &this->mGLTexID );
		glBindTexture ( GL_TEXTURE_2D, this->mGLTexID );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mGLTexID, 0 );
				
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