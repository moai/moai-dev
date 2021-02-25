// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCONDITIONVARIABLE_WI32_H
#define ZLCONDITIONVARIABLE_WI32_H

#ifdef _WIN32

#include <windows.h>

class MOAIMutexImpl;

//================================================================//
// ZLConditionVariableImpl
//================================================================//
class ZLConditionVariableImpl {
private:

	friend class ZLConditionVariable;

	CONDITION_VARIABLE	mConditional;

	//----------------------------------------------------------------//
	void			Broadcast						();
					ZLConditionVariableImpl		();
					~ZLConditionVariableImpl		();
	void			Signal							();
	void			Wait							( MOAIMutexImpl& mutexImpl );
};

#endif
#endif
