// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMUTEX_WIN32_H
#define MOAIMUTEX_WIN32_H

#ifdef _WIN32

#include <moaicore/MOAIMutex.h>
#include <windows.h>

//================================================================//
// MOAIMutexImpl
//================================================================//
class MOAIMutexImpl {
private:

	friend class MOAIMutex;

	HANDLE			mMutexHandle;

	//----------------------------------------------------------------//
	void			Init					();
	void			Lock					();
					MOAIMutexImpl			();				
					~MOAIMutexImpl			();
	void			Unlock					();
};

#endif
#endif
