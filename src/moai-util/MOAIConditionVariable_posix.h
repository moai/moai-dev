// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICONDITIONVARIABLE_POSIX_H
#define MOAICONDITIONVARIABLE_POSIX_H

#ifndef _WIN32

class MOAIMutexImpl;

//================================================================//
// MOAIConditionVariableImpl
//================================================================//
class MOAIConditionVariableImpl {
private:

	friend class MOAIConditionVariable;

	pthread_cond_t		mConditional;

	//----------------------------------------------------------------//
	void			Broadcast						();
					MOAIConditionVariableImpl		();
					~MOAIConditionVariableImpl		();
	void			Signal							();
	void			Wait							( MOAIMutexImpl& mutexImpl );
};

#endif
#endif
