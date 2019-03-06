// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfxLogger.h>
#include <zl-gfx/ZLGfxPipelineClerk.h>
#include <zl-gfx/ZLGfxStateGPUCache.h>

#ifdef DeleteFile
	#undef DeleteFile
#endif

//================================================================//
// ZLGfxPipeline
//================================================================//

//----------------------------------------------------------------//
ZLGfxRetained* ZLGfxPipeline::GetDisplayList () {

	if ( this->mFreeDisplayLists.GetTop () > 0 ) {
		return this->mFreeDisplayLists.Pop ();
	}
	
	ZLIndex idx = ZLIndexCast ( this->mDisplayLists.Size () );
	this->mDisplayLists.Grow (( ZLSize )idx + 1 );
	this->mDisplayLists [ idx ] = new ZLGfxRetained ();
	return this->mDisplayLists [ idx ];
}

//----------------------------------------------------------------//
bool ZLGfxPipeline::HasContent () {

	return ( this->mPipeline [ PIPELINE_PENDING ] != 0 );
}

//----------------------------------------------------------------//
ZLGfxPipeline::ZLGfxPipeline () :
	mRenderCount ( 0 ),
	mEnableLogging ( false ) {

	memset ( this->mPipeline, 0, sizeof ( this->mPipeline ));
}

//----------------------------------------------------------------//
ZLGfxPipeline::~ZLGfxPipeline () {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mDisplayLists.Size (); ++i ) {
		delete ( this->mDisplayLists [ i ]);
	}
}

