// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxPipelineMgr.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIGfxPipeline
//================================================================//

//----------------------------------------------------------------//
MOAIGfxPipelinePair* MOAIGfxPipeline::GetPair () {

	if ( this->mFreeDisplayPairs.GetTop () > 0 ) {
		return this->mFreeDisplayPairs.Pop ();
	}
	
	size_t size = this->mDisplayPairs.Size ();
	this->mDisplayPairs.Grow ( size + 1 );
	this->mDisplayPairs [ size ] = new MOAIGfxPipelinePair ();
	return this->mDisplayPairs [ size ];
}

//----------------------------------------------------------------//
bool MOAIGfxPipeline::HasContent () {

	return this->mHasContent;
}

//----------------------------------------------------------------//
MOAIGfxPipeline::MOAIGfxPipeline () :
	mHasContent ( false ),
	mRenderCount ( 0 ),
	mEnableLogging ( false ) {

	memset ( this->mPipeline, 0, sizeof ( this->mPipeline ));
}

//----------------------------------------------------------------//
MOAIGfxPipeline::~MOAIGfxPipeline () {

	for ( u32 i = 0; i < this->mDisplayPairs.Size (); ++i ) {
		free ( this->mDisplayPairs [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::PhaseBegin ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
			
			// put these back
			while ( this->mFinishedDisplayPairs.GetTop ()) {
				this->ReleasePair ( this->mFinishedDisplayPairs.Pop ());
			}

			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_CPU ] == 0 );
			
			MOAIGfxPipelinePair* pair = 0;
			
			if ( this->mPipeline [ PIPELINE_PENDING ]) {
			
				pair = this->mPipeline [ PIPELINE_PENDING ];
				pair->mCritical.Comment ( "ROLLING BACK FROM PENDING" );
				pair->mOptional.Reset (); // discard pending commands
			}
			else {
				pair = this->GetPair ();
			}

			this->mPipeline [ PIPELINE_CPU ] = pair;
			this->mPipeline [ PIPELINE_PENDING ] = 0;
			
			this->mHasContent = pair->mCritical.HasContent ();
			
			break;
		}
		
		case GPU_PHASE: {
		
			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_GPU ] == 0 );
			
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_PENDING ];
			
			if ( pair ) {
				this->mPipeline [ PIPELINE_GPU ] = pair;
				this->mPipeline [ PIPELINE_PENDING ] = 0;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::PhaseEnd ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
		
			assert ( this->mPipeline [ PIPELINE_PENDING ] == 0 );
		
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_CPU ];
			assert ( pair );
			
			this->mPipeline [ PIPELINE_CPU ] = 0;
			
			this->mHasContent = pair->mOptional.HasContent () || pair->mCritical.HasContent ();
			
			if ( this->mHasContent ) {
				this->mPipeline [ PIPELINE_PENDING ] = pair;
			}
			else {
				this->ReleasePair ( pair );
			}
			break;
		}
		
		case GPU_PHASE: {
		
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_GPU ];
	
			if ( pair ) {
				this->mFinishedDisplayPairs.Push ( pair );
				this->mPipeline [ PIPELINE_GPU ] = 0;
			}
		
			this->mHasContent = false;
		
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::PublishAndReset () {

	size_t top = this->mFinishedDisplayPairs.GetTop ();
	for ( size_t i = 0; i < top; ++i ) {
	
		MOAIGfxPipelinePair* pair = this->mFinishedDisplayPairs [ i ];
		
		pair->mCritical.PublishEventsAndReset ();
		pair->mOptional.PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::ReleasePair ( MOAIGfxPipelinePair* pair ) {

	pair->mCritical.Reset ();
	pair->mOptional.Reset ();

	this->mFreeDisplayPairs.Push ( pair );
}

//================================================================//
// MOAIGfxPipelineMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::BeginPhase ( u32 phase ) {

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
void MOAIGfxPipelineMgr::BeginPhase ( u32 pipelineID, u32 phase ) {

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
void MOAIGfxPipelineMgr::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new MOAIGfxPipeline ();
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::EnablePipelineLogging ( bool enable ) {

	this->mEnablePipelineLogging = enable;
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::EndPhase ( u32 phase ) {

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
void MOAIGfxPipelineMgr::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxPipelineMgr::HasContent ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	return this->mPipelines [ pipelineID ] ? this->mPipelines [ pipelineID ]->HasContent () : true;
}

//----------------------------------------------------------------//
bool MOAIGfxPipelineMgr::IsPipelineEnabled ( u32 pipelineID ) {

	return ( pipelineID < TOTAL_PIPELINES ) && ( this->mPipelines [ pipelineID ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::LogPipelineRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name, cc8* flavor ) {

	STLString temp;
	temp.write ( "%s/%p.tmp", GFX_PIPELINE_LOGGING_FOLDER, &gfx );
	
	ZLGfxLogger logger;
	logger.Print ( temp, gfx );
	
	size_t commandCount = logger.GetCommandCount () - logger.GetCommentCount ();
	
	if ( commandCount ) {
		STLString filename;
		filename.write ( "%s/display %04lu - %s %s (%lu).txt", GFX_PIPELINE_LOGGING_FOLDER, renderCount, name, flavor, commandCount );
		ZLFileSys::Rename ( temp, filename );
	}
	else {
		ZLFileSys::DeleteFile ( temp );
	}
}

//----------------------------------------------------------------//
MOAIGfxPipelineMgr::MOAIGfxPipelineMgr () :
	mDrawingAPI ( &mGfxImmediate ),
	mDrawCount ( 0 ),
	mPipelineRenderCount ( 0 ),
	mEnablePipelineLogging ( false ) {
	
	memset ( this->mPipelines, 0, sizeof ( this->mPipelines ));
}

//----------------------------------------------------------------//
MOAIGfxPipelineMgr::~MOAIGfxPipelineMgr () {

	for ( u32 i = 0; i < TOTAL_PIPELINES; ++i ) {
		if ( this->mPipelines [ i ]) {
			delete this->mPipelines [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::ProcessPipeline ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_GPU ]) {

		MOAIGfxPipelinePair* pair = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_GPU ];

		if ( pair->mCritical.HasContent () || pair->mOptional.HasContent ()) {

			if ( pipeline->mEnableLogging ) {
			
				cc8* name = pipelineID == DRAWING_PIPELINE ? "drawing" : "loading";
			
				this->LogPipelineRender ( pair->mCritical, pipeline->mRenderCount, name, "critical" );
				this->LogPipelineRender ( pair->mOptional, pipeline->mRenderCount, name, "temporal" );
			}

			pair->mCritical.Draw ( this->mGfxImmediate );
			pair->mOptional.Draw ( this->mGfxImmediate );
			
			if ( pipelineID == LOADING_PIPELINE ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipelineMgr::ResetDrawingAPIs () {

	ZLGfx& loadingAPI = this->SelectDrawingAPI ( LOADING_PIPELINE );
	MOAIGfxDevice::Get ().ResetState ();
	
	ZLGfx& drawingAPI = this->SelectDrawingAPI ( DRAWING_PIPELINE );
	if ( &loadingAPI != &drawingAPI ) {
		MOAIGfxDevice::Get ().ResetState ();
	}
}

//----------------------------------------------------------------//
ZLGfx& MOAIGfxPipelineMgr::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return *this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx& MOAIGfxPipelineMgr::SelectDrawingAPI ( u32 pipelineID, bool critical ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			
			MOAIGfxPipelinePair* pair = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_CPU ];
			assert ( pair );
			
			this->mDrawingAPI = critical ? &pair->mCritical : &pair->mOptional;
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
		
			this->SelectDrawingAPI ( DRAWING_PIPELINE );
		}
	}
	
	return *this->mDrawingAPI;
}
