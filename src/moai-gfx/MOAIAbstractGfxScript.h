// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPT_H
#define	MOAIABSTRACTGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractGfxScriptInterface.h>

//================================================================//
// MOAIAbstractGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScript :
	public virtual MOAIAbstractGfxScriptInterface {
protected:

	friend class MOAIAbstractGfxScriptInterface;

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractGfxScript_Execute			( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand ) = 0;
	virtual void		MOAIAbstractGfxScript_Optimize			() = 0;
	virtual void		MOAIAbstractGfxScript_RetainObject 		( ZLRefCountedObject* object ) = 0;
	virtual void		MOAIAbstractGfxScript_SubmitCommand		( MOAIGfxScriptCmdEnum::_ cmd, const void* param, ZLSize size ) = 0;
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SubmitCommand ( MOAIGfxScriptCmdEnum::_ cmd, const TYPE& param ) {
		this->MOAIAbstractGfxScript_SubmitCommand ( cmd, &param, sizeof ( TYPE ));
	}
	
public:

	//----------------------------------------------------------------//
	void Execute ( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand ) {
		this->MOAIAbstractGfxScript_Execute ( callable, callCommand );
	}
	
	//----------------------------------------------------------------//
	void Optimize () {
		this->MOAIAbstractGfxScript_Optimize ();
	}
};

#endif
