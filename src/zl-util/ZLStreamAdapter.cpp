// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLStreamProxy
//================================================================//

//----------------------------------------------------------------//
void ZLStreamProxy::Flush () {
	if ( this->mProxiedStream ) {
		this->mProxiedStream->Flush ();
	}
}

//----------------------------------------------------------------//
u32 ZLStreamProxy::GetCaps () {

	return this->mProxiedStream ? this->mProxiedStream->GetCaps () : 0;
}

//----------------------------------------------------------------//
size_t ZLStreamProxy::GetCursor () {

	return this->mProxiedStream ? this->mProxiedStream->GetCursor () : 0;
}

//----------------------------------------------------------------//
size_t ZLStreamProxy::GetLength () {

	return this->mProxiedStream ? this->mProxiedStream->GetLength () : 0;
}

//----------------------------------------------------------------//
bool ZLStreamProxy::IsAtEnd () {

	return this->mProxiedStream ? this->mProxiedStream->GetLength () : true;
}

//----------------------------------------------------------------//
size_t ZLStreamProxy::ReadBytes ( void* buffer, size_t size ) {

	return this->mProxiedStream ? this->mProxiedStream->ReadBytes ( buffer, size ) : 0;
}

//----------------------------------------------------------------//
int ZLStreamProxy::SetCursor ( long offset ) {

	return this->mProxiedStream ? this->mProxiedStream->SetCursor ( offset ) : -1;
}

//----------------------------------------------------------------//
size_t ZLStreamProxy::WriteBytes ( const void* buffer, size_t size ) {

	return this->mProxiedStream ? this->mProxiedStream->WriteBytes ( buffer, size ) : 0;
}

//----------------------------------------------------------------//
ZLStreamProxy::ZLStreamProxy () :
	mProxiedStream ( 0 ) {
}

//----------------------------------------------------------------//
ZLStreamProxy::~ZLStreamProxy () {
}

//================================================================//
// ZLStreamAdapter
//================================================================//

//----------------------------------------------------------------//
void ZLStreamAdapter::Close () {
	
	if ( this->mIsOpen ) {
		this->OnClose ();
	}
	this->mProxiedStream = 0;
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

	this->mProxiedStream = stream;
	
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
	mBase ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ),
	mIsOpen ( false ) {
}

//----------------------------------------------------------------//
ZLStreamAdapter::~ZLStreamAdapter () {

	this->Close ();
}
