// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListenerHandle
//================================================================//

//----------------------------------------------------------------//
ZLGfxListener* ZLGfxListenerHandle::Listener ( ZLGfxListenerHandle* handle ) {

	return handle ? handle->mListener : 0;
}

//----------------------------------------------------------------//
ZLGfxListenerHandle::ZLGfxListenerHandle () :
	mListener ( 0 ) {
}

//----------------------------------------------------------------//
ZLGfxListenerHandle::~ZLGfxListenerHandle () {
}

//================================================================//
// ZLGfxListener
//================================================================//

//----------------------------------------------------------------//
void ZLGfxListener::Abandon () {

	if ( this->mHandle ) {
		this->mHandle->mListener = 0;
		this->mHandle->Release ();
	}
	
	this->mHandle = new ZLGfxListenerHandle ();
	this->mHandle->Retain ();
	this->mHandle->mListener = this;
}

//----------------------------------------------------------------//
void ZLGfxListener::OnSignal ( u32 signal, void* userdata ) {
	UNUSED ( signal );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::OnUniformLocation ( u32 addr, void* userdata ) {
	UNUSED ( addr );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
ZLGfxListener::ZLGfxListener () :
	mHandle ( 0 ) {

	this->Abandon ();
}

//----------------------------------------------------------------//
ZLGfxListener::~ZLGfxListener () {

	this->mHandle->mListener = 0;
	this->mHandle->Release ();
}
