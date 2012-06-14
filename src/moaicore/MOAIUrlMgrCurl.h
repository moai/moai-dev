// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIURLMGRCURL_H
#define MOAIURLMGRCURL_H

#ifdef USE_CURL

#include <moaicore/MOAIHttpTaskCurl.h>

//================================================================//
// MOAIUrlMgrCurl
//================================================================//
class MOAIUrlMgrCurl :
	public MOAIGlobalClass < MOAIUrlMgrCurl > {
private:

	STLMap < CURL*, MOAIHttpTaskCurl* > mHandleMap;
	CURLM*	mMultiHandle;
	bool	mMore;

	//----------------------------------------------------------------//
	void			AddHandle				( MOAIHttpTaskCurl& task );

public:

	friend class MOAIHttpTaskCurl;

	//----------------------------------------------------------------//
					MOAIUrlMgrCurl			();
					~MOAIUrlMgrCurl			();
	void			Process					();
};

#endif
#endif
