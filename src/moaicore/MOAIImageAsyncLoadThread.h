#ifndef IMAGEASYNCLOADTHREAD_H_
#define IMAGEASYNCLOADTHREAD_H_

#include "pch.h"
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIDataBuffer.h>
#include <untz/src/threading/Threading.h>

class ImageAsyncLoadThread : public RThread
{
public:
	static ImageAsyncLoadThread* getInstance();
	static void deleteInstance();

	void run();

	bool setParams(void* params);

private:
	static ImageAsyncLoadThread* mInstance;

	MoaiImageAsyncParams* loadParams;

	bool mLoadDispatchReady;
	RCriticalSection mLock;
};


#endif