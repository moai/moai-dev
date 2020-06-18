// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTBATCH_H
#define	MOAIHASGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractHasGfxScriptBatch.h>

//================================================================//
// MOAIHasComposerBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxScriptBatch :
	public virtual MOAIAbstractHasGfxScriptBatch {
protected:

	ZLStrongPtr < MOAIGfxScriptBatch > mGfxScriptBatch;

	//----------------------------------------------------------------//
	void							_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScriptBatch&		MOAIAbstractHasGfxScriptBatch_AffirmGfxScriptBatch	();

public:

	DECL_LUA_FACTORY ( MOAIHasGfxScriptBatch )

	//----------------------------------------------------------------//
						MOAIHasGfxScriptBatch		();
						~MOAIHasGfxScriptBatch		();
};

#endif
