// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCONDITIONVARIABLE_H
#define ZLCONDITIONVARIABLE_H

#include <zl-core/ZLMutex.h>

class ZLConditionVariableImpl;

//================================================================//
// ZLConditionVariable
//================================================================//
class ZLConditionVariable :
	public ZLMutex {
private:

	ZLConditionVariableImpl*	mConditionImpl;

	//----------------------------------------------------------------//
	ZLConditionVariable& operator = ( const ZLConditionVariable& ) {
		return *this;
	}

	//----------------------------------------------------------------//
	ZLConditionVariable ( const ZLConditionVariable& ) {
	}

public:

	//----------------------------------------------------------------//
	void			Broadcast					();
					ZLConditionVariable		();
					~ZLConditionVariable		();
	void			Signal						();
	void			Wait						();
};

#endif
