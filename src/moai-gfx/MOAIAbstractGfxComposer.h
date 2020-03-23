// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXCOMPOSER_H
#define	MOAIABSTRACTGFXCOMPOSER_H

#include <moai-gfx/MOAIAbstractGfxComposerInterface.h>

//================================================================//
// MOAIAbstractGfxComposer
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxComposer :
	public virtual MOAIAbstractGfxComposerInterface {
protected:

	friend class MOAIAbstractGfxComposerInterface;

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractGfxComposer_Execute				( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) = 0;
	virtual void		MOAIAbstractGfxComposer_Optimize			() = 0;
	virtual void		MOAIAbstractGfxComposer_RetainObject 		( ZLRefCountedObject* object ) = 0;
	virtual void		MOAIAbstractGfxComposer_SubmitCommand		( MOAIGfxComposerCmdEnum::_ cmd, const void* param, ZLSize size ) = 0;
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SubmitCommand ( MOAIGfxComposerCmdEnum::_ cmd, const TYPE& param ) {
		this->MOAIAbstractGfxComposer_SubmitCommand ( cmd, &param, sizeof ( TYPE ));
	}
	
public:

	//----------------------------------------------------------------//
	void Execute ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {
		this->MOAIAbstractGfxComposer_Execute ( callable, callCommand );
	}
	
	//----------------------------------------------------------------//
	void Optimize () {
		this->MOAIAbstractGfxComposer_Optimize ();
	}
};

#endif
