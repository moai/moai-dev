// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTBATCH_H
#define	MOAIABSTRACTGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractHasGfxScriptBatch.h>

class MOAIGfxScriptBatch;

//================================================================//
// MOAIAbstractGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptBatch :
	public virtual MOAIAbstractHasGfxScriptBatch {
protected:
	
	friend class MOAIAbstractHasGfxScriptBatch;
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxScript&		MOAIAbstractGfxScriptBatch_AffirmGfxScript		( ZLIndex index ) = 0;
	virtual MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatch_GetGfxScript			( ZLIndex index ) = 0;
	virtual ZLSize						MOAIAbstractGfxScriptBatch_GetIndexBatchSize	() = 0;
	virtual void						MOAIAbstractGfxScriptBatch_ReserveGfxScripts	( ZLSize size ) = 0;
	virtual void						MOAIAbstractGfxScriptBatch_SetGfxScript			( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) = 0;
	virtual void						MOAIAbstractGfxScriptBatch_SetIndexBatchSize	( ZLSize size ) = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScriptBatch () {
	}

	//----------------------------------------------------------------//
	~MOAIAbstractGfxScriptBatch () {
	}
};

#endif
