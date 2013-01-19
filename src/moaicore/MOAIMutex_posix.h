// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMUTEX_POSIX_H
#define MOAIMUTEX_POSIX_H

#ifndef _WIN32

#include <moaicore/MOAIThread.h>

//================================================================//
// MOAIMutexImpl
//================================================================//
class MOAIMutexImpl {
private:

	friend class MOAIMutex;

	pthread_mutex_t		mMutex;

	//----------------------------------------------------------------//
	void			Init					();
	void			Lock					();
					MOAIMutexImpl			();				
					~MOAIMutexImpl			();
	void			Unlock					();
};

#endif
#endif
