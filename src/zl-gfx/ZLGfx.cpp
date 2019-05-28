// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfx
//================================================================//

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateBuffer () {

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return ZLGfxHandleGL ( ZLGfxResourceGL::NONE, 0, ZLGfxResourceGL::NOT_ALLOCATED );
	#else
		ZLGfxHandleGL handle ( ZLGfxResourceGL::BUFFER, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
		this->AllocateResource ( *handle.mResource, 0 );
		return handle;
	#endif
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateFramebuffer () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::FRAMEBUFFER, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateProgram () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::PROGRAM, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateRenderbuffer () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::RENDERBUFFER, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateShader ( u32 shaderType ) {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::SHADER, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, shaderType );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateTexture () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::TEXTURE, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::CreateVertexArray () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::VERTEXARRAY, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
void ZLGfx::DiscardResource ( ZLGfxResourceGL& resource ) {

	resource.Discard ();
}

//----------------------------------------------------------------//
ZLGfxHandleGL ZLGfx::GetCurrentFramebuffer () {

	ZLGfxHandleGL handle ( ZLGfxResourceGL::FRAMEBUFFER, 0, ZLGfxResourceGL::PENDING_ALLOCATION );
	this->GetCurrentFramebuffer ( *handle.mResource );
	return handle;
}

//----------------------------------------------------------------//
bool ZLGfx::IsFlag ( u32 flag ) {

	return (( ZGL_FIRST_FLAG <= flag ) && ( flag < ZGL_TOTAL_FLAGS ));
}
