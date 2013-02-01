// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef THREAD_WIN32_H
#define THREAD_WIN32_H

#ifdef _WIN32

#include <uslscore/USThread.h>
#include <windows.h>

//================================================================//
// USThreadImpl
//================================================================//
class USThreadImpl {
private:

	friend class USThread;

	HANDLE			mThread;
	DWORD			mID;

	//----------------------------------------------------------------//
	void			Cleanup				();
	bool			IsCurrent			() const;
	bool			IsRunning			() const;
	void			Join				();
	static void		Sleep				();
	void			Start				( USThread& thread, u32 stackSize );
					USThreadImpl		();				
					~USThreadImpl		();
};

#endif
#endif
