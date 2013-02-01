// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_MUTEX_POSIX_H
#define ZL_MUTEX_POSIX_H

#ifndef _WIN32

#include <uslscore/USThread.h>

//================================================================//
// USMutexImpl
//================================================================//
class USMutexImpl {
private:

	friend class USMutex;

	pthread_mutex_t		mMutex;

	//----------------------------------------------------------------//
	void			Init				();
	void			Lock				();
	void			Unlock				();
					USMutexImpl			();				
					~USMutexImpl		();
};

#endif
#endif
