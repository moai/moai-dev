// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MUTEX_POSIX_H
#define MUTEX_POSIX_H

#ifndef _WIN32

#include <uslsext/USThread.h>

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
