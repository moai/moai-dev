// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTHREAD_WIN32_H
#define ZLTHREAD_WIN32_H

#ifdef _WIN32

#include <zl-core/ZLThread.h>
#include <windows.h>

//================================================================//
// ZLThreadImpl
//================================================================//
class ZLThreadImpl {
private:

	friend class ZLThread;

	HANDLE			mThread;
	DWORD			mID;

	//----------------------------------------------------------------//
	void			Cleanup				();
	bool			IsCurrent			() const;
	bool			IsRunning			() const;
	void			Join				();
	static void		Sleep				();
	void			Start				( ZLThread& thread, u32 stackSize );
					ZLThreadImpl		();				
					~ZLThreadImpl		();
};

//================================================================//
// ZLThreadLocalImpl
//================================================================//
class ZLThreadLocalImpl {
public:

	DWORD			mTlsIndex;
	
	//----------------------------------------------------------------//
	ZLThread*		GetCurrentThread		() const;
					ZLThreadLocalImpl		();
					~ZLThreadLocalImpl		();
	void			SetCurrentThread		( ZLThread* thread );
};

#endif
#endif
