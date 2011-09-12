// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIFrameBuffer.h>

//================================================================//
// MOAIFrameBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBuffer::Init ( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat ) {

	this->Clear ();

	this->mWidth			= width;
	this->mHeight			= height;
	this->mColorFormat		= colorFormat;
	this->mDepthFormat		= depthFormat;
	this->mStencilFormat	= stencilFormat;
	
	this->OnLoad ();
}

//----------------------------------------------------------------//
bool MOAIFrameBuffer::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIFrameBuffer::IsValid () {

	return ( this->mGLFrameBufferID != 0 );
}

//----------------------------------------------------------------//
MOAIFrameBuffer::MOAIFrameBuffer () :
	mWidth ( 0 ),
	mHeight ( 0 ),
	mGLFrameBufferID ( 0 ),
	mGLColorBufferID ( 0 ),
	mGLDepthBufferID ( 0 ),
	mGLStencilBufferID ( 0 ),
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBuffer::~MOAIFrameBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnBind () {

	if ( this->mGLFrameBufferID ) {
		glBindFramebuffer ( GL_FRAMEBUFFER, this->mGLFrameBufferID );
	}
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnClear () {

	this->OnUnload ();
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnLoad () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
		this->SetError ();
		return;
	}
	
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
	
	if ( status != GL_FRAMEBUFFER_COMPLETE ) {
		this->Clear ();
		this->SetError ();
	}
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnRenew () {
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnUnload () {

	if ( this->mGLFrameBufferID ) {
		glDeleteFramebuffers ( 1, &this->mGLFrameBufferID );
	}
	
	if ( this->mGLColorBufferID ) {
		glDeleteRenderbuffers ( 1, &this->mGLColorBufferID );
	}
	
	if ( this->mGLDepthBufferID ) {
		glDeleteRenderbuffers ( 1, &this->mGLDepthBufferID );
	}
	
	if ( this->mGLStencilBufferID ) {
		glDeleteRenderbuffers ( 1, &this->mGLStencilBufferID );
	}
}

//----------------------------------------------------------------//
STLString MOAIFrameBuffer::ToString () {

	STLString repr;

	return repr;
}