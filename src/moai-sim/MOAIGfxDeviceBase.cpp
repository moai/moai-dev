// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceBase.h>
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
	
		case LOGIC_PHASE: {
			
			// put these back
			while ( this->mFinishedDisplayPairs.GetTop ()) {
				this->ReleasePair ( this->mFinishedDisplayPairs.Pop ());
			}

			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_LOGIC ] == 0 );
			
			MOAIGfxPipelinePair* pair = 0;
			
			if ( this->mPipeline [ PIPELINE_PENDING ]) {
			
				pair = this->mPipeline [ PIPELINE_PENDING ];
				pair->mCritical.Comment ( "ROLLING BACK FROM PENDING" );
				pair->mTemporal.Reset ();
			}
			else {
				pair = this->GetPair ();
			}

			this->mPipeline [ PIPELINE_LOGIC ] = pair;
			this->mPipeline [ PIPELINE_PENDING ] = 0;
			this->mHasContent = pair->mCritical.HasContent ();
			
			break;
		}
		
		case RENDER_PHASE: {
		
			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_RENDER ] == 0 );
			
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_PENDING ];
			
			if ( pair ) {
				this->mPipeline [ PIPELINE_RENDER ] = pair;
				this->mPipeline [ PIPELINE_PENDING ] = 0;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::PhaseEnd ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE: {
		
			assert ( this->mPipeline [ PIPELINE_PENDING ] == 0 );
		
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_LOGIC ];
			assert ( pair );
			
			this->mPipeline [ PIPELINE_LOGIC ] = 0;
			this->mPipeline [ PIPELINE_PENDING ] = pair;
			
			this->mHasContent = pair->mTemporal.HasContent () || pair->mCritical.HasContent ();
			
			break;
		}
		
		case RENDER_PHASE: {
		
			MOAIGfxPipelinePair* pair = this->mPipeline [ PIPELINE_RENDER ];
	
			if ( pair ) {
				this->mFinishedDisplayPairs.Push ( pair );
				this->mPipeline [ PIPELINE_RENDER ] = 0;
			}
		
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
		pair->mTemporal.PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxPipeline::ReleasePair ( MOAIGfxPipelinePair* pair ) {

	this->mFreeDisplayPairs.Push ( pair );
}

//================================================================//
// MOAIGfxDeviceBase
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BeginPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->PublishAndReset ( LOADING_PIPELINE );
			this->PublishAndReset ( DRAWING_PIPELINE );
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipeline::LOGIC_PHASE );
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::LOGIC_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::RENDER_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BeginPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
		
			pipeline->mEnableLogging = this->mEnablePipelineLogging;
			
			pipeline->PhaseBegin ( phase );
			
			if (( phase == MOAIGfxPipeline::RENDER_PHASE ) && ( pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_RENDER ])) {
				pipeline->mRenderCount = this->mPipelineRenderCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new MOAIGfxPipeline ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipeline::LOGIC_PHASE );
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::LOGIC_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxPipeline::RENDER_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceBase::HasContent ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	return this->mPipelines [ pipelineID ]->HasContent ();
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceBase::IsPipelineEnabled ( u32 pipelineID ) {

	return ( pipelineID < TOTAL_PIPELINES ) && ( this->mPipelines [ pipelineID ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::LogPipelineRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name, cc8* flavor ) {

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
MOAIGfxDeviceBase::MOAIGfxDeviceBase () :
	mDrawingAPI ( &mGfxImmediate ),
	mShaderDirty ( false ),
	mDrawCount ( 0 ),
	mPipelineRenderCount ( 0 ),
	mEnablePipelineLogging ( false ) {
	
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	memset ( this->mPipelines, 0, sizeof ( this->mPipelines ));
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::~MOAIGfxDeviceBase () {

	for ( u32 i = 0; i < TOTAL_PIPELINES; ++i ) {
		if ( this->mPipelines [ i ]) {
			delete this->mPipelines [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::ProcessPipeline ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_RENDER ]) {

		MOAIGfxPipelinePair* pair = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_RENDER ];

		if ( pair->mCritical.HasContent () || pair->mTemporal.HasContent ()) {

			if ( pipeline->mEnableLogging ) {
			
				cc8* name = pipelineID == DRAWING_PIPELINE ? "drawing" : "loading";
			
				this->LogPipelineRender ( pair->mCritical, pipeline->mRenderCount, name, "critical" );
				this->LogPipelineRender ( pair->mTemporal, pipeline->mRenderCount, name, "temporal" );
			}

			pair->mCritical.Draw ( this->mGfxImmediate );
			pair->mTemporal.Draw ( this->mGfxImmediate );
			
			if ( pipelineID == LOADING_PIPELINE ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
ZLGfx& MOAIGfxDeviceBase::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return *this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx& MOAIGfxDeviceBase::SelectDrawingAPI ( u32 pipelineID, bool critical ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			
			MOAIGfxPipelinePair* pair = pipeline->mPipeline [ MOAIGfxPipeline::PIPELINE_LOGIC ];
			assert ( pair );
			
			this->mDrawingAPI = critical ? &pair->mCritical : &pair->mTemporal;
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
		
			this->SelectDrawingAPI ( DRAWING_PIPELINE );
		}
	}
	return *this->mDrawingAPI;
}
