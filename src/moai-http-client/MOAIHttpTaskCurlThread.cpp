//
//  MOAIHttpTaskCurlThread.cpp
//  libmoai
//
//  Created by Brendan Ragan on 14/12/13.
//
//

#include "MOAIHttpTaskCurlThread.h"

MOAIHttpTaskCurlThread* MOAIHttpTaskCurlThread::mInstance = NULL;

// private const/dest
MOAIHttpTaskCurlThread::MOAIHttpTaskCurlThread():
bLoadReady(false)
{
}

MOAIHttpTaskCurlThread* MOAIHttpTaskCurlThread::getInstance()
{
	if(!mInstance)
	{
		mInstance = new MOAIHttpTaskCurlThread();
	}
	return mInstance;
}

void MOAIHttpTaskCurlThread::deleteInstance()
{
	if(mInstance)
	{
		mInstance->stop();
		mInstance->wait();
		
		delete mInstance;
		mInstance = NULL;
	}
}

void MOAIHttpTaskCurlThread::run()
{
	if(!bLoadReady)
		return;
	
	RScopedLock l(&mLock);
	task->AsyncThreadRunner();
}

void MOAIHttpTaskCurlThread::setParams(void* params)
{
	RScopedLock l(&mLock);
	task = (MOAIHttpTaskCurl*)params;
	
	bLoadReady = true;
}