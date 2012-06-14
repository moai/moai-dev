// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIFrameBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes frame buffer.
	
	@in		MOAIFrameBuffer self
	@in		number width
	@in		number height
	@out	nil
*/
int MOAIFrameBuffer::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "UNN" )
	
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

//----------------------------------------------------------------//
/**	@name	setClearColor
	@text	At the start of each frame the buffer will by default automatically
	render a background color.  Using this function you can set the background color
	that is drawn each frame.  If you specify no arguments to this function, then
	automatic redraw of the background color will be turned off (i.e
	the previous render will be used as the background).

	@in		MOAIFrameBuffer self
	@opt	number red			The red value of the color.
	@opt	number green		The green value of the color.
	@opt	number blue			The blue value of the color.
	@opt	number alpha		The alpha value of the color.
	@out	nil
*/
int MOAIFrameBuffer::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )
	
	self->mClearFlags &= ~GL_COLOR_BUFFER_BIT;
	self->mClearColor = 0;

	if ( state.GetTop () > 1 ) {
	
		float r = state.GetValue < float >( 2, 0.0f );
		float g = state.GetValue < float >( 3, 0.0f );
		float b = state.GetValue < float >( 4, 0.0f );
		float a = state.GetValue < float >( 5, 0.0f );
		
		self->mClearColor = USColor::PackRGBA ( r, g, b, a );
		self->mClearFlags |= GL_COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setClearDepth
	@text	At the start of each frame the buffer will by default automatically
			clear the depth buffer.  This function sets whether or not the depth
			buffer should be cleared at the start of each frame.

	@in		MOAIFrameBuffer self
	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
*/
int MOAIFrameBuffer::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )
	
	bool clearDepth = state.GetValue < bool >( 2, false );
	
	if ( clearDepth ) {
		self->mClearFlags |= GL_DEPTH_BUFFER_BIT;
	}
	else {
		self->mClearFlags &= ~GL_DEPTH_BUFFER_BIT;
	}
	return 0;
}

//================================================================//
// MOAIFrameBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBuffer::BindAsFrameBuffer () {

	if ( this->Affirm ()) {
		
		glBindFramebuffer ( GL_FRAMEBUFFER, this->mGLFrameBufferID );
		
		if ( this->mClearFlags ) {
		
			if ( this->mClearFlags & GL_COLOR_BUFFER_BIT ) {
				USColorVec colorVec;
				colorVec.SetRGBA ( this->mClearColor );
				glClearColor ( colorVec.mR, colorVec.mG, colorVec.mB, colorVec.mA );
			}
			glClear ( this->mClearFlags );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::Init ( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat ) {

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
bool MOAIFrameBuffer::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIFrameBuffer::IsValid () {

	return ( this->mGLFrameBufferID != 0 );
}

//----------------------------------------------------------------//
MOAIFrameBuffer::MOAIFrameBuffer () :
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0 ),
	mGLFrameBufferID ( 0 ),
	mGLColorBufferID ( 0 ),
	mGLDepthBufferID ( 0 ),
	mGLStencilBufferID ( 0 ),
	mColorFormat ( 0 ),
	mDepthFormat ( 0 ),
	mStencilFormat ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITextureBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBuffer::~MOAIFrameBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnCreate () {
	
	if ( !( this->mWidth && this->mHeight && ( this->mColorFormat || this->mDepthFormat || this->mStencilFormat ))) {
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
void MOAIFrameBuffer::OnDestroy () {

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
void MOAIFrameBuffer::OnInvalidate () {

	this->mGLFrameBufferID = 0;
	this->mGLColorBufferID = 0;
	this->mGLDepthBufferID = 0;
	this->mGLStencilBufferID = 0;

	this->MOAITextureBase::OnInvalidate ();
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::OnLoad () {
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITextureBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAITextureBase::RegisterLuaFuncs ( state );	

	luaL_Reg regTable [] = {
		{ "init",						_init },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAITextureBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAITextureBase::SerializeOut ( state, serializer );
}