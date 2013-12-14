//
//  MOAIHttpTaskCurlThread.h
//  libmoai
//
//  Created by Brendan Ragan on 14/12/13.
//
//

#ifndef __libmoai__MOAIHttpTaskCurlThread__
#define __libmoai__MOAIHttpTaskCurlThread__

#include "pch.h"
#include "MOAIHttpTaskCurl.h"
#include <threading/Threading.h>

extern "C" {
	#include <curl/curl.h>
}

class MOAIHttpTaskCurlThread : public RThread
{
public:
	static MOAIHttpTaskCurlThread* getInstance();
	static void deleteInstance();
	
	void setParams(void* params);
	
protected:
	void run();
	
private:
	MOAIHttpTaskCurlThread();
	
	bool bLoadReady;
	MOAIHttpTaskCurl* task;
	
	static MOAIHttpTaskCurlThread* mInstance;
	
	RCriticalSection mLock;
};

#endif /* defined(__libmoai__MOAIHttpTaskCurlThread__) */
