// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTCALLABLE_H
#define	MOAIABSTRACTGFXSCRIPTCALLABLE_H

//================================================================//
// MOAIAbstractGfxScriptCallable
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptCallable {
protected:

	friend class MOAIGfxScriptCmd;

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractGfxScriptCallable_Call		() = 0;

public:

	//----------------------------------------------------------------//
	void Call () {
		this->MOAIAbstractGfxScriptCallable_Call ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractGfxScriptCallable () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractGfxScriptCallable () {
	}
};

#endif
