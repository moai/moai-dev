// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITHREAD_POSIX_H
#define MOAITHREAD_POSIX_H

#ifndef _WIN32

#include <moaicore/MOAIThread.h>

//================================================================//
// MOAIThreadImpl
//================================================================//
class MOAIThreadImpl {
private:

	friend class MOAIThread;

	pthread_t		mThread;

	//----------------------------------------------------------------//
	void			Cleanup				();
	bool			IsCurrent			() const;
	bool			IsRunning			() const;
	void			Join				();
	static void		Sleep				();
	void			Start				( MOAIThread& thread, u32 stackSize );
					MOAIThreadImpl		();				
					~MOAIThreadImpl		();
};

#endif
#endif
