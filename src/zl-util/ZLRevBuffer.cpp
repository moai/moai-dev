// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLRevBuffer.h>

//================================================================//
// ZLRevBufferHandle
//================================================================//
	
//----------------------------------------------------------------//
void ZLRevBufferHandle::Release () {

	if ( this->mRefCount > 0 ) {
		--this->mRefCount;
	}
	
	if ( this->mRefCount == 0 ) {
		delete ( this );
	}
}

//----------------------------------------------------------------//
void ZLRevBufferHandle::Retain () {

	this->mRefCount++;
}

//================================================================//
// ZLRevBufferEdition
//================================================================//

//----------------------------------------------------------------//
void ZLRevBufferEdition::Init ( ZLRevBufferHandle& handle, size_t size ) {

	this->mHandle = &handle;
	this->mHandle->Retain ();

	this->mSize = size;
	this->mData = malloc ( size );
}

//----------------------------------------------------------------//
void ZLRevBufferEdition::Release () {

	if ( this->mRefCount > 0 ) {
		--this->mRefCount;
	}
	
	if ( this->mRefCount == 0 ) {
		if ( this->mHandle->mBuffer ) {
			this->mHandle->mBuffer->Reuse ( this );
		}
	}
	
	if ( this->mRefCount == 0 ) {
		this->mHandle->Release ();
		delete ( this );
	}
}

//----------------------------------------------------------------//
void ZLRevBufferEdition::Retain () {

	this->mRefCount++;
}

//----------------------------------------------------------------//
ZLRevBufferEdition::ZLRevBufferEdition () :
	mRefCount ( 0 ),
	mData ( 0 ),
	mHandle ( 0 ),
	mSize ( 0 ) {
}

//----------------------------------------------------------------//
ZLRevBufferEdition::~ZLRevBufferEdition () {
}

//================================================================//
// ZLRevBuffer
//================================================================//

//----------------------------------------------------------------//
void ZLRevBuffer::Clear () {

	this->mSize = 0;
	
	size_t size = this->mCache.Size ();
	for ( size_t i = 0; i < size; ++i ) {
		this->mCache [ i ]->Release ();
	}
	this->mCache.Clear ();
	
	if ( this->mCurrentEdition ) {
		this->mCurrentEdition->Release ();
	}
	this->mCurrentEdition = 0;
}

//----------------------------------------------------------------//
ZLRevBufferEdition* ZLRevBuffer::GetCurrentEdition () {

	if ( this->mCurrentEdition ) {
		this->mLocked = true;
		this->mCurrentEdition->Retain ();
		return this->mCurrentEdition;
	}
	return 0;
}

//----------------------------------------------------------------//
const void* ZLRevBuffer::GetImmutableData () {

	return this->mCurrentEdition ? this->mCurrentEdition->GetImmutableData () : 0;
}

//----------------------------------------------------------------//
void* ZLRevBuffer::GetMutableData () {

	if ( this->mSize ) {
	
		if ( this->mLocked ) {
		
			assert ( this->mCurrentEdition );
		
			this->mLocked = false;
			this->mCurrentEdition->Release ();
			this->mCurrentEdition = 0;
		}
		
		if ( !this->mCurrentEdition ) {
			this->mCurrentEdition = new ZLRevBufferEdition ();
			this->mCurrentEdition->Init ( *this->mHandle, this->mSize );
			this->mCurrentEdition->Release ();
		}
		
		return this->mCurrentEdition->GetMutableData ();
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLRevBuffer::Init ( size_t size ) {

	this->Clear ();
	this->mSize = size;
}

//----------------------------------------------------------------//
void ZLRevBuffer::Reuse ( ZLRevBufferEdition* edition ) {

	if ( edition && ( edition->mSize == this->mSize )) {
	
		edition->Retain ();
		this->mCache.Push ( edition );
	}
}

//----------------------------------------------------------------//
ZLRevBuffer::ZLRevBuffer () :
	mSize ( 0 ),
	mHandle ( 0 ),
	mLocked ( false ),
	mCurrentEdition ( 0 ) {
	
	this->mHandle = new ZLRevBufferHandle ();
	this->mHandle->mBuffer = this;
	this->mHandle->Retain ();
}

//----------------------------------------------------------------//
ZLRevBuffer::~ZLRevBuffer () {

	this->Clear ();
	
	if ( this->mCurrentEdition ) {
		this->mCurrentEdition->Release ();
		this->mCurrentEdition = 0;
	}
	this->mHandle->mBuffer = 0;
	this->mHandle->Release ();
}
