// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListener
//================================================================//

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( event );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnReadPixels ( const ZLCopyOnWrite& copyOnWrite, void* userdata ) {
	UNUSED ( copyOnWrite );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {
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
ZLGfxHandle ZLGfx::CreateBuffer () {

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return ZLGfxHandle ( ZLGfxResource::NONE, 0, ZLGfxResource::NOT_ALLOCATED );
	#else
		ZLGfxHandle handle ( ZLGfxResource::BUFFER, 0, ZLGfxResource::PENDING_ALLOCATION );
		this->AllocateResource ( *handle.mResource, 0 );
		return handle;
	#endif
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateFramebuffer () {

	ZLGfxHandle handle ( ZLGfxResource::FRAMEBUFFER, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateProgram () {

	ZLGfxHandle handle ( ZLGfxResource::PROGRAM, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateRenderbuffer () {

	ZLGfxHandle handle ( ZLGfxResource::RENDERBUFFER, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateShader ( u32 shaderType ) {

	ZLGfxHandle handle ( ZLGfxResource::SHADER, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, shaderType );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateTexture () {

	ZLGfxHandle handle ( ZLGfxResource::TEXTURE, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::CreateVertexArray () {

	ZLGfxHandle handle ( ZLGfxResource::VERTEXARRAY, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->AllocateResource ( *handle.mResource, 0 );
	return handle;
}

//----------------------------------------------------------------//
void ZLGfx::DiscardResource ( ZLGfxResource& resource ) {

	resource.Discard ();
}

//----------------------------------------------------------------//
ZLGfxHandle ZLGfx::GetCurrentFramebuffer () {

	ZLGfxHandle handle ( ZLGfxResource::FRAMEBUFFER, 0, ZLGfxResource::PENDING_ALLOCATION );
	this->GetCurrentFramebuffer ( *handle.mResource );
	return handle;
}

//----------------------------------------------------------------//
bool ZLGfx::IsFlag ( u32 flag ) {

	return (( ZGL_FIRST_FLAG <= flag ) && ( flag < ZGL_TOTAL_FLAGS ));
}
