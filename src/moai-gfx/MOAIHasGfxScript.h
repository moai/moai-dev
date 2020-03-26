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
	public virtual MOAIAbstractHasGfxScript {
protected:

	ZLStrongPtr < MOAIAbstractGfxScript > mGfxScript;
	
	//----------------------------------------------------------------//
	MOAIGfxScriptRetained&		MOAIAbstractHasGfxScript_AffirmGfxScript	();
	MOAIAbstractGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		();
	void						MOAIAbstractHasGfxScript_SetGfxScript		( MOAIAbstractGfxScript* gfxScript );

public:

	//----------------------------------------------------------------//
				MOAIHasGfxScript			();
				~MOAIHasGfxScript			();
};

#endif
