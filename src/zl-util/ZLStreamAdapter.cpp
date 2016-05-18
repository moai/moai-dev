// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLStreamProxy
//================================================================//

//----------------------------------------------------------------//
void ZLStreamProxy::Compact () {
	if ( this->mProxiedStream ) {
		this->mProxiedStream->Compact ();
	}
}

//----------------------------------------------------------------//
void ZLStreamProxy::Flush () {
	if ( this->mProxiedStream ) {
		this->mProxiedStream->Flush ();
	}
}

//----------------------------------------------------------------//
u32 ZLStreamProxy::GetCaps () {

	return this->mProxiedStream ? this->mProxiedStream->GetCaps () : CAN_WRITE;
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

	return this->mProxiedStream ? this->mProxiedStream->GetLength () > 0 : true; // TODO: does this make sense?
}

//----------------------------------------------------------------//
ZLSizeResult ZLStreamProxy::ReadBytes ( void* buffer, size_t size ) {

	if ( this->mProxiedStream ) return this->mProxiedStream->ReadBytes ( buffer, size );
	ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );
}

//----------------------------------------------------------------//
ZLResultCode ZLStreamProxy::SetCursor ( size_t offset ) {

	return this->mProxiedStream ? this->mProxiedStream->SetCursor ( offset ) : ZL_ERROR;
}

//----------------------------------------------------------------//
ZLSizeResult ZLStreamProxy::SetLength ( size_t length ) {

	if ( this->mProxiedStream ) return this->mProxiedStream->SetLength ( length );
	ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStreamProxy::WriteBytes ( const void* buffer, size_t size ) {

	if ( this->mProxiedStream ) return this->mProxiedStream->WriteBytes ( buffer, size );
	ZL_RETURN_SIZE_RESULT ( size, ZL_OK ); // this is deliberate; an open proxy to an empty stream should just throw writes away
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
ZLResultCode ZLStreamAdapter::OnOpen () {

	return ZL_OK;
}

//----------------------------------------------------------------//
ZLResultCode ZLStreamAdapter::Open ( ZLStream* stream ) {

	this->Close ();

	this->mProxiedStream = stream;
	
	if ( stream ) {
		this->mBase = stream->GetCursor ();
	}
	
	this->mIsOpen = ( this->OnOpen () == ZL_OK );
	if ( !this->mIsOpen ) {
		this->Close ();
	}
	return this->mIsOpen ? ZL_OK : ZL_ERROR;
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
