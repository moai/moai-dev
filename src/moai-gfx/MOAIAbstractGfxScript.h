// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPT_H
#define	MOAIABSTRACTGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractHasGfxScript.h>

//================================================================//
// MOAIAbstractGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScript :
	public virtual MOAIAbstractHasGfxScript {
protected:
	
	friend class MOAIAbstractHasGfxScript;
	
	//----------------------------------------------------------------//
	virtual void		MOAIAbstractGfxScript_RetainObject			( ZLRefCountedObject* object ) = 0;
	virtual void		MOAIAbstractGfxScript_RunScript				( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) = 0;
	virtual void		MOAIAbstractGfxScript_SubmitCommand			( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript () {
	}

	//----------------------------------------------------------------//
	~MOAIAbstractGfxScript () {
	}
};

#endif
