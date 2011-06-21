#pragma once

#include <Threading/Threading.h>

class RScopedLock
{	
public:
	RScopedLock(RCriticalSection* lock) : mLock(lock)
	{
		mLock->lock();
	}
	
	~RScopedLock()
	{
		mLock->unlock();
	}
	
private:
	RCriticalSection *mLock;
};
