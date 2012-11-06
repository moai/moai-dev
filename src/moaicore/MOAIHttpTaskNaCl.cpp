// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTaskNaCl.h>
#include <moaicore/MOAIUrlMgrNaCl.h>

#define MAX_HEADER_LENGTH 1024

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_OS_NACL

#include "moai_nacl.h"

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::Clear () {
	this->mUrl.clear ();
	this->mData.Clear ();
	this->mReady = false;
	this->mResponseCode = 0;
	this->mResponseHeaders.clear();
}

//----------------------------------------------------------------//
bool MOAIHttpTaskNaCl::IsReady () {
	return mReady;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size ) {

	MOAIHttpTaskNaCl *taskInfo = static_cast < MOAIHttpTaskNaCl * > ( handler->GetUserData ());
	taskInfo->mResponseCode = handler->GetStatusCode ();

	//printf ( "MOAIHttpTaskNaCl::HttpLoaded status? %d, size %d, pointer %p, data %s\n", handler->GetStatusCode (), size, taskInfo, buffer );

	taskInfo->mStream->WriteBytes ( buffer, size );

	taskInfo->mReady = true;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::HttpGetMainThread ( void* userData, int32_t result ) {

	MOAIHttpTaskNaCl * taskInfo = static_cast < MOAIHttpTaskNaCl * > ( userData );

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
MOAIHttpTaskNaCl::MOAIHttpTaskNaCl () {

	this->mStream = &this->mMemStream;
}

//----------------------------------------------------------------//
MOAIHttpTaskNaCl::~MOAIHttpTaskNaCl () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::NaClFinish () {

	NACL_LOG ("MOAIHttpTaskNaCl::Finish %p\n", this );
	if ( this->mStream == &this->mMemStream ) {
	
		u32 size = this->mMemStream.GetLength ();
		NACL_LOG ("MOAIHttpTaskNaCl::Finish get size %d\n", size );
		
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
void MOAIHttpTaskNaCl::Prepare ( GetURLHandler *handler ) {

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
void MOAIHttpTaskNaCl::PerformAsync () {

	this->mReady = false;
	this->mLock = true;

	pp::CompletionCallback cc ( HttpGetMainThread, this );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( this->mLock ) {
		sleep ( 0.0001f );
	}

	MOAIUrlMgrNaCl::Get ().AddHandle ( *this );

}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::PerformSync () {

	this->mReady = false;
	this->mLock = true;

	pp::CompletionCallback cc ( HttpGetMainThread, this );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( !this->mReady ) {
		sleep ( 0.0001f );
	}

}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::Reset () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::SetBody ( const void* buffer, u32 size ) {

	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );
}

void MOAIHttpTaskNaCl::SetCookieDst ( const char *file ) {
	//unimplemented
}

void MOAIHttpTaskNaCl::SetCookieSrc	( const char *file ) {
	//unimplemented
}

void MOAIHttpTaskNaCl::SetFailOnError ( bool enable ) {
	//unimplemented
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::SetUrl ( cc8* url ) {
	
	this->mUrl = url;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::SetUserAgent ( cc8* useragent ) {
	
	//do nothing, user agent will be chrome
}

//----------------------------------------------------------------//
void MOAIHttpTaskNaCl::SetVerb ( u32 verb ) {
	
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
void MOAIHttpTaskNaCl::SetVerbose ( bool verbose ) {

	//TODO make it toggle logging on http manager
}

#endif
