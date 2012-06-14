// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef THREAD_POSIX_H
#define THREAD_POSIX_H

#ifndef _WIN32

#include <uslscore/USThread.h>

//================================================================//
// USThreadImpl
//================================================================//
class USThreadImpl {
private:

	friend class USThread;

	pthread_t		mThread;

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
