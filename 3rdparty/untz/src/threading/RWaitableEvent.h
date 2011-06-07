#ifndef RWaitableEvent_H
#define RWaitableEvent_H


// Use Win or Posix
#ifdef WIN32
	#include <windows.h>
#else
	#ifndef POSIX
		#warning POSIX will be used (but you did not define it)
	#endif
	#include <semaphore.h>
#endif

class RWaitableEvent
{
protected:

#ifdef WIN32
	HANDLE mhEvent;
#else
	sem_t mSem;
#endif

public:

	RWaitableEvent(int in_init = 0)
	{
#ifdef WIN32
		mhEvent = CreateEvent(NULL, false, in_init, NULL);
#else
		sem_init(&mSem,0,in_init);
#endif
	}
	
	RWaitableEvent(const RWaitableEvent &in_sem)
	{
		int value = in_sem.value();
#ifdef WIN32
		mhEvent = CreateEvent(NULL, false, false, NULL);
#else
		sem_init(&mSem,0,value);
#endif
	}
	
	void operator=(const RWaitableEvent &in_sem)
	{
		reset(in_sem.value());
	}
	
	virtual ~RWaitableEvent()
	{ 
#ifdef WIN32
		CloseHandle(mhEvent);
#else
		sem_destroy(&mSem);
#endif
	}

	bool wait() const
	{ 
#ifdef WIN32
		return WaitForSingleObject(static_cast<HANDLE>(mhEvent), INFINITE) == 0x00000000L;
#else
		return sem_wait(const_cast<sem_t*>(&mSem)) == 0;
#endif
	}
	
	bool post()
	{ 
#ifdef WIN32
        SetEvent(mhEvent);
		return 1;
#else
		return sem_post(&mSem) == 0;
#endif
	}
	
	int value() const
	{ 
#ifdef WIN32
		return 0;
#else
		int value = -1;
		sem_getvalue(const_cast<sem_t*>(&mSem), &value);
		return value;
#endif
	}

	void reset(int in_init = 0)
	{ 
#ifdef WIN32
		CloseHandle(mhEvent);
		mhEvent = CreateEvent(NULL, false, in_init, NULL);
#else
		sem_destroy(&mSem);
		sem_init(&mSem, 0, in_init);
#endif
	}

};

#endif	// RWaitableEvent_H
