// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfxHandle.h>

//================================================================//
// ZLGfxResource
//================================================================//

ZLGfxResource ZLGfxResource::UNBIND ( ZLGfxResource::NONE, 0, ZLGfxResource::NOT_ALLOCATED );

//----------------------------------------------------------------//
void ZLGfxResource::Discard () {

	this->mType = NONE;
	this->mGLID = 0;
	this->mStatus = NOT_ALLOCATED;
}

//----------------------------------------------------------------//
u32 ZLGfxResource::GLID () const {

	return this->mGLID;
}

//----------------------------------------------------------------//
ZLGfxResource* ZLGfxResource::RetainForWrite () {

	this->Retain ();
	return this;
}

//----------------------------------------------------------------//
ZLGfxResource::ZLGfxResource ( ZLGfxResource::Type type, u32 glid, ZLGfxResource::Status status ) :
	mType ( type ),
	mGLID ( glid ),
	mStatus ( status ) {
}

//----------------------------------------------------------------//
ZLGfxResource::~ZLGfxResource () {

	assert (( this->mStatus == SYSTEM_ALLOCATED ) || ( this->mGLID == 0 ));
}

//================================================================//
// ZLGfxHandle
//================================================================//

//----------------------------------------------------------------//
ZLGfxHandle& ZLGfxHandle::operator= ( const ZLGfxHandle& handle ) {

	if ( this->mResource != handle.mResource ) {

		if ( handle.mResource ) {
			handle.mResource->Retain ();
		}

		if ( this->mResource ) {
			this->mResource->Release ();
		}
		
		this->mResource = handle.mResource;
	}
	return *this;
}

//----------------------------------------------------------------//
bool ZLGfxHandle::CanBind () const {
	
	return this->mResource ? this->mResource->mGLID != 0 : false;
}

//----------------------------------------------------------------//
u32 ZLGfxHandle::GetStatus () const {

	return this->mResource ? this->mResource->mStatus : ZLGfxResource::NOT_ALLOCATED;
}

//----------------------------------------------------------------//
u32 ZLGfxHandle::GetType () const {

	return this->mResource ? this->mResource->mType : ZLGfxResource::NONE;
}

//----------------------------------------------------------------//
ZLGfxHandle::ZLGfxHandle () :
	mResource ( 0 ) {
}

//----------------------------------------------------------------//
ZLGfxHandle::ZLGfxHandle ( ZLGfxResource::Type type, u32 glid, ZLGfxResource::Status status ) {

	this->mResource = new ZLGfxResource ( type, glid, status );
	this->mResource->Retain ();
}

//----------------------------------------------------------------//
ZLGfxHandle::ZLGfxHandle ( const ZLGfxHandle& handle ) {
	
	this->mResource = handle.mResource;
	
	if ( this->mResource ) {
		this->mResource->Retain ();
	}
}

//----------------------------------------------------------------//
ZLGfxHandle::~ZLGfxHandle () {

	if ( this->mResource ) {
		this->mResource->Release ();
	}
}
