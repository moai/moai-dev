#ifndef MOAIASYNCIMAGELOADTHREAD_H_
#define MOAIASYNCIMAGELOADTHREAD_H_

#include "pch.h"
#include <moaicore/MOAIImage.h>
#include <threading/Threading.h>

class MOAIAsyncImageLoadThread : public RThread
{
public:
	static MOAIAsyncImageLoadThread* getInstance();
	static void deleteInstance();

	void run();

	void setParams(void* params);

private:
	MOAIAsyncImageLoadThread();
	~MOAIAsyncImageLoadThread();

	bool bLoadReady;
	MoaiImageAsyncParams* mParams;

	static MOAIAsyncImageLoadThread* mInstance;

	RCriticalSection mLock;
};


#endif