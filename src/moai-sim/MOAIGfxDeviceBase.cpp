// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceBase.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIGfxThreadPipeline
//================================================================//

//----------------------------------------------------------------//
ZLGfxRetained* MOAIGfxThreadPipeline::GetList () {

	if ( this->mFreeDisplayLists.GetTop () > 0 ) {
		return this->mFreeDisplayLists.Pop ();
	}
	
	size_t size = this->mDisplayLists.Size ();
	this->mDisplayLists.Grow ( size + 1 );
	this->mDisplayLists [ size ] = new ZLGfxRetained ();
	return this->mDisplayLists [ size ];
}

//----------------------------------------------------------------//
bool MOAIGfxThreadPipeline::HasContent () {

	return this->mHasContent;
}

//----------------------------------------------------------------//
MOAIGfxThreadPipeline::MOAIGfxThreadPipeline () :
	mHasContent ( false ),
	mResetPoint ( 0 ) {

	memset ( this->mPipeline, 0, sizeof ( this->mPipeline ));
}

//----------------------------------------------------------------//
MOAIGfxThreadPipeline::~MOAIGfxThreadPipeline () {

	for ( u32 i = 0; i < this->mDisplayLists.Size (); ++i ) {
		free ( this->mDisplayLists [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIGfxThreadPipeline::PhaseBegin ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE: {
			
			// put these back
			while ( this->mFinishedDisplayLists.GetTop ()) {
				this->ReleaseList ( this->mFinishedDisplayLists.Pop ());
			}

			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_LOGIC ] == 0 );
			
			ZLGfxRetained* gfx = this->mResetPoint ? this->mPipeline [ PIPELINE_PENDING ] : this->GetList ();
			assert ( gfx );
			
			gfx->GetStream ()->Seek ( this->mResetPoint );
			this->mPipeline [ PIPELINE_LOGIC ] = gfx;
			this->mPipeline [ PIPELINE_PENDING ] = 0;
			
			this->mHasContent = false;
			
			break;
		}
		
		case RENDER_PHASE: {
		
			// illegal to begin while already in progress
			assert ( this->mPipeline [ PIPELINE_RENDER ] == 0 );
			
			ZLGfxRetained* gfx = this->mPipeline [ PIPELINE_PENDING ];
			
			if ( gfx ) {
				this->mPipeline [ PIPELINE_RENDER ] = gfx;
				this->mPipeline [ PIPELINE_PENDING ] = 0;
				this->mResetPoint = 0;
			}
		
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxThreadPipeline::PhaseEnd ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE: {
		
			assert ( this->mPipeline [ PIPELINE_PENDING ] == 0 );
		
			ZLGfxRetained* gfx = this->mPipeline [ PIPELINE_LOGIC ];
			assert ( gfx );
			
			this->mPipeline [ PIPELINE_LOGIC ] = 0;
			this->mPipeline [ PIPELINE_PENDING ] = gfx;
			
			this->mHasContent = gfx->HasContent ();
		
			break;
		}
		
		case RENDER_PHASE: {
		
			ZLGfxRetained* gfx = this->mPipeline [ PIPELINE_RENDER ];
	
			if ( gfx ) {
				this->mFinishedDisplayLists.Push ( gfx );
				this->mPipeline [ PIPELINE_RENDER ] = 0;
			}
		
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxThreadPipeline::PublishAndReset () {

	size_t top = this->mFinishedDisplayLists.GetTop ();
	for ( size_t i = 0; i < top; ++i ) {
		this->mFinishedDisplayLists [ i ]->PublishEvents ();
		this->mFinishedDisplayLists [ i ]->Reset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxThreadPipeline::ReleaseList ( ZLGfxRetained* list ) {

	this->mFreeDisplayLists.Push ( list );
}

//----------------------------------------------------------------//
void MOAIGfxThreadPipeline::UpdateResetPoint () {

	ZLGfxRetained* gfx = this->mPipeline [ PIPELINE_LOGIC ];
	
	if ( gfx ) {
		this->mResetPoint = gfx->GetStream ()->GetCursor ();
	}
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
		
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxThreadPipeline::LOGIC_PHASE );
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxThreadPipeline::LOGIC_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_PIPELINE, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_PIPELINE, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BeginPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxThreadPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline) {
			pipeline->PhaseBegin ( phase );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EnablePipeline ( u32 pipelineID ) {
	
	assert ( pipelineID < TOTAL_PIPELINES );
	this->mPipelines [ pipelineID ] = new MOAIGfxThreadPipeline ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxThreadPipeline::LOGIC_PHASE );
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxThreadPipeline::LOGIC_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_PIPELINE, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_PIPELINE, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
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
void MOAIGfxDeviceBase::EndPhase ( u32 pipelineID, u32 phase ) {

	if ( pipelineID < TOTAL_PIPELINES ) {
		MOAIGfxThreadPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::MOAIGfxDeviceBase () :
	mDrawingAPI ( &this->mGfxImmediate ),
	mShaderDirty ( false ),
	mDrawCount ( 0 ) {
	
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
	MOAIGfxThreadPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_RENDER ]) {

		ZLGfxRetained* retained = pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_RENDER ];

		if ( retained->HasContent ()) {
		
			retained->Draw ( this->mGfxImmediate );
			
			if ( pipelineID == LOADING_PIPELINE ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::PublishAndReset ( u32 pipelineID ) {

	assert ( pipelineID < TOTAL_PIPELINES );
	MOAIGfxThreadPipeline* pipeline = this->mPipelines [ pipelineID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SelectPipeline () {

	this->mDrawingAPI = &this->mGfxImmediate;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SelectPipeline ( u32 pipelineID ) {

	this->mDrawingAPI = &this->mGfxImmediate;
	this->mPipeline = 0;

	if ( pipelineID < TOTAL_PIPELINES ) {
	
		MOAIGfxThreadPipeline* pipeline = this->mPipelines [ pipelineID ];
		
		if ( pipeline ) {
			this->mDrawingAPI = pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_LOGIC ];
			this->mPipeline = pipeline;
		}
		else if ( pipelineID == LOADING_PIPELINE ) {
			this->SelectPipeline ( DRAWING_PIPELINE );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::UpdateResetPoint () {

	if ( this->mPipeline ) {
		this->mPipeline->UpdateResetPoint ();
	}
}
