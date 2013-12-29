#include "pch.h"
#include "MOAIImageAsyncLoadThread.h"

// private const/dest
MOAIImageAsyncLoadThread::MOAIImageAsyncLoadThread():
	mParamImage(NULL),
	mParamFilename(NULL),
	mParamTransform(0) {
}

MOAIImageAsyncLoadThread::~MOAIImageAsyncLoadThread() {
	if (mParamFilename != NULL)
		free(mParamFilename);
	this->stop();
	this->wait();
}

void MOAIImageAsyncLoadThread::run() {
	mSelfDeleting = true;
	if ((mParamImage == NULL) || (mParamFilename == NULL)) {
		return;
	}

	RScopedLock l(&mLock);

	mParamImage->Load(mParamFilename, mParamTransform);
	mParamImage->mLoading = false;
}

void MOAIImageAsyncLoadThread::setParams(MOAIImage *image, char *filename, u32 transform) {
	RScopedLock l(&mLock);

	mParamImage = image;
	mParamFilename = filename;
	mParamTransform = transform;
}