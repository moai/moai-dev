// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_GETURL_GETURL_HANDLER_H_
#define EXAMPLES_GETURL_GETURL_HANDLER_H_

#include <string>
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_response_info.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/cpp/instance.h"


// GetURLHandler is used to download data from |url|. When download is
// finished or when an error occurs, it posts a message back to the browser
// with the results encoded in the message as a string and self-destroys.
//
// EXAMPLE USAGE:
// GetURLHandler* handler* = GetURLHandler::Create(instance,url);
// handler->Start();

class GetURLHandler;

typedef void (* GetURLCallback )( GetURLHandler *handler, const char *buffer, int32_t size );

class GetURLHandler {
 public:

	 enum {
		 GET,
		 HEAD,
		 POST,
		 PUT,
		 DELETE
	 };

	static GetURLHandler* Create ( pp::Instance* instance_, const std::string& url );

	bool Start ( GetURLCallback callback );

	void SetMethod ( int method );
	void SetUserData ( void *userData );

	int GetStatusCode () { return mHttpStatusCode; }
	void *GetUserData () { return mUserData; }

	void SetBody ( const void *data, int size );
	void SetHeaders ( const char *string );

 private:
	static const int kBufferSize = 4096;

	GetURLHandler ( pp::Instance* instance_, const std::string& url );
	~GetURLHandler ();

	void OnOpen ( int32_t result );
	void OnRead ( int32_t result );
	void ReadBody ();
	void ReportResult ( const std::string& fname,
					const std::string& text,
					bool success );
	void ReportResultAndDie ( const std::string& fname,
							const std::string& text,
							bool success );

	GetURLCallback mCallback;

	int mHttpStatusCode;
	void * mUserData;

	pp::Instance* mInstance;  // Weak pointer.

	std::string url_;  // URL to be downloaded.

	pp::URLRequestInfo url_request_;
	pp::URLLoader mUrlLoader;  // URLLoader provides an API to download URLs.

	char buffer_[kBufferSize];  // buffer for pp::URLLoader::ReadResponseBody().
	int mMethod;

	std::string url_response_body_;  // Contains downloaded data.
	pp::CompletionCallbackFactory<GetURLHandler> cc_factory_;

	GetURLHandler ( const GetURLHandler& );
	void operator= ( const GetURLHandler& );
};

#endif  // EXAMPLES_GETURL_GETURL_HANDLER_H_

