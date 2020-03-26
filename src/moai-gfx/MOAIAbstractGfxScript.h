// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPT_H
#define	MOAIABSTRACTGFXSCRIPT_H

#include <moai-gfx/MOAIDrawingCommand.h>

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
	virtual void		MOAIAbstractGfxScript_RunScript			( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand ) = 0;
	
public:
	
	//----------------------------------------------------------------//
	void RunScript ( MOAIAbstractDrawingAPICallback* callable = NULL, MOAIDrawingCmdEnum::_ callCommand = MOAIDrawingCmdEnum::NONE ) {
		this->MOAIAbstractGfxScript_RunScript ( callable, callCommand );
	}
};

#endif
