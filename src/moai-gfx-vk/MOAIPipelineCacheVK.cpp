// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineCacheVK.h>

//================================================================//
// MOAIPipelineCacheVK
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineCacheVK::AffirmPages () {

	if ( this->mPages.Size () == 0 ) {
		this->InitializePipelineCache ( DEFAULT_PAGE_COUNT, this->mUpdateCounterLimit );
	}
}

//----------------------------------------------------------------//
void MOAIPipelineCacheVK::InitializePipelineCache ( ZLSize pageCount, ZLSize updateCounterLimit ) {

	this->mPages.Init ( pageCount );
	this->mUpdateCounterLimit = updateCounterLimit;
}

//----------------------------------------------------------------//
MOAIPipelineCacheVK::MOAIPipelineCacheVK () :
	mCurrentPage ( 0 ),
	mUpdateCounter ( 0 ),
	mUpdateCounterLimit ( DEFAULT_UPDATE_COUNTER_LIMIT ) {
}

//----------------------------------------------------------------//
MOAIPipelineCacheVK::~MOAIPipelineCacheVK () {
}

//----------------------------------------------------------------//
MOAIPipelineSnapshotVK* MOAIPipelineCacheVK::ProcurePipeline ( const MOAIPipelineParamsVK& params ) {

	MOAIPipelineKeyVK key ( params );
	MOAIPipelineSnapshotVK* pipeline = this->mWeakCache.contains ( key ) ? this->mWeakCache [ key ] : NULL;

	if ( !pipeline ) {
		this->AffirmPages ();
		pipeline = new MOAIPipelineSnapshotVK ( params );
		this->mWeakCache [ *pipeline ] = pipeline;
	}
	this->mPages [ this->mCurrentPage ].TakePipeline ( *pipeline );
	return pipeline;
}

//----------------------------------------------------------------//
void MOAIPipelineCacheVK::UpdatePipelineCache () {

	this->mUpdateCounter++;
	if ( this->mUpdateCounter >= this->mUpdateCounterLimit ) {
		this->mUpdateCounter = 0;
		this->mCurrentPage = ( this->mCurrentPage + 1 ) % this->mPages.Size ();
		this->mPages [ this->mCurrentPage ].Clear ();
	}
}
