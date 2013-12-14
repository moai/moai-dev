// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIURLMGRCURL_H
#define MOAIURLMGRCURL_H

#if MOAI_WITH_LIBCURL

#include <moai-http-client/MOAIHttpTaskCurl.h>

//================================================================//
// MOAIUrlMgrCurl
//================================================================//
class MOAIUrlMgrCurl :
	public MOAIGlobalClass < MOAIUrlMgrCurl > {
private:

	STLArray < MOAIHttpTaskCurl *> mTasks;

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
