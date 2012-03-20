// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

//#include <uslsext/USData.h>
#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIHttpTaskInfo_nacl.h>
#include <moaicore/MOAIUrlMgr.h>

#define MAX_HEADER_LENGTH 1024

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_OS_NACL

#include "moai_nacl.h"
//----------------------------------------------------------------//
void MOAIHttpTask::HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size ) {

	MOAIHttpTask *taskInfo = static_cast < MOAIHttpTask * > ( handler->GetUserData ());
	taskInfo->mResponseCode = handler->GetStatusCode ();

	//printf ( "MOAIHttpTask::HttpLoaded status? %d, size %d, pointer %p, data %s\n", handler->GetStatusCode (), size, taskInfo, buffer );

	taskInfo->mStream->WriteBytes ( buffer, size );

	taskInfo->mReady = true;
}

//----------------------------------------------------------------//
void MOAIHttpTask::HttpGetMainThread ( void* userData, int32_t result ) {

	MOAIHttpTask * taskInfo = static_cast < MOAIHttpTask * > ( userData );

	GetURLHandler* handler = GetURLHandler::Create( g_instance, taskInfo->mUrl );
	
	if ( handler != NULL ) {

		handler->SetMethod ( taskInfo->mMethod );
		handler->SetUserData ( taskInfo );

		if ( taskInfo->mBody.Size () > 0 ) {
			NACL_LOG ( "\nSet Body: %s\n", taskInfo->mBody.Data ());
			handler->SetBody ( taskInfo->mBody.Data (), taskInfo->mBody.Size ());
		}

		taskInfo->Prepare ( handler );

		handler->Start( HttpLoaded );
	}

	taskInfo->mLock = false;
}

//----------------------------------------------------------------//
void MOAIHttpTask::Clear () {
	this->mUrl.clear ();
	this->mData.Clear ();
	this->mReady = false;
	this->mResponseCode = 0;
}

//----------------------------------------------------------------//
void MOAIHttpTask::NaClFinish () {

	NACL_LOG ("MOAIHttpTask::Finish %p\n", this );
	if ( this->mStream == &this->mMemStream ) {
	
		u32 size = this->mMemStream.GetLength ();
		NACL_LOG ("MOAIHttpTask::Finish get size %d\n", size );
		
		if ( size ) {
			this->mData.Init ( size );
			this->mStream->Seek ( 0, SEEK_SET );
			this->mStream->ReadBytes ( this->mData, size );
		}
		this->mMemStream.Clear ();
	}

	this->Finish ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::Prepare ( GetURLHandler *handler ) {

	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// USMemStream which will grow dynamically
	this->mStream = &this->mMemStream;

	char buffer [ MAX_HEADER_LENGTH ];

	int written = 0;

	// prepare the custom headers (if any)
	HeaderMapIt headerMapIt = this->mHeaderMap.begin ();
	for ( ; headerMapIt != this->mHeaderMap.end (); ++headerMapIt ) {
	
		STLString key = headerMapIt->first;
		STLString value = headerMapIt->second;
	
		assert (( written + ( key.size () + value.size () + 3 )) < MAX_HEADER_LENGTH );
	
		if ( value.size ()) {
			written += sprintf ( buffer + written, "%s: %s\n", key.c_str (), value.c_str ());
		}
		else {
			written += sprintf ( buffer + written, "%s:\n", key.c_str ());
		}
		
	}

	//append headers
	handler->SetHeaders ( buffer );

}
//----------------------------------------------------------------//
MOAIHttpTask::MOAIHttpTask () {

	this->mStream = &this->mMemStream;
}

//----------------------------------------------------------------//
MOAIHttpTask::~MOAIHttpTask () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::PerformAsync () {

	this->mReady = false;
	this->mLock = true;

	pp::CompletionCallback cc ( HttpGetMainThread, this );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( this->mLock ) {
		sleep ( 0.0001f );
	}

	MOAIUrlMgr::Get ().AddHandle ( *this );

}

//----------------------------------------------------------------//
void MOAIHttpTask::PerformSync () {

	this->mReady = false;
	this->mLock = true;

	pp::CompletionCallback cc ( HttpGetMainThread, this );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( !this->mReady ) {
		sleep ( 0.0001f );
	}

}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTask::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTask::Reset () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetBody ( const void* buffer, u32 size ) {

	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetUrl ( cc8* url ) {
	
	this->mUrl = url;
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetUserAgent ( cc8* useragent ) {
	
	//do nothing, user agent will be chrome
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetVerb ( u32 verb ) {
	
	switch ( verb ) {
	
		case HTTP_GET:
			mMethod = GetURLHandler::GET;
			break;
		
		case HTTP_HEAD:
			mMethod = GetURLHandler::HEAD;
			break;
		
		case HTTP_POST:
			mMethod = GetURLHandler::POST;
			break;
		
		case HTTP_PUT:
			mMethod = GetURLHandler::PUT;
			break;
		
		case HTTP_DELETE:
			mMethod = GetURLHandler::DELETE;
			break;
	}
	
	//set on http task
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetVerbose ( bool verbose ) {

	//TODO make it toggle logging on http manager
}
#endif
