// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINECACHEVK_H
#define MOAIPIPELINECACHEVK_H

#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>

//================================================================//
// MOAIPipelineCachePageVK
//================================================================//
class MOAIPipelineCachePageVK {
private:

	friend class MOAIPipelineCacheVK;

	STLSet < MOAIPipelineSnapshotVK* > mPipelineSet;

	//----------------------------------------------------------------//
	void Clear () {
	
		STLSet < MOAIPipelineSnapshotVK* >::iterator pipelineIt = this->mPipelineSet.begin ();
		for ( ; pipelineIt != this->mPipelineSet.end (); ++pipelineIt ) {
			( *pipelineIt )->Release ();
		}
		this->mPipelineSet.clear ();
	}
	
	//----------------------------------------------------------------//
	void TakePipeline ( MOAIPipelineSnapshotVK& pipeline ) {
	
		if ( pipeline.mPage == this ) return;
	
		if ( pipeline.mPage ) {
			pipeline.mPage->mPipelineSet.erase ( &pipeline );
		}
		else {
			pipeline.Retain ();
		}
		this->mPipelineSet.insert ( &pipeline );
		pipeline.mPage = this;
	}

public:

	//----------------------------------------------------------------//
	MOAIPipelineCachePageVK () {
	}

	//----------------------------------------------------------------//
	~MOAIPipelineCachePageVK () {
		this->Clear ();
	}
};

//================================================================//
// MOAIPipelineCacheVK
//================================================================//
class MOAIPipelineCacheVK {
protected:

	friend class MOAIPipelineSnapshotVK;

	static const ZLSize		DEFAULT_PAGE_COUNT 				= 60; // every minute
	static const ZLSize		DEFAULT_UPDATE_COUNTER_LIMIT 	= 60; // every second at 60fps

	STLMap < MOAIPipelineKeyVK, MOAIPipelineSnapshotVK* >	mWeakCache;
	ZLLeanArray < MOAIPipelineCachePageVK >			mPages;
	
	ZLIndex				mCurrentPage;
	ZLSize				mUpdateCounter;
	ZLSize				mUpdateCounterLimit;

	//----------------------------------------------------------------//
	void				AffirmPages					();

public:

	//----------------------------------------------------------------//
	void						InitializePipelineCache		( ZLSize pageCount, ZLSize updateCounterLimit );
								MOAIPipelineCacheVK			();
								~MOAIPipelineCacheVK		();
	MOAIPipelineSnapshotVK*		ProcurePipeline				( const MOAIPipelineParamsVK& params );
	void						UpdatePipelineCache			();
};

#endif
