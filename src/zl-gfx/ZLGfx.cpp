// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListener
//================================================================//

//----------------------------------------------------------------//
void ZLGfxListener::OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( event );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::OnReadPixels ( const ZLCopyOnWrite& copyOnWrite, void* userdata ) {
	UNUSED ( copyOnWrite );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::OnUniformLocation ( u32 addr, void* userdata ) {
	UNUSED ( addr );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
ZLGfxListener::ZLGfxListener () {
}

//----------------------------------------------------------------//
ZLGfxListener::~ZLGfxListener () {
}

//================================================================//
// ZLGfx
//================================================================//

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateBuffer () {

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return 0;
	#else
		return this->Create ( new ZLGfxHandle ( ZLGfxHandle::BUFFER, 0, true ), 0 );
	#endif
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateFramebuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateProgram () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::PROGRAM, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateRenderbuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::RENDERBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateShader ( u32 shaderType ) {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::SHADER, 0, true ), shaderType );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateTexture () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::TEXTURE, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfx::CreateVertexArray () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::VERTEXARRAY, 0, true ), 0 );
}

//----------------------------------------------------------------//
void ZLGfx::Delete ( ZLGfxHandle*& handle ) {

	if ( handle ) {
		if ( handle->mOwns ) {
			this->Delete ( handle->mType, handle->mGLID );
		}
		handle = 0;
	}
}

//----------------------------------------------------------------//
void ZLGfx::Discard ( ZLGfxHandle*& handle ) {

	if ( handle ) {
		delete handle;
		handle = 0;
	}
}
