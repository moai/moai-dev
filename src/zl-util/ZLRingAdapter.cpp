// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLRingAdapter.h>

//================================================================//
// ZLHexAdapter
//================================================================//

//----------------------------------------------------------------//
size_t ZLRingAdapter::GetCursor () {

	return this->mCursor - this->mBase;
}

//----------------------------------------------------------------//
bool ZLRingAdapter::IsAtEnd () {

	return this->mLength <= 0;
}

//----------------------------------------------------------------//
size_t ZLRingAdapter::Process ( void* readBuffer, const void* writeBuffer, size_t size ) {

	if ( !( this->mProxiedStream && ( readBuffer || writeBuffer ))) return 0;
	
	size_t bytes = 0;
	
	while ( bytes < size ) {
	
		size_t available = this->mLength - this->mCursor;
		
		if ( available == 0 ) {
			
			if ( this->mProxiedStream->SetCursor ( this->mBase ) != 0 ) break;
			
			this->mCursor = 0;
			available = this->mLength;
		}
		
		size_t chunkSize = size <= available ? size : available;
	
		size_t result = 0;
		
		if ( readBuffer ) {
			result = this->mProxiedStream->ReadBytes ( readBuffer, chunkSize );
		}
		else {
			result = this->mProxiedStream->WriteBytes ( writeBuffer, chunkSize );
		}
		
		this->mCursor += result;
		bytes += result;
	
		if ( result < chunkSize ) break;
	}
	return bytes;
}

//----------------------------------------------------------------//
ZLSizeResult ZLRingAdapter::ReadBytes ( void* buffer, size_t size ) {
	
	return this->Process ( buffer, 0, size );
}

//----------------------------------------------------------------//
int ZLRingAdapter::SetCursor ( long offset ) {

	if ( this->mProxiedStream ) {
		offset = offset % this->mLength;
		this->mCursor = offset;
		this->mProxiedStream->SetCursor ( this->mBase + offset );
	}
}

//----------------------------------------------------------------//
ZLSizeResult ZLRingAdapter::SetLength ( size_t length ) {

	this->mLength = length;
}

//----------------------------------------------------------------//
ZLSizeResult ZLRingAdapter::WriteBytes ( const void* buffer, size_t size ) {
	
	return this->Process ( 0, buffer, size );
}

//----------------------------------------------------------------//
ZLRingAdapter::ZLRingAdapter () {
}

//----------------------------------------------------------------//
ZLRingAdapter::~ZLRingAdapter () {
}
