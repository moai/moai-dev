#include "pch.h"
#include <moaicore/MOAIImageAsyncLoadThread.h>

ImageAsyncLoadThread* ImageAsyncLoadThread::mInstance = NULL;

ImageAsyncLoadThread* ImageAsyncLoadThread::getInstance()
{
	if(!mInstance)
	{
		mInstance = new ImageAsyncLoadThread();
	}
	return mInstance;
}

void ImageAsyncLoadThread::deleteInstance()
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


void ImageAsyncLoadThread::run()
{
	if(!mLoadDispatchReady)
		return;

	// lock for the duration of this scope
	RScopedLock l(&mLock);

	loadParams->image->Load(
		loadParams->filename,
		loadParams->transform
	);
	loadParams->image->mLoading = false;

	free(loadParams->filename);
	free(loadParams);
	loadParams = NULL;
}

bool ImageAsyncLoadThread::setParams(void* params)
{
	// lock for the duration of this scope
	RScopedLock l(&mLock);

	loadParams = ((MoaiImageAsyncParams*)params);
	mLoadDispatchReady = true;

	return true;
}