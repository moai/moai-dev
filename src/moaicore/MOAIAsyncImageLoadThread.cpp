#include "pch.h"
#include <moaicore/MOAIAsyncImageLoadThread.h>

MOAIAsyncImageLoadThread* MOAIAsyncImageLoadThread::mInstance = NULL;

// private const/dest
MOAIAsyncImageLoadThread::MOAIAsyncImageLoadThread():
	bLoadReady(false)
{
}
MOAIAsyncImageLoadThread::~MOAIAsyncImageLoadThread()
{
}

MOAIAsyncImageLoadThread* MOAIAsyncImageLoadThread::getInstance()
{
	if(!mInstance)
	{
		mInstance = new MOAIAsyncImageLoadThread();
	}
	return mInstance;
}

void MOAIAsyncImageLoadThread::deleteInstance()
{
	if(mInstance)
	{
		mInstance->stop();
		//mInstance->mReadMore.signal();
		mInstance->wait();

		delete mInstance;
		mInstance = NULL;
	}
}

void MOAIAsyncImageLoadThread::run()
{
	if(!bLoadReady)
		return;

	RScopedLock l(&mLock);

	mParams->image->Load(mParams->filename, mParams->transform);
	mParams->image->mLoading = false;

	free(mParams->filename);
	free(mParams);
}

void MOAIAsyncImageLoadThread::setParams(void* params)
{
	RScopedLock l(&mLock);

	mParams = (MoaiImageAsyncParams*)params;
	bLoadReady = true;
}