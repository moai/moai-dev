// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MUTEX_WIN32_H
#define MUTEX_WIN32_H

#ifdef _WIN32

#include <uslscore/USMutex.h>
#include <windows.h>

//================================================================//
// USMutexImpl
//================================================================//
class USMutexImpl {
private:

	friend class USMutex;

	HANDLE			mMutexHandle;

	//----------------------------------------------------------------//
	void			Init				();
	void			Lock				();
	void			Unlock				();
					USMutexImpl			();				
					~USMutexImpl		();
};


#endif
#endif
