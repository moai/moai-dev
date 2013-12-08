#include "pch.h"
#include <moaicore/MOAIAsyncImageLoadThread.h>

MOAIImageAsyncLoadThread* MOAIImageAsyncLoadThread::mInstance = NULL;

// private const/dest
MOAIImageAsyncLoadThread::MOAIImageAsyncLoadThread():
	bLoadReady(false)
{
}
MOAIImageAsyncLoadThread::MOAIImageAsyncLoadThread()
{
}

MOAIImageAsyncLoadThread* MOAIImageAsyncLoadThread::getInstance()
{
	if(!mInstance)
	{
		mInstance = new MOAIAsyncImageLoadThread();
	}
	return mInstance;
}

void MOAIImageAsyncLoadThread::deleteInstance()
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

void MOAIImageAsyncLoadThread::run()
{
	if(!bLoadReady)
		return;

	RScopedLock l(&mLock);

	//

	mParams->image->Load(mParams->filename, mParams->transform);
	mParams->image->mLoading = false;

	//free(mParams->filename);
	//free(mParams);
}

void MOAIImageAsyncLoadThread::setParams(void* params)
{
	RScopedLock l(&mLock);

	mParams = (MoaiImageAsyncParams*)params;
	bLoadReady = true;
}