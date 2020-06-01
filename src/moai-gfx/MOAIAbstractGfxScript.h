// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPT_H
#define	MOAIABSTRACTGFXSCRIPT_H

#include <moai-gfx/MOAIDrawingAPI.h>

//================================================================//
// MOAIAbstractGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScript :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
protected:

	friend class MOAIAbstractGfxScriptInterface;

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractGfxScript_RunScript			( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) = 0;
	
public:
	
	//----------------------------------------------------------------//
	void RunScript ( MOAIAbstractGfxScriptCallback* callable = NULL, MOAIDrawingAPIEnum::_ callCommand = MOAIDrawingAPIEnum::NONE ) {
		this->MOAIAbstractGfxScript_RunScript ( callable, callCommand );
	}
};

#endif
