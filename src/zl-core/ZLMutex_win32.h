// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMUTEX_WIN32_H
#define ZLMUTEX_WIN32_H

#ifdef _WIN32

#include <zl-core/ZLMutex.h>
#include <windows.h>

//================================================================//
// ZLMutexImpl
//================================================================//
class ZLMutexImpl {
private:

	friend class ZLMutex;
	friend class ZLConditionVariableImpl;

	CRITICAL_SECTION	mCriticalSection;

	//----------------------------------------------------------------//
	void			Lock				();
					ZLMutexImpl			();
					~ZLMutexImpl		();
	void			Unlock				();
};

#endif
#endif
