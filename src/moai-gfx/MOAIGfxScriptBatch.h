// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTBATCH_H
#define	MOAIGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractGfxScriptBatch.h>

//================================================================//
// MOAIGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIGfxScriptBatch :
	public virtual MOAIAbstractGfxScriptBatch {
private:

	ZLSize													mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIAbstractGfxScript > >	mGfxScripts;

	//----------------------------------------------------------------//
	MOAIGfxScriptRetained&		MOAIAbstractGfxScriptBatch_AffirmGfxScript		( ZLIndex index );
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatch_GetGfxScript			( ZLIndex index );
	ZLSize						MOAIAbstractGfxScriptBatch_GetIndexBatchSize	();
	void						MOAIAbstractGfxScriptBatch_ReserveGfxScripts	( ZLSize size );
	void						MOAIAbstractGfxScriptBatch_SetGfxScript			( ZLSize size, MOAIAbstractGfxScript* gfxScript );
	void						MOAIAbstractGfxScriptBatch_SetIndexBatchSize	( ZLSize size );

	//----------------------------------------------------------------//
	inline ZLIndex GetRawIndex ( ZLIndex idx ) {
	
		ZLSize totalGfxScripts = this->mGfxScripts.Size ();
		ZLSize rawIndex = ( totalGfxScripts && ( this->mIndexBatchSize > 0 )) ? (( idx / this->mIndexBatchSize ) % totalGfxScripts ) : 0;
		return rawIndex;
	}

public:

	GET_SET ( ZLSize, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIGfxScriptBatch )

	//----------------------------------------------------------------//
								MOAIGfxScriptBatch			();
	virtual						~MOAIGfxScriptBatch			();
	MOAIAbstractGfxScript*		RawGetGfxScript				( ZLIndex idx );
};

#endif
