#ifndef MOAIASYNCIMAGELOADTHREAD_H_
#define MOAIASYNCIMAGELOADTHREAD_H_

#include "pch.h"
#include "MOAIImage.h"
#include <threading/Threading.h>

class MOAIImageAsyncLoadThread : public RThread
{
public:
	MOAIImageAsyncLoadThread();
	~MOAIImageAsyncLoadThread();

	void run();

	void setParams(MOAIImage *image, char *filename, u32 transform);
private:
	MOAIImage *mParamImage;
	char *mParamFilename;
	u32 mParamTransform;
	
	RCriticalSection mLock;
};

#endif