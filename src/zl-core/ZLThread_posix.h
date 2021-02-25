// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTHREAD_POSIX_H
#define ZLTHREAD_POSIX_H

#ifndef _WIN32

#include <zl-core/ZLThread.h>

//================================================================//
// ZLThreadImpl
//================================================================//
class ZLThreadImpl {
private:

	friend class ZLThread;

	pthread_t		mThread;

	//----------------------------------------------------------------//
	void			Cleanup				();
	bool			IsCurrent			() const;
	bool			IsRunning			() const;
	void			Join				();
					ZLThreadImpl		();				
					~ZLThreadImpl		();
	static void		Sleep				();
	void			Start				( ZLThread& thread, u32 stackSize );
};

//================================================================//
// ZLThreadLocalImpl
//================================================================//
class ZLThreadLocalImpl {
public:

	pthread_key_t	mTlsKey;
	
	//----------------------------------------------------------------//
	ZLThread*		GetCurrentThread		() const;
					ZLThreadLocalImpl		();
					~ZLThreadLocalImpl		();
	void			SetCurrentThread		( ZLThread* thread );
};

#endif
#endif
