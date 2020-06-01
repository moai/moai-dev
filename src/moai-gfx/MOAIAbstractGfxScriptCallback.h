// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTCALLBACK_H
#define	MOAIABSTRACTGFXSCRIPTCALLBACK_H

//================================================================//
// MOAIAbstractGfxScriptCallback
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptCallback {
protected:

	//----------------------------------------------------------------//
	virtual void MOAIAbstractGfxScriptCallback_Call () = 0;

public:

	//----------------------------------------------------------------//
	void Call () {
		this->MOAIAbstractGfxScriptCallback_Call ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractGfxScriptCallback () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractGfxScriptCallback () {
	}
};

#endif
