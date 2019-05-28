// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-gfx/ZLGfxHandleGL.h>

//================================================================//
// ZLGfxResourceGL
//================================================================//

ZLGfxResourceGL ZLGfxResourceGL::UNBIND ( ZLGfxResourceGL::NONE, 0, ZLGfxResourceGL::NOT_ALLOCATED );

//----------------------------------------------------------------//
void ZLGfxResourceGL::Discard () {

	this->mType = NONE;
	this->mGLID = 0;
	this->mStatus = NOT_ALLOCATED;
}

//----------------------------------------------------------------//
u32 ZLGfxResourceGL::GLID () const {

	return this->mGLID;
}

//----------------------------------------------------------------//
ZLGfxResourceGL* ZLGfxResourceGL::RetainForWrite () {

	this->Retain ();
	return this;
}

//----------------------------------------------------------------//
ZLGfxResourceGL::ZLGfxResourceGL ( ZLGfxResourceGL::Type type, u32 glid, ZLGfxResourceGL::Status status ) :
	mType ( type ),
	mGLID ( glid ),
	mStatus ( status ) {
}

//----------------------------------------------------------------//
ZLGfxResourceGL::~ZLGfxResourceGL () {

	assert (( this->mStatus == SYSTEM_ALLOCATED ) || ( this->mGLID == 0 ));
}

//================================================================//
// ZLGfxHandleGL
//================================================================//

//----------------------------------------------------------------//
ZLGfxHandleGL& ZLGfxHandleGL::operator= ( const ZLGfxHandleGL& handle ) {

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
bool ZLGfxHandleGL::CanBind () const {
	
	return this->mResource ? this->mResource->mGLID != 0 : false;
}

//----------------------------------------------------------------//
u32 ZLGfxHandleGL::GetStatus () const {

	return this->mResource ? this->mResource->mStatus : ZLGfxResourceGL::NOT_ALLOCATED;
}

//----------------------------------------------------------------//
u32 ZLGfxHandleGL::GetType () const {

	return this->mResource ? this->mResource->mType : ZLGfxResourceGL::NONE;
}

//----------------------------------------------------------------//
ZLGfxHandleGL::ZLGfxHandleGL () :
	mResource ( 0 ) {
}

//----------------------------------------------------------------//
ZLGfxHandleGL::ZLGfxHandleGL ( ZLGfxResourceGL::Type type, u32 glid, ZLGfxResourceGL::Status status ) {

	this->mResource = new ZLGfxResourceGL ( type, glid, status );
	this->mResource->Retain ();
}

//----------------------------------------------------------------//
ZLGfxHandleGL::ZLGfxHandleGL ( const ZLGfxHandleGL& handle ) {
	
	this->mResource = handle.mResource;
	
	if ( this->mResource ) {
		this->mResource->Retain ();
	}
}

//----------------------------------------------------------------//
ZLGfxHandleGL::~ZLGfxHandleGL () {

	if ( this->mResource ) {
		this->mResource->Release ();
	}
}
