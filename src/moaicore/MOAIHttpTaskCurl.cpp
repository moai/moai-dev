// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <algorithm>
#include <moaicore/MOAIHttpTaskCurl.h>
#include <moaicore/MOAIUrlMgrCurl.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef USE_CURL

#define MAX_HEADER_LENGTH 1024

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
u32 MOAIHttpTaskCurl::_writeData ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTaskCurl* self = ( MOAIHttpTaskCurl* )s;
	u32 size = n * l;
	
	self->mStream->WriteBytes ( data, size );
	return size;
}

//----------------------------------------------------------------//
u32 MOAIHttpTaskCurl::_writeHeader ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTaskCurl* self = ( MOAIHttpTaskCurl* )s;
	u32 size = n * l;
	   	
	char *endp = data + size;
	char *colon = data;
	while ( colon < endp && *colon != ':' ) {
		colon++;
	}

	if ( colon < endp )
	{
		STLString name ( data, colon - data );
		// Case insensitive

		char *vstart = colon;
		vstart++;
		while( vstart < endp && isspace ( *vstart )) {
		  vstart++;
		}
		char *vend = endp - 1;
		while( vend > vstart && isspace ( *vend ) ) {
		  vend--;
		}
		STLString value(vstart, ( vend - vstart ) + 1);

		// Emulate XMLHTTPRequest.getResponseHeader() behavior of appending with comma
		// separator if there are multiple header responses?

		if( self->mResponseHeaders.find ( name ) != self->mResponseHeaders.end ())	{
		  self->mResponseHeaders [ name ] = self->mResponseHeaders [ name ] + "," + value;
		}
		else {
		  self->mResponseHeaders [ name ] = value;
		}
	}

	// Shouldn't this be a case-insensitive check?

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

u32 MOAIHttpTaskCurl::_progressFunction ( void* ptr, double totalDownload, double downloaded, double totalUpload, double uploaded ) {
	
	UNUSED ( uploaded );
	UNUSED ( totalUpload );
	
	MOAIHttpTaskCurl *self = ( MOAIHttpTaskCurl * ) ptr;
	
	if ( totalDownload ) {
		self->mProgress = ( float ) ( downloaded / totalDownload );
	}
	
	if ( self->mProgress > 1.0f ) {
		self->mProgress = 1.0f;
	}
	
	if ( self->mProgress < 0.0f ) {
		self->mProgress = 0.0f;
	}
	
	return 0;
}

//================================================================//
// MOAIHttpTaskCurl
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::AffirmHandle () {

	if ( this->mEasyHandle ) return;
	
	CURLcode result;
	
	this->mEasyHandle = curl_easy_init ();
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERFUNCTION, _writeHeader );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERDATA, this );
	PrintError ( result );

	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEDATA, this );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FAILONERROR, 1 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOPROGRESS, 0 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_PROGRESSDATA, this );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_PROGRESSFUNCTION, _progressFunction );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYPEER, 0 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYHOST, 0 );
	PrintError ( result );

#ifdef USE_ARES
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOSIGNAL, 1 );
	PrintError ( result );
#endif
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::Clear () {

	this->mUrl.clear ();
	this->mBody.Clear ();
	this->mMemStream.Clear ();
	this->mData.Clear ();
	this->mResponseHeaders.clear();
	
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
void MOAIHttpTaskCurl::CurlFinish () {

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
MOAIHttpTaskCurl::MOAIHttpTaskCurl () :
	mDefaultTimeout ( 10 ),
	mEasyHandle ( 0 ),
	mHeaderList ( 0 ),
	mStream ( 0 ) {

	RTTI_SINGLE ( MOAIHttpTaskBase )
	
	this->Reset ();
}

//----------------------------------------------------------------//
MOAIHttpTaskCurl::~MOAIHttpTaskCurl () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::Prepare () {

	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// USMemStream which will grow dynamically
	if ( this->mUserStream ) {
		
		this->mStream = this->mUserStream->GetUSStream();
	}
	else {
		
		this->mStream = &this->mMemStream;	
	}

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
		PrintError ( result );
	}

	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CONNECTTIMEOUT, this->mDefaultTimeout );
	
	// follow redirects based on settings in base class (default is to NOT follow redirects)
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FOLLOWLOCATION, this->mFollowRedirects );
	
	// set the timeout for this task
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_TIMEOUT, this->mTimeout );
	
	PrintError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::PerformAsync () {

	if ( this->mEasyHandle ) {
		this->Prepare ();
		MOAIUrlMgrCurl::Get ().AddHandle ( *this );
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::PerformSync () {

	if ( this->mEasyHandle ) {
		this->Prepare ();
		curl_easy_perform ( this->mEasyHandle );
		this->CurlFinish ();
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::PrintError ( CURLcode error ) {

	if ( error ) {
		USLog::Print ( "%s\n", curl_easy_strerror ( error ));
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::Reset () {

	this->Clear ();
	this->AffirmHandle ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetBody ( const void* buffer, u32 size ) {

	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );

	CURLcode result;

	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDS, this->mBody.Data ());
	PrintError ( result );
	
    result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDSIZE, ( long )size );
    PrintError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetCookieDst	( const char *file ) {
	
	CURLcode result = curl_easy_setopt( this->mEasyHandle, CURLOPT_COOKIEFILE, file );
	PrintError ( result );

}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetCookieSrc	( const char *file ) {
	CURLcode result = curl_easy_setopt( this->mEasyHandle, CURLOPT_COOKIEJAR, file );
	PrintError ( result );
}

void MOAIHttpTaskCurl::SetFailOnError ( bool enable ) {
	
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FAILONERROR, ( u32 ) enable );
	PrintError ( result );
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetUrl ( cc8* url ) {

	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_URL, url );
	PrintError ( result );
	
	this->mUrl = url;
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetUserAgent ( cc8* useragent ) {
	
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_USERAGENT, useragent );
	PrintError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetVerb ( u32 verb ) {

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
	
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOBODY, verb == HTTP_HEAD ? 1 : 0 );
	PrintError ( result );
}

//----------------------------------------------------------------//
void MOAIHttpTaskCurl::SetVerbose ( bool verbose ) {

	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_VERBOSE, verbose ? 1 : 0 );
	PrintError ( result );
}

#endif
