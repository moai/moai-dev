// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTBATCH_H
#define	MOAIGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractGfxScriptBatchInterface.h>

class MOAIGfxScript;

//================================================================//
// MOAIGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIGfxScriptBatch :
	public virtual MOAIAbstractGfxScriptBatchInterface {
private:

	friend class MOAIHasComposerBatch;
	friend class MOAIAbstractGfxScriptBatchInterface;

	ZLSize														mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIAbstractGfxScript > >		mComposers;

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript&		MOAIAbstractGfxScriptBatchInterface_AffirmComposer			( ZLIndex index );
	MOAIGfxScriptBatch&			MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch		();
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatchInterface_GetComposer				( ZLIndex index );
	MOAIGfxScriptBatch*			MOAIAbstractGfxScriptBatchInterface_GetComposerBatch		();

	//----------------------------------------------------------------//
	inline ZLIndex GetRawIndex ( ZLIndex idx ) {
	
		ZLSize totalComposers = this->mComposers.Size ();
		ZLSize rawIndex = ( totalComposers && ( this->mIndexBatchSize > 0 )) ? (( idx / this->mIndexBatchSize ) % totalComposers ) : 0;
		return rawIndex;
	}

public:

	GET_SET ( ZLSize, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIGfxScriptBatch )

	//----------------------------------------------------------------//
	void						Clear						();
								MOAIGfxScriptBatch			();
	virtual						~MOAIGfxScriptBatch			();
	MOAIAbstractGfxScript*		RawGetComposer				( ZLIndex idx );
	void						Reserve						( ZLSize n );
	size_t						Size						();
};

#endif
