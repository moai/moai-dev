// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "geturl_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "NaClFileSystem.h"
#include "moai_nacl.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace {
bool IsError(int32_t result) {
  return ((PP_OK != result) && (PP_OK_COMPLETIONPENDING != result));
}
}  // namespace

GetURLHandler* GetURLHandler::Create ( pp::Instance* instance, const std::string& url ) {
	return new GetURLHandler ( instance, url );
}

GetURLHandler::GetURLHandler(pp::Instance* instance, const std::string& url )
    : mInstance(instance),
      url_(url),
      url_request_ ( instance ),
      mUrlLoader ( instance ),
      cc_factory_(this) {

	mCallback = NULL;
	url_request_.SetURL( url );

	mMethod = GET;
	url_request_.SetMethod ( "GET" );

	//url_request_.SetAllowCrossOriginRequests ( true );
}

GetURLHandler::~GetURLHandler () {
}

void GetURLHandler::SetMethod ( int method ) {

	mMethod = method;

	if ( method == GET ) {
		url_request_.SetMethod ( "GET" );
	}
	else if ( method == HEAD ) {
		url_request_.SetMethod ( "HEAD" );
	}
	else if ( method == POST ) {
		url_request_.SetMethod ( "POST" );
		url_request_.SetHeaders ( "Content-Type: application/x-www-form-urlencoded" );
	}
	else if ( method == PUT ) {
		url_request_.SetMethod ( "PUT" );
	}
	else if ( method == DELETE ) {
		url_request_.SetMethod ( "DELETE" );
	}
}
void GetURLHandler::SetBody ( const void *data, int size ) {

	url_request_.AppendDataToBody ( data, size );
}

void GetURLHandler::SetHeaders ( const char *string ) {
	url_request_.SetHeaders ( string );
}

void GetURLHandler::SetUserData ( void *userData ) {

	mUserData = userData;
}

bool GetURLHandler::Start ( GetURLCallback callback ) {

	mCallback = callback;

	pp::CompletionCallback cc = cc_factory_.NewCallback ( &GetURLHandler::OnOpen );

	int32_t res = mUrlLoader.Open ( url_request_, cc );

	if ( PP_OK_COMPLETIONPENDING != res ) {
		cc.Run ( res );
	}

	return !IsError ( res );
}

void GetURLHandler::OnOpen ( int32_t result ) {

	if ( result < 0 ) {
		printf ( "ERROR: GetURLHandler::OnOpen %d, %s\n", result, url_.c_str ());
		ReportResultAndDie ( url_, "pp::URLLoader::Open() failed", false );
	}
	else {

		//get info about file TODO: get size,exist, other file stat
		mHttpStatusCode = mUrlLoader.GetResponseInfo ().GetStatusCode ();

		if ( mMethod == GET || mMethod == POST ) {
			ReadBody ();
		}
		else {
			ReportResultAndDie (  url_, url_response_body_, true );
		}

	}
}

void GetURLHandler::OnRead ( int32_t result ) {

	if ( result < 0 ) {
		ReportResultAndDie (url_, "pp::URLLoader::ReadResponseBody() result<0", false );
	} 
	else if ( result != 0 ) {
    
		int32_t num_bytes = result < kBufferSize ? result : sizeof ( buffer_ );
    
		url_response_body_.reserve ( url_response_body_.size () + num_bytes );
		url_response_body_.insert ( url_response_body_.end(), buffer_, buffer_ + num_bytes );

		ReadBody();
	} 
	else {  // result == 0, end of stream
		ReportResultAndDie ( url_, url_response_body_, true );
	}
}

void GetURLHandler::ReadBody () {

	pp::CompletionCallback cc = cc_factory_.NewCallback ( &GetURLHandler::OnRead );

	int32_t res = mUrlLoader.ReadResponseBody ( buffer_, sizeof ( buffer_ ), cc );

	if ( PP_OK_COMPLETIONPENDING != res ) {
		cc.Run ( res );
	}
}

void GetURLHandler::ReportResultAndDie ( const std::string& fname,
                                         const std::string& text,
                                         bool success ) {
	ReportResult ( fname, text, success );
	delete this;
}

void GetURLHandler::ReportResult(const std::string& fname,
                                 const std::string& text,
                                 bool success) {
	if ( success ) {
		//printf( "GetURLHandler::ReportResult(Ok).\n" );
	}
	else {
		NACL_LOG( "GetURLHandler::ReportResult(Err). %s\n", text.c_str ());
	}

	if ( mInstance ) {

		mCallback ( this, text.c_str (), text.length ());
	}
}

