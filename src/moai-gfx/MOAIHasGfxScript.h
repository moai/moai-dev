// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPT_H
#define	MOAIHASGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractHasGfxScript.h>

//================================================================//
// MOAIHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIHasGfxScript :
	public virtual MOAIAbstractHasGfxScript,
	public virtual MOAIAbstractDrawingObject {
protected:

	ZLStrongPtr < MOAIAbstractGfxScript > mGfxScript;
	
	//----------------------------------------------------------------//
	void						MOAIAbstractDrawingAPI_RetainObject			( ZLRefCountedObject* object );
	void						MOAIAbstractDrawingAPI_SubmitCommand		( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	MOAIGfxScriptRetained&		MOAIAbstractHasGfxScript_AffirmGfxScript	();
	MOAIAbstractGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		();
	void						MOAIAbstractHasGfxScript_SetGfxScript		( MOAIAbstractGfxScript* gfxScript );

public:

	//----------------------------------------------------------------//
				MOAIHasGfxScript			();
				~MOAIHasGfxScript			();
};

#endif
