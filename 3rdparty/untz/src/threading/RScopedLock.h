//
//  RScopedLock.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#ifndef RSCOPEDLOCK_H_
#define RSCOPEDLOCK_H_

#include <threading/Threading.h>

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

#endif