// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCONDITIONVARIABLE_POSIX_H
#define ZLCONDITIONVARIABLE_POSIX_H

#ifndef _WIN32

class ZLMutexImpl;

//================================================================//
// ZLConditionVariableImpl
//================================================================//
class ZLConditionVariableImpl {
private:

	friend class ZLConditionVariable;

	pthread_cond_t		mConditional;

	//----------------------------------------------------------------//
	void			Broadcast						();
					ZLConditionVariableImpl			();
					~ZLConditionVariableImpl		();
	void			Signal							();
	void			Wait							( ZLMutexImpl& mutexImpl );
};

#endif
#endif
