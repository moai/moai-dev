#pragma once

// Use Window or Posix
#ifdef WIN32
	#include <windows.h>
#else
	#ifndef POSIX
		#warning POSIX will be used (but you did not define it)
	#endif
	#include <pthread.h>
#endif

class RCriticalSection
{
private:

#ifdef WIN32
	CRITICAL_SECTION mMutex;
#else
	pthread_mutex_t mMutex;
#endif

	bool _locked;
	
public:	
	RCriticalSection()
	{
		init();
	}

	RCriticalSection(const RCriticalSection &in_mutex)
	{
		init();
			
		if(in_mutex._locked && !_locked)
			lock();
		else if(!in_mutex._locked && _locked)
			unlock();
	}
	
	RCriticalSection& operator=(const RCriticalSection &in_mutex)
	{
		if(in_mutex._locked && !_locked)
			lock();
		else if(!in_mutex._locked && _locked)
			unlock();
		return *this;
	}
	
	virtual ~RCriticalSection()
	{
#ifdef WIN32
		DeleteCriticalSection(&mMutex);
#else
		pthread_mutex_unlock(&mMutex);
		pthread_mutex_destroy(&mMutex);
#endif
	}

	bool lock()
	{ 
		_locked = true;
#ifdef WIN32
		EnterCriticalSection(&mMutex);
		return true;
#else
		return pthread_mutex_lock(&mMutex) == 0; 
#endif
	}

	bool tryLock()
	{ 
		_locked = true;
#ifdef WIN32
		return TryEnterCriticalSection(&mMutex) ? true : false; 
#else
		return pthread_mutex_trylock(&mMutex) == 0;
#endif
	}


	bool unlock()
	{ 
		_locked = false;
#ifdef WIN32
		LeaveCriticalSection(&mMutex);
		return true;
#else
		return pthread_mutex_unlock(&mMutex) == 0;
#endif
	}
	
	bool isLocked() const
	{
		return _locked;
	}


private:
	void init(){
#ifdef WIN32
		InitializeCriticalSection(&mMutex);
#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mMutex,&attr);
		pthread_mutexattr_destroy(&attr);
#endif
		_locked = false;
	}
};