// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USData.h>
#include <uslsext/USHttpTask.h>
#include <uslsext/USHttpTask_impl.h>
#include <uslsext/USUrlMgr.h>

//----------------------------------------------------------------//
u32 USHttpTaskInfo::_writeData ( char* data, u32 n, u32 l, void* s ) {
	
	USHttpTaskInfo* self = ( USHttpTaskInfo* )s;
	u32 size = n * l;
	
	self->mStream->WriteBytes ( data, size );
	return size;
}

//----------------------------------------------------------------//
u32 USHttpTaskInfo::_writeHeader ( char* data, u32 n, u32 l, void* s ) {
	
	USHttpTaskInfo* self = ( USHttpTaskInfo* )s;
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

//----------------------------------------------------------------//
void USHttpTaskInfo::Clear () {

	this->mUrl.clear ();
	this->mData.Clear ();
	
	if ( this->mEasyHandle ) {
		curl_easy_cleanup ( this->mEasyHandle );
		this->mEasyHandle = 0;
	}
}

//----------------------------------------------------------------//
void USHttpTaskInfo::Finish () {

	if ( this->mStream == &this->mMemStream ) {
	
		u32 size = this->mMemStream.GetLength ();
		this->mData.Init ( size );
	
		this->mStream->Seek ( 0, SEEK_SET );
		this->mStream->ReadBytes ( this->mData, size );
		
		this->mMemStream.Clear ();
	}
}

//----------------------------------------------------------------//
void USHttpTaskInfo::InitForGet ( cc8* url ) {

	this->Clear ();
	
	CURL* easyHandle = curl_easy_init ();

	curl_easy_setopt ( easyHandle, CURLOPT_URL, url );

	curl_easy_setopt ( easyHandle, CURLOPT_HEADERFUNCTION, _writeHeader );
	curl_easy_setopt ( easyHandle, CURLOPT_HEADERDATA, this );

	curl_easy_setopt ( easyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	curl_easy_setopt ( easyHandle, CURLOPT_WRITEDATA, this );
	
	curl_easy_setopt ( easyHandle, CURLOPT_FAILONERROR, 1 );
	curl_easy_setopt ( easyHandle, CURLOPT_NOPROGRESS, 1 );
	
	this->mEasyHandle = easyHandle;
	this->mUrl = url;
}

//----------------------------------------------------------------//
void USHttpTaskInfo::InitForPost ( cc8* url, const void* buffer, u32 size ) {

	this->Clear ();
	
	CURL* easyHandle = curl_easy_init ();

	curl_easy_setopt ( easyHandle, CURLOPT_URL, url );
	curl_easy_setopt ( easyHandle, CURLOPT_POSTFIELDS, buffer );
    curl_easy_setopt ( easyHandle, CURLOPT_POSTFIELDSIZE, ( long )size );

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

	curl_easy_setopt ( easyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	curl_easy_setopt ( easyHandle, CURLOPT_WRITEDATA, this );
	
	curl_easy_setopt ( easyHandle, CURLOPT_FAILONERROR, 1 );
	curl_easy_setopt ( easyHandle, CURLOPT_NOPROGRESS, 1 );
	
	this->mEasyHandle = easyHandle;
	this->mUrl = url;
}

//----------------------------------------------------------------//
USHttpTaskInfo::USHttpTaskInfo () :
	mEasyHandle ( 0 ) {
	
	this->mStream = &this->mMemStream;
}

//----------------------------------------------------------------//
USHttpTaskInfo::~USHttpTaskInfo () {

	this->Clear ();
}
