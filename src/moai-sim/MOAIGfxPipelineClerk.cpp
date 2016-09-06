// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxPipelineClerk.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

#ifdef DeleteFile
	#undef DeleteFile
#endif

//================================================================//
// MOAIGfxPipeline
//================================================================//

//----------------------------------------------------------------//
ZLGfxRetained* MOAIGfxPipeline::GetDisplayList () {

	if ( this->mFreeDisplayLists.GetTop () > 0 ) {
		return this->mFreeDisplayLists.Pop ();
	}
	
	size_t size = this->mDisplayLists.Size ();
	this->mDisplayLists.Grow ( size + 1 );
	this->mDisplayLists [ size ] = new ZLGfxRetained ();
	return this->mDisplayLists [ size ];
}

//----------------------------------------------------------------//
bool MOAIGfxPipeline::HasContent () {

	return ( this->mPipeline [ PIPELINE_PENDING ] != 0 );
}

//----------------------------------------------------------------//
MOAIGfxPipeline::MOAIGfxPipeline () :
	mRenderCount ( 0 ),
	mEnableLogging ( false ) {

	memset ( this->mPipeline, 0, sizeof ( this->mPipeline ));
}

//----------------------------------------------------------------//
MOAIGfxPipeline::~MOAIGfxPipeline () {

	for ( u32 i = 0; i < this->mDisplayLists.Size (); ++i ) {
		delete ( this->mDisplayLists [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::PhaseBegin ( u32 phase ) {

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
void MOAIGfxPipeline::PhaseEnd ( u32 phase ) {

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
void MOAIGfxPipeline::PublishAndReset () {

	size_t top = this->mFinishedDisplayLists.GetTop ();
	for ( size_t i = 0; i < top; ++i ) {
	
		ZLGfxRetained* list = this->mFinishedDisplayLists [ i ];
		
		list->PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::ReleaseDisplayList ( ZLGfxRetained* list ) {

	list->Reset ();

	this->mFreeDisplayLists.Push ( list );
}

//================================================================//
// MOAIGfxPipelineClerk
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::BeginPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->PublishAndReset ( LOADING_PIPELINE );
			this->PublishAndReset ( DRAWING_PIPELINE );
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipeline::CPU_PHASE );
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::CPU_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipeline::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::BeginPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
		
			pipeline->mEnableLogging = this->mEnablePipelineLogging;
			
			pipeline->PhaseBegin ( phase );
			
			if (( phase == MOAIGfxPipeline::GPU_PHASE ) && ( pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_GPU ])) {
				pipeline->mRenderCount = this->mPipelineRenderCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new MOAIGfxPipeline ();
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::EnablePipelineLogging ( bool enable ) {

	this->mEnablePipelineLogging = enable;
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipeline::CPU_PHASE );
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::CPU_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipeline::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxPipelineClerk::HasContent ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	return this->mPipelines [ pipelineID ] ? this->mPipelines [ pipelineID ]->HasContent () : false;
}

//----------------------------------------------------------------//
bool MOAIGfxPipelineClerk::IsPipelineEnabled ( u32 pipelineID ) {

	return ( pipelineID < TOTAL_PIPELINES ) && ( this->mPipelines [ pipelineID ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::LogPipelineRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name ) {

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
MOAIGfxPipelineClerk::MOAIGfxPipelineClerk () :
	mDrawingAPI ( &mGfxImmediate ),
	mDrawCount ( 0 ),
	mPipelineRenderCount ( 0 ),
	mEnablePipelineLogging ( false ) {
	
	memset ( this->mPipelines, 0, sizeof ( this->mPipelines ));
}

//----------------------------------------------------------------//
MOAIGfxPipelineClerk::~MOAIGfxPipelineClerk () {

	for ( u32 i = 0; i < TOTAL_PIPELINES; ++i ) {
		if ( this->mPipelines [ i ]) {
			delete this->mPipelines [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::ProcessPipeline ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_GPU ]) {

		ZLGfxRetained* list = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_GPU ];

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
void MOAIGfxPipelineClerk::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineClerk::ResetDrawingAPIs () {

	ZLGfx* loadingAPI = this->SelectDrawingAPI ( LOADING_PIPELINE );
	if ( loadingAPI ) {
		MOAIGfxMgr::Get ().mGfxState.ResetState ();
	}
	
	ZLGfx* drawingAPI = this->SelectDrawingAPI ( DRAWING_PIPELINE );
	if ( drawingAPI && ( loadingAPI != drawingAPI )) {
		MOAIGfxMgr::Get ().mGfxState.ResetState ();
	}
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxPipelineClerk::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxPipelineClerk::SelectDrawingAPI ( u32 pipelineID ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			
			this->mDrawingAPI = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_CPU ];
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
		
			this->SelectDrawingAPI ( DRAWING_PIPELINE );
		}
	}
	
	return this->mDrawingAPI;
}
