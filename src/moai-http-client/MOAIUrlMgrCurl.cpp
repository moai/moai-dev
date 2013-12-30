// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-http-client/MOAIHttpTaskCurl.h>
#include <moai-http-client/MOAIUrlMgrCurl.h>
#include <moai-http-client/MOAIHttpTaskCurlThread.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCURL

//================================================================//
// MOAIUrlMgrCurl
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::AddHandle ( MOAIHttpTaskCurl& task ) {

	CURL* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	task.LatchRetain ();
	
	mTasks.push_back(&task);
	
	MOAIHttpTaskCurlThread* thread = new MOAIHttpTaskCurlThread();
	thread->setTask(&task);
	thread->start();
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::MOAIUrlMgrCurl () {
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::~MOAIUrlMgrCurl () {
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::Process () {
	if (mTasks.size() == 0)
		return;

	std::vector<int> delindexes;

	STLArray < MOAIHttpTaskCurl*>::iterator it = mTasks.begin();
	int index = 0;
	while(it != mTasks.end() && mTasks.size() > 0) {
		MOAIHttpTaskCurl *task = *it;
		if (!task->inProgress()) {
			task->CurlFinish();
			task->LatchRelease();
			delindexes.push_back(index);
		}
		it++;
		index++;
	}

	index = 0;
	std::vector<int>::iterator di = delindexes.begin();
	while(di != delindexes.end()) {
		mTasks.erase(mTasks.begin() + (*di - index));
		di++;
		index++;
	}
 }

#endif