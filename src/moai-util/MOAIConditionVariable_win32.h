// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICONDITIONVARIABLE_WI32_H
#define MOAICONDITIONVARIABLE_WI32_H

#ifdef _WIN32

#include <windows.h>

class MOAIMutexImpl;

//================================================================//
// MOAIConditionVariableImpl
//================================================================//
class MOAIConditionVariableImpl {
private:

	friend class MOAIConditionVariable;

	CONDITION_VARIABLE	mConditional;

	//----------------------------------------------------------------//
	void			Broadcast						();
					MOAIConditionVariableImpl		();
					~MOAIConditionVariableImpl		();
	void			Signal							();
	void			Wait							( MOAIMutexImpl& mutexImpl );
};

#endif
#endif
