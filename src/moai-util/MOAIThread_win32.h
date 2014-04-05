// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITHREAD_WIN32_H
#define MOAITHREAD_WIN32_H

#ifdef _WIN32

#include <moai-util/MOAIThread.h>
#include <windows.h>

//================================================================//
// MOAIThreadImpl
//================================================================//
class MOAIThreadImpl {
private:

	friend class MOAIThread;

	HANDLE			mThread;
	DWORD			mID;

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

//================================================================//
// MOAIThreadLocalImpl
//================================================================//
class MOAIThreadLocalImpl {
public:

	DWORD		mTlsIndex;
	
	//----------------------------------------------------------------//
	MOAIThread*		GetCurrentThread		() const;
					MOAIThreadLocalImpl		();
					~MOAIThreadLocalImpl	();
	void			SetCurrentThread		( MOAIThread* thread );
};

#endif
#endif
