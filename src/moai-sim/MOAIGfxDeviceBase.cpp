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
		
			this->PublishAndResetList ( LOADING_LIST );
			this->PublishAndResetList ( DRAWING_LIST );
		
			this->BeginPhase ( DRAWING_LIST, MOAIGfxThreadPipeline::LOGIC_PHASE );
			this->BeginPhase ( LOADING_LIST, MOAIGfxThreadPipeline::LOGIC_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_LIST, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_LIST, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BeginPhase ( u32 list, u32 phase ) {

	if ( list < TOTAL_LISTS ) {
	
		MOAIGfxThreadPipeline* pipeline = this->mLists [ list ];
		
		if ( pipeline) {
			pipeline->PhaseBegin ( phase );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EnableList ( u32 list ) {
	
	assert ( list < TOTAL_LISTS );
	this->mLists [ list ] = new MOAIGfxThreadPipeline ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( DRAWING_LIST, MOAIGfxThreadPipeline::LOGIC_PHASE );
			this->EndPhase ( LOADING_LIST, MOAIGfxThreadPipeline::LOGIC_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_LIST, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_LIST, MOAIGfxThreadPipeline::RENDER_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceBase::HasContent ( u32 list ) {

	assert ( list < TOTAL_LISTS );
	return this->mLists [ list ]->HasContent ();
}

//----------------------------------------------------------------//
bool MOAIGfxDeviceBase::IsListEnabled ( u32 list ) {

	return ( list < TOTAL_LISTS ) && ( this->mLists [ list ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::EndPhase ( u32 list, u32 phase ) {

	if ( list < TOTAL_LISTS ) {
		MOAIGfxThreadPipeline* pipeline = this->mLists [ list ];
		
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
	
	memset ( this->mLists, 0, sizeof ( this->mLists ));
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::~MOAIGfxDeviceBase () {

	for ( u32 i = 0; i < TOTAL_LISTS; ++i ) {
		if ( this->mLists [ i ]) {
			delete this->mLists [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::ProcessList ( u32 list ) {

	assert ( list < TOTAL_LISTS );
	MOAIGfxThreadPipeline* pipeline = this->mLists [ list ];
	
	if ( pipeline && pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_RENDER ]) {

		ZLGfxRetained* retained = pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_RENDER ];

		if ( retained->HasContent ()) {
		
			retained->Draw ( this->mGfxImmediate );
			
			if ( list == LOADING_LIST ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::PublishAndResetList ( u32 list ) {

	assert ( list < TOTAL_LISTS );
	MOAIGfxThreadPipeline* pipeline = this->mLists [ list ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SelectList () {

	this->mDrawingAPI = &this->mGfxImmediate;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SelectList ( u32 list ) {

	this->mDrawingAPI = &this->mGfxImmediate;
	this->mPipeline = 0;

	if ( list < TOTAL_LISTS ) {
	
		MOAIGfxThreadPipeline* pipeline = this->mLists [ list ];
		
		if ( pipeline ) {
			this->mDrawingAPI = pipeline->mPipeline [ MOAIGfxThreadPipeline::PIPELINE_LOGIC ];
			this->mPipeline = pipeline;
		}
		else if ( list == LOADING_LIST ) {
			this->SelectList ( DRAWING_LIST );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::UpdateResetPoint () {

	if ( this->mPipeline ) {
		this->mPipeline->UpdateResetPoint ();
	}
}
