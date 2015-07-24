// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICONDITIONVARIABLE_H
#define MOAICONDITIONVARIABLE_H

#include <moai-util/MOAIMutex.h>

class MOAIConditionVariableImpl;

//================================================================//
// MOAIConditionVariable
//================================================================//
class MOAIConditionVariable :
	public MOAIMutex {
private:

	MOAIConditionVariableImpl*	mConditionImpl;

	//----------------------------------------------------------------//
	MOAIConditionVariable& operator = ( const MOAIConditionVariable& ) {
		return *this;
	}

	//----------------------------------------------------------------//
	MOAIConditionVariable ( const MOAIConditionVariable& ) {
	}

public:

	//----------------------------------------------------------------//
	void			Broadcast					();
					MOAIConditionVariable		();
					~MOAIConditionVariable		();
	void			Signal						();
	void			Wait						();
};

#endif
