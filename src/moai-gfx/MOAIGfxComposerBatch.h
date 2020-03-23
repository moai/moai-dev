// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERBATCH_H
#define	MOAIGFXCOMPOSERBATCH_H

#include <moai-gfx/MOAIAbstractGfxComposerBatchInterface.h>

class MOAIGfxComposerRetained;

//================================================================//
// MOAIGfxComposerBatch
//================================================================//
// TODO: doxygen
class MOAIGfxComposerBatch :
	public virtual MOAIAbstractGfxComposerBatchInterface {
private:

	friend class MOAIHasComposerBatch;
	friend class MOAIAbstractGfxComposerBatchInterface;

	ZLSize														mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIAbstractGfxComposer > >		mComposers;

	//----------------------------------------------------------------//
	MOAIAbstractGfxComposer&		MOAIAbstractGfxComposerBatchInterface_AffirmComposer			( ZLIndex index );
	MOAIGfxComposerBatch&			MOAIAbstractGfxComposerBatchInterface_AffirmComposerBatch		();
	MOAIAbstractGfxComposer*		MOAIAbstractGfxComposerBatchInterface_GetComposer				( ZLIndex index );
	MOAIGfxComposerBatch*			MOAIAbstractGfxComposerBatchInterface_GetComposerBatch			();

	//----------------------------------------------------------------//
	inline ZLIndex GetRawIndex ( ZLIndex idx ) {
	
		ZLSize totalComposers = this->mComposers.Size ();
		ZLSize rawIndex = ( totalComposers && ( this->mIndexBatchSize > 0 )) ? (( idx / this->mIndexBatchSize ) % totalComposers ) : 0;
		return rawIndex;
	}

public:

	GET_SET ( ZLSize, IndexBatchSize, mIndexBatchSize )

	DECL_LUA_FACTORY ( MOAIGfxComposerBatch )

	//----------------------------------------------------------------//
	void							Clear						();
									MOAIGfxComposerBatch		();
	virtual							~MOAIGfxComposerBatch		();
	MOAIAbstractGfxComposer*		RawGetComposer				( ZLIndex idx );
	void							Reserve						( ZLSize n );
	size_t							Size						();
};

#endif
