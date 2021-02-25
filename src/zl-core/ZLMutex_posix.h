// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMUTEX_POSIX_H
#define ZLMUTEX_POSIX_H

#ifndef _WIN32

#include <zl-core/ZLThread.h>

//================================================================//
// ZLMutexImpl
//================================================================//
class ZLMutexImpl {
private:

	friend class ZLMutex;
	friend class ZLConditionVariableImpl;

	pthread_mutex_t		mMutex;

	//----------------------------------------------------------------//
	void			Lock				();
					ZLMutexImpl			();
					~ZLMutexImpl		();
	void			Unlock				();
};

#endif
#endif
