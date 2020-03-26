// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWINGAPICALLBACK_H
#define	MOAIABSTRACTDRAWINGAPICALLBACK_H

//================================================================//
// MOAIAbstractDrawingAPICallback
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPICallback {
protected:

	//----------------------------------------------------------------//
	virtual void MOAIAbstractDrawingAPICallback_Call () = 0;

public:

	//----------------------------------------------------------------//
	void Call () {
		this->MOAIAbstractDrawingAPICallback_Call ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractDrawingAPICallback () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractDrawingAPICallback () {
	}
};

#endif
