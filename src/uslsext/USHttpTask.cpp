// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USDataIOTask.h>
#include <uslsext/USHttpTask.h>
#include <uslsext/USHttpTask_impl.h>
#include <uslsext/USLog.h>
#include <uslsext/USUrlMgr.h>

//================================================================//
// USHttpTask
//================================================================//

//----------------------------------------------------------------//
void USHttpTask::Cancel () {

	this->Clear ();
	this->mCallback.Clear ();
}

//----------------------------------------------------------------//
void USHttpTask::Clear () {

	if ( this->mInfo ) {
		delete this->mInfo;
		this->mInfo = 0;
	}
	
	this->mBytes = 0;
	this->mSize = 0;
}

//----------------------------------------------------------------//
void USHttpTask::Finish () {

	this->mInfo->Finish ();
	
	this->mBytes = this->mInfo->mData;
	this->mSize = this->mInfo->mData.Size ();

	this->mCallback.Call ( this );

	this->Clear ();
	this->Release ();
}

//----------------------------------------------------------------//
void* USHttpTask::GetData () {

	return this->mBytes;
}

//----------------------------------------------------------------//
void USHttpTask::GetData ( void* buffer, u32 size ) {

	USByteStream byteStream;
	
	byteStream.SetBuffer ( this->mBytes, this->mSize );
	byteStream.SetLength ( this->mSize );
	byteStream.ReadBytes ( buffer, size );
}

//----------------------------------------------------------------//
u32 USHttpTask::GetSize () {

	return this->mSize;
}

//----------------------------------------------------------------//
void USHttpTask::HttpGet ( cc8* url ) {

	this->Clear ();
	this->mInfo = new USHttpTaskInfo ();
	this->mInfo->InitForGet ( url );
	
	this->Retain ();
	USUrlMgr::Get ().AddHandle ( *this );
}

//----------------------------------------------------------------//
void USHttpTask::HttpPost ( cc8* url, const void* buffer, u32 size ) {

	this->Clear ();
	this->mInfo = new USHttpTaskInfo ();
	this->mInfo->InitForPost ( url, buffer, size );
	
	this->Retain ();
	USUrlMgr::Get ().AddHandle ( *this );
}

//----------------------------------------------------------------//
USHttpTask::USHttpTask () :
	mInfo ( 0 ),
	mBytes ( 0 ),
	mSize ( 0 ) {
}

//----------------------------------------------------------------//
USHttpTask::USHttpTask ( const USHttpTask& task ) {
	UNUSED ( task );
	assert ( false ); // copy constructor not supported
}

//----------------------------------------------------------------//
USHttpTask::~USHttpTask () {

	this->Clear ();
}
