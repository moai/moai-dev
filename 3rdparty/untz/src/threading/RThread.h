//
//  RThread.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef RTHREAD_H_
#define RTHREAD_H_


// Use Win or Posix
#ifdef WIN32
	#include <windows.h>
#else
    #ifdef __ANDROID__
        #define POSIX
    #endif
    #ifdef __APPLE__
        #define POSIX
    #endif
	#ifndef POSIX
		#warning POSIX will be used (but you did not define it)
	#endif
	#include <pthread.h>
	#include <signal.h>
#endif


class RThread
{
public:	
	RThread()
	{
#ifdef WIN32
 		mThread = NULL;
#endif
		mIsRunning = false;
		mStopRunning = false;
	}

	virtual ~RThread()
	{
		if(!mIsRunning)
			return;
        wait();
#ifdef WIN32
		CloseHandle(mThread);
#endif
	}

	bool start()
	{
		if(mIsRunning)
			return false;
#ifdef WIN32
		mThread = CreateThread(NULL, NULL,RThread::Starter, static_cast<void*>(this), NULL, NULL);
		return mThread != NULL;
#else
		return pthread_create(&mThread, NULL, RThread::Starter, static_cast<void*>(this)) == 0;
#endif
	}

	bool stop()
	{
		if(!mIsRunning)
			return true;

		mStopRunning = true;
		return false;
	}

	virtual void run() = 0;

	bool isRunning() const
	{
		return mIsRunning;
	}

	bool shouldThreadExit() const
	{
		return mStopRunning;
	}

	bool wait() const
	{
 		if(!mIsRunning)
			return false;
#ifdef WIN32
		return WaitForSingleObject(mThread,INFINITE) == 0x00000000L;
#else
		return pthread_join(mThread, NULL) == 0;
#endif
	}
	
	bool kill()
	{
		if(!mIsRunning)
			return false;	
        mIsRunning = false;
#ifdef WIN32
		bool success = TerminateThread(mThread,1) && CloseHandle(mThread);
		mThread = NULL;
 		return success;
#else
 		return pthread_kill(mThread, SIGKILL) == 0;
#endif
	}

private:
#ifdef WIN32
 	static DWORD WINAPI Starter(LPVOID in_thread)
	{
#else
	static void* Starter(void *in_thread)
	{
#endif
		RThread *thread = static_cast<RThread *>(in_thread);
		thread->mIsRunning = true;
		thread->mStopRunning = false;
		thread->run();
		thread->mIsRunning = false;
		return NULL;
	}

private:
	bool mStopRunning;
	bool mIsRunning;
#ifdef WIN32
 	HANDLE mThread;
#else
	pthread_t mThread;
#endif

};

#endif