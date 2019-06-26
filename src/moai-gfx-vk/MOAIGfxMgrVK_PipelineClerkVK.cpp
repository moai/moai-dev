// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>

#ifdef DeleteFile
	#undef DeleteFile
#endif

//================================================================//
// MOAIGfxPipelineVK
//================================================================//

//----------------------------------------------------------------//
ZLGfxRetained* MOAIGfxPipelineVK::GetDisplayList () {

	if ( this->mFreeDisplayLists.GetTop () > 0 ) {
		return this->mFreeDisplayLists.Pop ();
	}
	
	ZLIndex idx = ZLIndexCast ( this->mDisplayLists.Size () );
	this->mDisplayLists.Grow (( ZLSize )idx + 1 );
	this->mDisplayLists [ idx ] = new ZLGfxRetained ();
	return this->mDisplayLists [ idx ];
}

//----------------------------------------------------------------//
bool MOAIGfxPipelineVK::HasContent () {

	return ( this->mPipeline [ PIPELINE_PENDING ] != 0 );
}

//----------------------------------------------------------------//
MOAIGfxPipelineVK::MOAIGfxPipelineVK () :
	mRenderCount ( 0 ),
	mEnableLogging ( false ) {

	memset ( this->mPipeline, 0, sizeof ( this->mPipeline ));
}

//----------------------------------------------------------------//
MOAIGfxPipelineVK::~MOAIGfxPipelineVK () {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mDisplayLists.Size (); ++i ) {
		delete ( this->mDisplayLists [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineVK::PhaseBegin ( u32 phase ) {

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
void MOAIGfxPipelineVK::PhaseEnd ( u32 phase ) {

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
void MOAIGfxPipelineVK::PublishAndReset () {

	ZLSize top = this->mFinishedDisplayLists.GetTop ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < top; ++i ) {
	
		ZLGfxRetained* list = this->mFinishedDisplayLists [ i ];
		
		list->PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineVK::ReleaseDisplayList ( ZLGfxRetained* list ) {

	list->Reset ();

	this->mFreeDisplayLists.Push ( list );
}

//================================================================//
// MOAIGfxMgrVK_PipelineClerkVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::BeginPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->PublishAndReset ( LOADING_PIPELINE );
			this->PublishAndReset ( DRAWING_PIPELINE );
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipelineVK::CPU_PHASE );
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipelineVK::CPU_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipelineVK::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipelineVK::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::BeginPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipelineVK* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
		
			pipeline->mEnableLogging = this->mEnablePipelineLogging;
			
			pipeline->PhaseBegin ( phase );
			
			if (( phase == MOAIGfxPipelineVK::GPU_PHASE ) && ( pipeline->mPipeline [ MOAIGfxPipelineVK::PIPELINE_GPU ])) {
				pipeline->mRenderCount = this->mPipelineRenderCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new MOAIGfxPipelineVK ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::EnablePipelineLogging ( bool enable ) {

	this->mEnablePipelineLogging = enable;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipelineVK::CPU_PHASE );
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipelineVK::CPU_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipelineVK::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipelineVK::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		MOAIGfxPipelineVK* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_PipelineClerkVK::HasContent ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	return this->mPipelines [ pipelineID ] ? this->mPipelines [ pipelineID ]->HasContent () : false;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrVK_PipelineClerkVK::IsPipelineEnabled ( u32 pipelineID ) {

	return ( pipelineID < TOTAL_PIPELINES ) && ( this->mPipelines [ pipelineID ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::LogPipelineRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name ) {

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
MOAIGfxMgrVK_PipelineClerkVK::MOAIGfxMgrVK_PipelineClerkVK () :
	mDrawingAPI ( &mGfxImmediate ),
	mDrawCount ( 0 ),
	mPipelineRenderCount ( 0 ),
	mEnablePipelineLogging ( false ) {
	
	memset ( this->mPipelines, 0, sizeof ( this->mPipelines ));
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_PipelineClerkVK::~MOAIGfxMgrVK_PipelineClerkVK () {

	for ( u32 i = 0; i < TOTAL_PIPELINES; ++i ) {
		if ( this->mPipelines [ i ]) {
			delete this->mPipelines [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::ProcessPipeline ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipelineVK* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxPipelineVK::PIPELINE_GPU ]) {

		ZLGfxRetained* list = pipeline->mPipeline [ MOAIGfxPipelineVK::PIPELINE_GPU ];

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
void MOAIGfxMgrVK_PipelineClerkVK::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipelineVK* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_PipelineClerkVK::ResetDrawingAPIs () {

	ZLGfx* loadingAPI = this->SelectDrawingAPI ( LOADING_PIPELINE );
	if ( loadingAPI ) {
		this->GetGPUCache ().ResetGPUState ();
	}
	
	ZLGfx* drawingAPI = this->SelectDrawingAPI ( DRAWING_PIPELINE );
	if ( drawingAPI && ( loadingAPI != drawingAPI )) {
		this->GetGPUCache ().ResetGPUState ();
	}
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxMgrVK_PipelineClerkVK::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxMgrVK_PipelineClerkVK::SelectDrawingAPI ( u32 pipelineID ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipelineVK* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			
			this->mDrawingAPI = pipeline->mPipeline [ MOAIGfxPipelineVK::PIPELINE_CPU ];
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
		
			this->SelectDrawingAPI ( DRAWING_PIPELINE );
		}
	}
	
	return this->mDrawingAPI;
}