//----------------------------------------------------------------//
void ZLGfxPipeline::PhaseBegin ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
			
			// put these back
			while ( this->mFinishedDisplayLists.GetTop ()) {
				this->ReleaseDisplayList ( this->mFinishedDisplayLists.Pop ());
			}

			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_CPU ] == 0 );

			// bail if pending
			if ( this->mPipeline [ PIPELINE_PENDING ]) return;
			
			// begin the phase
			this->mPipeline [ PIPELINE_CPU ] = this->GetDisplayList ();
			
			break;
		}
		
		case GPU_PHASE: {
		
			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_GPU ] == 0 );
			
			this->mPipeline [ PIPELINE_GPU ] =  this->mPipeline [ PIPELINE_PENDING ];
			this->mPipeline [ PIPELINE_PENDING ] = 0;
			
			break;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxPipeline::PhaseEnd ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
			
			ZLGfxRetained* list = this->mPipeline [ PIPELINE_CPU ];
			if ( !list ) return;
			
			assert ( this->mPipeline [ PIPELINE_PENDING ] == 0 );
			
			this->mPipeline [ PIPELINE_CPU ] = 0;
			
			if ( list->HasContent ()) {
				this->mPipeline [ PIPELINE_PENDING ] = list;
			}
			else {
				this->ReleaseDisplayList ( list );
			}
			break;
		}
		
		case GPU_PHASE: {
		
			ZLGfxRetained* list = this->mPipeline [ PIPELINE_GPU ];
	
			if ( list ) {
				this->mFinishedDisplayLists.Push ( list );
				this->mPipeline [ PIPELINE_GPU ] = 0;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxPipeline::PublishAndReset () {

	ZLSize top = this->mFinishedDisplayLists.GetTop ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < top; ++i ) {
	
		ZLGfxRetained* list = this->mFinishedDisplayLists [ i ];
		
		list->PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void ZLGfxPipeline::ReleaseDisplayList ( ZLGfxRetained* list ) {

	list->Reset ();

	this->mFreeDisplayLists.Push ( list );
}

//================================================================//
// ZLGfxPipelineClerk
//================================================================//

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::BeginPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->PublishAndReset ( LOADING_PIPELINE );
			this->PublishAndReset ( DRAWING_PIPELINE );
		
			this->BeginPhase ( LOADING_PIPELINE, ZLGfxPipeline::CPU_PHASE );
			this->BeginPhase ( DRAWING_PIPELINE, ZLGfxPipeline::CPU_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_PIPELINE, ZLGfxPipeline::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_PIPELINE, ZLGfxPipeline::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::BeginPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		ZLGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
		
			pipeline->mEnableLogging = this->mEnablePipelineLogging;
			
			pipeline->PhaseBegin ( phase );
			
			if (( phase == ZLGfxPipeline::GPU_PHASE ) && ( pipeline->mPipeline [ ZLGfxPipeline::PIPELINE_GPU ])) {
				pipeline->mRenderCount = this->mPipelineRenderCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new ZLGfxPipeline ();
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::EnablePipelineLogging ( bool enable ) {

	this->mEnablePipelineLogging = enable;
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, ZLGfxPipeline::CPU_PHASE );
			this->EndPhase ( DRAWING_PIPELINE, ZLGfxPipeline::CPU_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, ZLGfxPipeline::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, ZLGfxPipeline::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		ZLGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool ZLGfxPipelineClerk::HasContent ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	return this->mPipelines [ pipelineID ] ? this->mPipelines [ pipelineID ]->HasContent () : false;
}

//----------------------------------------------------------------//
bool ZLGfxPipelineClerk::IsPipelineEnabled ( u32 pipelineID ) {

	return ( pipelineID < TOTAL_PIPELINES ) && ( this->mPipelines [ pipelineID ] != NULL );
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::LogPipelineRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name ) {

	STLString temp;
	temp.write ( "%s/%p.tmp", GFX_PIPELINE_LOGGING_FOLDER, &gfx );
	
	ZLGfxLogger logger;
	logger.Print ( temp, gfx );
	
	size_t commandCount = logger.GetCommandCount () - logger.GetCommentCount ();
	
	if ( commandCount ) {
		STLString filename;
		filename.write ( "%s/display %04lu - %s (%lu).txt", GFX_PIPELINE_LOGGING_FOLDER, renderCount, name, commandCount );
		ZLFileSys::Rename ( temp, filename );
	}
	else {
		ZLFileSys::DeleteFile ( temp );
	}
}

//----------------------------------------------------------------//
ZLGfxPipelineClerk::ZLGfxPipelineClerk () :
	mDrawingAPI ( &mGfxImmediate ),
	mDrawCount ( 0 ),
	mPipelineRenderCount ( 0 ),
	mEnablePipelineLogging ( false ) {
	
	memset ( this->mPipelines, 0, sizeof ( this->mPipelines ));
}

//----------------------------------------------------------------//
ZLGfxPipelineClerk::~ZLGfxPipelineClerk () {

	for ( u32 i = 0; i < TOTAL_PIPELINES; ++i ) {
		if ( this->mPipelines [ i ]) {
			delete this->mPipelines [ i ];
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::ProcessPipeline ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	ZLGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ ZLGfxPipeline::PIPELINE_GPU ]) {

		ZLGfxRetained* list = pipeline->mPipeline [ ZLGfxPipeline::PIPELINE_GPU ];

		if ( list->HasContent ()) {

			if ( pipeline->mEnableLogging ) {
			
				cc8* name = pipelineID == DRAWING_PIPELINE ? "drawing" : "loading";
			
				this->LogPipelineRender ( *list, pipeline->mRenderCount, name );
			}

			list->Draw ( this->mGfxImmediate );
			
			if ( pipelineID == LOADING_PIPELINE ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	ZLGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void ZLGfxPipelineClerk::ResetDrawingAPIs () {

	ZLGfx* loadingAPI = this->SelectDrawingAPI ( LOADING_PIPELINE );
	if ( loadingAPI ) {
		this->GetGfxStateCacheGPU ().ResetGPUState ();
	}
	
	ZLGfx* drawingAPI = this->SelectDrawingAPI ( DRAWING_PIPELINE );
	if ( drawingAPI && ( loadingAPI != drawingAPI )) {
		this->GetGfxStateCacheGPU ().ResetGPUState ();
	}
}

//----------------------------------------------------------------//
ZLGfx* ZLGfxPipelineClerk::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx* ZLGfxPipelineClerk::SelectDrawingAPI ( u32 pipelineID ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		ZLGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			
			this->mDrawingAPI = pipeline->mPipeline [ ZLGfxPipeline::PIPELINE_CPU ];
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
		
			this->SelectDrawingAPI ( DRAWING_PIPELINE );
		}
	}
	
	return this->mDrawingAPI;
}
