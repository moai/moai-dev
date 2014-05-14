// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAI_HTTP_CLIENT_HEADERS_H
#define	MOAI_HTTP_CLIENT_HEADERS_H

#include <moai-http-client/pch.h>

#if MOAI_WITH_LIBCURL
	#include <moai-http-client/MOAIHttpTaskCurl.h>
	#include <moai-http-client/MOAIUrlMgrCurl.h>
#endif

#if MOAI_OS_NACL
	#include <moai-http-client/MOAIHttpTaskNaCl.h>
	#include <moai-http-client/MOAIUrlMgrNaCl.h>
#endif

#endif
