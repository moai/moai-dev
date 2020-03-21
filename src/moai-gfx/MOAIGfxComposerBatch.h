// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERBATCH_H
#define	MOAIGFXCOMPOSERBATCH_H

#include <moai-gfx/MOAIGfxComposerBatchInterface.h>

class MOAIGfxComposer;

//================================================================//
// MOAIGfxComposerBatch
//================================================================//
// TODO: doxygen
class MOAIGfxComposerBatch :
	public virtual MOAIGfxComposerBatchInterface {
private:

	friend class MOAIHasComposerBatch;
	friend class MOAIGfxComposerBatchInterface;

	ZLSize												mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIGfxComposer > >		mComposers;

	//----------------------------------------------------------------//
	MOAIGfxComposer&			MOAIGfxComposerBatchInterface_AffirmComposer			( ZLIndex index );
	MOAIGfxComposerBatch&		MOAIGfxComposerBatchInterface_AffirmComposerBatch		();
	MOAIGfxComposer*			MOAIGfxComposerBatchInterface_GetComposer				( ZLIndex index );
	MOAIGfxComposerBatch*		MOAIGfxComposerBatchInterface_GetComposerBatch			();

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
	void				Clear						();
						MOAIGfxComposerBatch		();
	virtual				~MOAIGfxComposerBatch		();
	MOAIGfxComposer*	RawGetComposer				( ZLIndex idx );
	void				Reserve						( ZLSize n );
	size_t				Size						();
};

#endif
