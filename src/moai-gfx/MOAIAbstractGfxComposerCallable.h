// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXCOMPOSERCALLABLE_H
#define	MOAIABSTRACTGFXCOMPOSERCALLABLE_H

//================================================================//
// MOAIAbstractGfxComposerCallable
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxComposerCallable {
protected:

	friend class MOAIGfxComposerCmd;

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractGfxComposerCallable_Call		() = 0;

public:

	//----------------------------------------------------------------//
	void Call () {
		this->MOAIAbstractGfxComposerCallable_Call ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractGfxComposerCallable () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractGfxComposerCallable () {
	}
};

#endif
