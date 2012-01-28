// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTaskBase.h>
#include <moaicore/MOAIHttpTask_curl.h>
#include <moaicore/MOAIUrlMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef USE_CURL

#define MAX_HEADER_LENGTH 1024

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTask::_printError ( CURLcode error ) {

	if ( error ) {
		USLog::Print ( "%s\n", curl_easy_strerror ( error ));
	}
}

//----------------------------------------------------------------//
u32 MOAIHttpTask::_writeData ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTask* self = ( MOAIHttpTask* )s;
	u32 size = n * l;
	
	self->mStream->WriteBytes ( data, size );
	return size;
}

//----------------------------------------------------------------//
u32 MOAIHttpTask::_writeHeader ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTask* self = ( MOAIHttpTask* )s;
	u32 size = n * l;
	
	STLString key = "content-length";
	u32 keyLength = ( u32 )strlen ( key );
	if ( strncmp ( data, key, keyLength ) == 0 ) {
	
		STLString header = data;
		u32 end = ( u32 )header.find_last_of ( '\n' );
		STLString value = header.clip ( keyLength + 2, end - 1 );

		u32 length = atoi ( value );
		if ( length ) {
			
			self->mData.Init ( length );
			self->mByteStream.SetBuffer ( self->mData, length );
			self->mByteStream.SetLength ( length );
			self->mStream = &self->mByteStream;
		}
	}
	return size;
}

//================================================================//
// MOAIHttpTask
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTask::AffirmHandle () {

	if ( this->mEasyHandle ) return;
	
	CURLcode result;
	
	this->mEasyHandle = curl_easy_init ();
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERFUNCTION, _writeHeader );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERDATA, this );
	_printError ( result );

	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEDATA, this );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FAILONERROR, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOPROGRESS, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYPEER, 0 );
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYHOST, 0 );
	_printError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTask::Clear () {

	this->mUrl.clear ();
	this->mBody.Clear ();
	this->mMemStream.Clear ();
	this->mData.Clear ();
	
	this->mResponseCode = 0;
	this->mStream = 0;
	
	if ( this->mEasyHandle ) {
		curl_easy_cleanup ( this->mEasyHandle );
		this->mEasyHandle = 0;
	}
	
	if ( this->mHeaderList ) {
		curl_slist_free_all ( this->mHeaderList );
		this->mHeaderList = 0;
	}
}

//----------------------------------------------------------------//
void MOAIHttpTask::CurlFinish () {

	if ( this->mEasyHandle ) {
		long response;
		curl_easy_getinfo ( this->mEasyHandle, CURLINFO_RESPONSE_CODE, &response );
		this->mResponseCode = ( u32 )response;
	}

	if ( this->mStream == &this->mMemStream ) {
	
		u32 size = this->mMemStream.GetLength ();
		
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
MOAIHttpTask::MOAIHttpTask () :
	mEasyHandle ( 0 ),
	mHeaderList ( 0 ),
	mStream ( 0 ) {
	
	RTTI_SINGLE ( MOAIHttpTaskBase )
	
	this->Reset ();
}

//----------------------------------------------------------------//
MOAIHttpTask::~MOAIHttpTask () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::Prepare () {

	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// USMemStream which will grow dynamically
	this->mStream = &this->mMemStream;

	char buffer [ MAX_HEADER_LENGTH ];

	// prepare the custom headers (if any)
	HeaderMapIt headerMapIt = this->mHeaderMap.begin ();
	for ( ; headerMapIt != this->mHeaderMap.end (); ++headerMapIt ) {
	
		STLString key = headerMapIt->first;
		STLString value = headerMapIt->second;
	
		assert (( key.size () + value.size () + 3 ) < MAX_HEADER_LENGTH );
	
		if ( value.size ()) {
			sprintf ( buffer, "%s: %s", key.c_str (), value.c_str ());
		}
		else {
			sprintf ( buffer, "%s:", key.c_str ());
		}
		
		this->mHeaderList = curl_slist_append ( this->mHeaderList, buffer );
	}
	
	if ( this->mHeaderList ) {
		CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HTTPHEADER, this->mHeaderList );
		_printError ( result );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTask::PerformAsync () {

	if ( this->mEasyHandle ) {
		this->Prepare ();
		MOAIUrlMgr::Get ().AddHandle ( *this );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTask::PerformSync () {

	if ( this->mEasyHandle ) {
		this->Prepare ();
		curl_easy_perform ( this->mEasyHandle );
		this->CurlFinish ();
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
	this->AffirmHandle ();
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetBody ( const void* buffer, u32 size ) {

	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );

	CURLcode result;

	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDS, this->mBody );
	_printError ( result );
	
    result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDSIZE, ( long )size );
    _printError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetUrl ( cc8* url ) {

	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_URL, url );
	_printError ( result );
	
	this->mUrl = url;
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetUserAgent ( cc8* useragent ) {
	
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_USERAGENT, useragent );
	_printError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetVerb ( u32 verb ) {

	CURLcode result = CURLE_OK;
	
	switch ( verb ) {
	
		case HTTP_GET:
			result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "GET" );
			break;
		
		case HTTP_HEAD:
			result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "HEAD" );
			break;
		
		case HTTP_POST:
			result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "POST" );
			break;
		
		case HTTP_PUT:
			result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "PUT" );
			break;
		
		case HTTP_DELETE:
			result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "DELETE" );
			break;
	}
	
	_printError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOBODY, verb == HTTP_HEAD ? 1 : 0 );
	_printError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTask::SetVerbose ( bool verbose ) {

	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_VERBOSE, verbose ? 1 : 0 );
	_printError ( result );
}

#endif