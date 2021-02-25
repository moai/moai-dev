// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIURLMGRCURL_H
#define MOAIURLMGRCURL_H

#if MOAI_WITH_LIBCURL

#include <moai-http-client/MOAIHttpTaskCurl.h>

//================================================================//
// MOAIUrlMgrCurl
//================================================================//
class MOAIUrlMgrCurl :
	public ZLContextClass < MOAIUrlMgrCurl > {
private:

	STLMap < CURL*, MOAIHttpTaskCurl* > mHandleMap;
	CURLM*	    mMultiHandle;
	bool	    mMore;
	ZLThread* mThread;
	ZLMutex   mLock;

	//----------------------------------------------------------------//
	void			AddHandle				( MOAIHttpTaskCurl& task );
	void			RemoveHandle			( MOAIHttpTaskCurl& task );
	void			Start					();
	void			StopIfDone				( bool force = false);

public:

	friend class MOAIHttpTaskCurl;

	//----------------------------------------------------------------//
					MOAIUrlMgrCurl			();
					~MOAIUrlMgrCurl			();
	bool			Process					();
	void			ProcessHandle			();
	void			ProcessAsync			();
};

#endif
#endif
