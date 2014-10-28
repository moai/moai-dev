// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLStreamAdapter
//================================================================//

//----------------------------------------------------------------//
void ZLStreamAdapter::Close () {
	
	if ( this->mIsOpen ) {
		this->OnClose ();
	}
	this->mStream = 0;
	this->mBase = 0;
	this->mCursor = 0;
	this->mLength = 0;
	this->mIsOpen = false;
	
}

//----------------------------------------------------------------//
size_t ZLStreamAdapter::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLStreamAdapter::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
bool ZLStreamAdapter::IsAtEnd () {

	return this->mLength && ( this->mCursor >= this->mLength ? 1 : 0 );
}

//----------------------------------------------------------------//
void ZLStreamAdapter::OnClose () {
}

//----------------------------------------------------------------//
bool ZLStreamAdapter::OnOpen () {

	return true;
}

//----------------------------------------------------------------//
bool ZLStreamAdapter::Open ( ZLStream* stream ) {

	this->Close ();

	this->mStream = stream;
	
	if ( stream ) {
		this->mBase = stream->GetCursor ();
	}
	
	this->mIsOpen = this->OnOpen ();
	if ( !this->mIsOpen ) {
		this->Close ();
	}
	return this->mIsOpen;
}

//----------------------------------------------------------------//
ZLStreamAdapter::ZLStreamAdapter () :
	mStream ( 0 ),
	mBase ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ),
	mIsOpen ( false ) {
}

//----------------------------------------------------------------//
ZLStreamAdapter::~ZLStreamAdapter () {

	this->Close ();
}
