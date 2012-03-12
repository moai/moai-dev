// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIHttpTaskInfo_curl.h>
#include <moaicore/MOAIUrlMgr.h>

#ifdef _WIN32
	#include <algorithm>
#endif


//----------------------------------------------------------------//
void MOAIHttpTaskInfo::_printError ( CURLcode error ) {

	if ( error ) {
		USLog::Print ( "%s\n", curl_easy_strerror ( error ));
	}
}

//----------------------------------------------------------------//
u32 MOAIHttpTaskInfo::_writeData ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTaskInfo* self = ( MOAIHttpTaskInfo* )s;
	u32 size = n * l;
	
	self->mStream->WriteBytes ( data, size );
	return size;
}

//----------------------------------------------------------------//
u32 MOAIHttpTaskInfo::_writeHeader ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTaskInfo* self = ( MOAIHttpTaskInfo* )s;
	u32 size = n * l;
	
	char *endp = data + size;
	char *colon = data;
	while( colon < endp && *colon != ':' )
		colon++;
	if( colon < endp )
	{
		STLString name(data, colon - data);
		// Case insensitive
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		char *vstart = colon;
		vstart++;
		while( vstart < endp && isspace(*vstart) )
			vstart++;
		char *vend = endp - 1;
		while( vend > vstart && isspace(*vend) )
			vend--;
		STLString value(vstart, (vend - vstart) + 1);
		
		// Emulate XMLHTTPRequest.getResponseHeader() behavior of appending with comma
		// separator if there are multiple header responses?
		
		if( self->mResponseHeaders.find(name) != self->mResponseHeaders.end() )
			self->mResponseHeaders[name] = self->mResponseHeaders[name] + "," + value;
		else
			self->mResponseHeaders[name] = value;
	}
	
	// Shouldn't this be a case-insensitive check?
	STLString key = "content-length";
	u32 keyLength = ( u32 )strlen ( key );
	if ( strncmp ( data, key, keyLength ) == 0 ) {

		// NOTE: libcurl explicitly says to not assume zero termination here!
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

//----------------------------------------------------------------//
void MOAIHttpTaskInfo::Clear () {

	this->mUrl.clear ();
	this->mData.Clear ();
	
	this->mResponseCode = 0;
	this->mResponseHeaders.clear();
	
	if ( this->mBody ) {
		free ( this->mBody );
		this->mBody = 0;
	}
	
	if( mRequestHeaders )
	{
		curl_slist_free_all(mRequestHeaders);
		mRequestHeaders = 0;
	}
	
	if ( this->mEasyHandle ) {
		curl_easy_cleanup ( this->mEasyHandle );
		this->mEasyHandle = 0;
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskInfo::Finish () {

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
}

//----------------------------------------------------------------//
void MOAIHttpTaskInfo::InitRequestHeaders ( HeaderList* extraHeaders ) {
	if( mRequestHeaders ) {
		curl_slist_free_all(mRequestHeaders);
		mRequestHeaders = 0;
	}
	
	if( extraHeaders ) {
		HeaderList::iterator it = extraHeaders->begin();
		HeaderList::iterator end = extraHeaders->end();
		for( ; it != end; ++it ) {
			mRequestHeaders = curl_slist_append(mRequestHeaders, it->c_str());
		}
	}

	if(	mEasyHandle ) {
		curl_easy_setopt( mEasyHandle, CURLOPT_HTTPHEADER, mRequestHeaders );
	}
}


//----------------------------------------------------------------//
void MOAIHttpTaskInfo::InitForGet ( cc8* url, cc8* useragent, bool verbose, HeaderList* extraHeaders ) {

	this->Clear ();
	
	CURLcode result;
	CURL* easyHandle = curl_easy_init ();

	result = curl_easy_setopt ( easyHandle, CURLOPT_URL, url );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_HEADERFUNCTION, _writeHeader );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_HEADERDATA, this );
	_printError ( result );

	result = curl_easy_setopt ( easyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_WRITEDATA, this );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_FAILONERROR, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_NOPROGRESS, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_SSL_VERIFYPEER, 0 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_SSL_VERIFYHOST, 0 );
	_printError ( result );

	if ( useragent ) {
		result = curl_easy_setopt ( easyHandle, CURLOPT_USERAGENT, useragent );
		_printError ( result );
	}
	
	if ( verbose ) {
		result = curl_easy_setopt ( easyHandle, CURLOPT_VERBOSE, 1 );
		_printError ( result );
	}
	
	this->mEasyHandle = easyHandle;
	this->mUrl = url;
	this->InitRequestHeaders(extraHeaders);
}

//----------------------------------------------------------------//
void MOAIHttpTaskInfo::InitForPost ( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose, HeaderList* extraHeaders ) {

	this->Clear ();
	
	this->mBody = malloc ( size );
	memcpy ( this->mBody, buffer, size );
	
	CURLcode result;
	CURL* easyHandle = curl_easy_init ();

	result = curl_easy_setopt ( easyHandle, CURLOPT_URL, url );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_POSTFIELDS, this->mBody );
	_printError ( result );
	
    result = curl_easy_setopt ( easyHandle, CURLOPT_POSTFIELDSIZE, ( long )size );
    _printError ( result );

	/*
	curl_httppost* formpost = 0;
	curl_httppost* lastptr = 0;

	curl_formadd (
		&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "uploaded",
		//CURLFORM_CONTENTTYPE, "multipart/form-data",
		CURLFORM_FILENAME, filename,
		CURLFORM_COPYCONTENTS, buffer,
		CURLFORM_CONTENTSLENGTH, ( long )size,
		CURLFORM_END
	);

	curl_easy_setopt ( easyHandle, CURLOPT_URL, url );
	curl_easy_setopt ( easyHandle, CURLOPT_HTTPPOST, formpost );
	*/

	result = curl_easy_setopt ( easyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_WRITEDATA, this );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_FAILONERROR, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_NOPROGRESS, 1 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_SSL_VERIFYPEER, 0 );
	_printError ( result );
	
	result = curl_easy_setopt ( easyHandle, CURLOPT_SSL_VERIFYHOST, 0 );
	_printError ( result );
	
	if ( useragent ) {
		result = curl_easy_setopt ( easyHandle, CURLOPT_USERAGENT, useragent );
		_printError ( result );
	}
	
	if ( verbose ) {
		result = curl_easy_setopt ( easyHandle, CURLOPT_VERBOSE, 1 );
		_printError ( result );
	}
	
	this->mEasyHandle = easyHandle;
	this->mUrl = url;
	this->InitRequestHeaders(extraHeaders);
}

//----------------------------------------------------------------//
MOAIHttpTaskInfo::MOAIHttpTaskInfo () :
	mEasyHandle ( 0 ),
	mRequestHeaders(0),
	mBody ( 0 )
	{
	
	this->mStream = &this->mMemStream;
}

//----------------------------------------------------------------//
MOAIHttpTaskInfo::~MOAIHttpTaskInfo () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskInfo::PerformSync () {

	if ( this->mEasyHandle ) {
		curl_easy_perform ( this->mEasyHandle );
	}
}
