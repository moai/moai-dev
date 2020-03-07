// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>

#ifdef DeleteFile
	#undef DeleteFile
#endif

//================================================================//
// MOAIDisplayListQueueGL
//================================================================//

//----------------------------------------------------------------//
ZLGfxRetained* MOAIDisplayListQueueGL::GetDisplayList () {

	if ( this->mFreeDisplayLists.GetTop () > 0 ) {
		return this->mFreeDisplayLists.Pop ();
	}
	
	ZLIndex idx = this->mDisplayLists.Size ();
	this->mDisplayLists.Grow (( ZLSize )idx + 1 );
	this->mDisplayLists [ idx ] = new ZLGfxRetained ();
	return this->mDisplayLists [ idx ];
}

//----------------------------------------------------------------//
bool MOAIDisplayListQueueGL::HasContent () {

	return ( this->mQueue [ QUEUE_PENDING ] != 0 );
}

//----------------------------------------------------------------//
MOAIDisplayListQueueGL::MOAIDisplayListQueueGL () :
	mRenderCount ( 0 ),
	mEnableLogging ( false ) {

	memset ( this->mQueue, 0, sizeof ( this->mQueue ));
}

//----------------------------------------------------------------//
MOAIDisplayListQueueGL::~MOAIDisplayListQueueGL () {

	for ( ZLIndex i = 0; i < this->mDisplayLists.Size (); ++i ) {
		delete ( this->mDisplayLists [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIDisplayListQueueGL::PhaseBegin ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
			
			// put these back
			while ( this->mFinishedDisplayLists.GetTop ()) {
				this->ReleaseDisplayList ( this->mFinishedDisplayLists.Pop ());
			}

			// illegal to begin while already in progress
			assert ( this->mQueue [ QUEUE_FOR_CPU ] == 0 );

			// bail if pending
			if ( this->mQueue [ QUEUE_PENDING ]) return;
			
			// begin the phase
			this->mQueue [ QUEUE_FOR_CPU ] = this->GetDisplayList ();
			
			break;
		}
		
		case GPU_PHASE: {
		
			// illegal to begin while already in progress
			assert ( this->mQueue [ QUEUE_FOR_GPU ] == 0 );
			
			this->mQueue [ QUEUE_FOR_GPU ] =  this->mQueue [ QUEUE_PENDING ];
			this->mQueue [ QUEUE_PENDING ] = 0;
			
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDisplayListQueueGL::PhaseEnd ( u32 phase ) {

	switch ( phase ) {
	
		case CPU_PHASE: {
			
			ZLGfxRetained* list = this->mQueue [ QUEUE_FOR_CPU ];
			if ( !list ) return;
			
			assert ( this->mQueue [ QUEUE_PENDING ] == 0 );
			
			this->mQueue [ QUEUE_FOR_CPU ] = 0;
			
			if ( list->HasContent ()) {
				this->mQueue [ QUEUE_PENDING ] = list;
			}
			else {
				this->ReleaseDisplayList ( list );
			}
			break;
		}
		
		case GPU_PHASE: {
		
			ZLGfxRetained* list = this->mQueue [ QUEUE_FOR_GPU ];
	
			if ( list ) {
				this->mFinishedDisplayLists.Push ( list );
				this->mQueue [ QUEUE_FOR_GPU ] = 0;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDisplayListQueueGL::PublishAndReset () {

	ZLSize top = this->mFinishedDisplayLists.GetTop ();
	for ( ZLIndex i = 0; i < top; ++i ) {
	
		ZLGfxRetained* list = this->mFinishedDisplayLists [ i ];
		
		list->PublishEventsAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIDisplayListQueueGL::ReleaseDisplayList ( ZLGfxRetained* list ) {

	list->Reset ();

	this->mFreeDisplayLists.Push ( list );
}

//================================================================//
// MOAIGfxMgrGL_DisplayListClerkGL
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::BeginPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->PublishAndReset ( LOADING_QUEUE );
			this->PublishAndReset ( DRAWING_QUEUE );
		
			this->BeginPhase ( LOADING_QUEUE, MOAIDisplayListQueueGL::CPU_PHASE );
			this->BeginPhase ( DRAWING_QUEUE, MOAIDisplayListQueueGL::CPU_PHASE );
			
			break;
			
		case LOADING_PHASE:
		
			this->BeginPhase ( LOADING_QUEUE, MOAIDisplayListQueueGL::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->BeginPhase ( DRAWING_QUEUE, MOAIDisplayListQueueGL::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::BeginPhase ( u32 queueID, u32 phase ) {

	if ( queueID < TOTAL_QUEUES ) {
	
		MOAIDisplayListQueueGL* pipeline = this->mQueues [ queueID ];
		
		if ( pipeline ) {
		
			pipeline->mEnableLogging = this->mEnableQueueLogging;
			
			pipeline->PhaseBegin ( phase );
			
			if (( phase == MOAIDisplayListQueueGL::GPU_PHASE ) && ( pipeline->mQueue [ MOAIDisplayListQueueGL::QUEUE_FOR_GPU ])) {
				pipeline->mRenderCount = this->mQueueRenderCount++;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::EnableQueue ( u32 queueID ) {
	
	assert ( queueID < TOTAL_QUEUES );
	this->mQueues [ queueID ] = new MOAIDisplayListQueueGL ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::EnableQueueLogging ( bool enable ) {

	this->mEnableQueueLogging = enable;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::EndPhase ( u32 phase ) {

	switch ( phase ) {
	
		case LOGIC_PHASE:
		
			this->EndPhase ( LOADING_QUEUE, MOAIDisplayListQueueGL::CPU_PHASE );
			this->EndPhase ( DRAWING_QUEUE, MOAIDisplayListQueueGL::CPU_PHASE );
			break;
			
		case LOADING_PHASE:
		
			this->EndPhase ( LOADING_QUEUE, MOAIDisplayListQueueGL::GPU_PHASE );
			break;
			
		case RENDER_PHASE:
		
			this->EndPhase ( DRAWING_QUEUE, MOAIDisplayListQueueGL::GPU_PHASE );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::EndPhase ( u32 queueID, u32 phase ) {

	if ( queueID < TOTAL_QUEUES ) {
		MOAIDisplayListQueueGL* pipeline = this->mQueues [ queueID ];
		
		if ( pipeline ) {
			pipeline->PhaseEnd ( phase );
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxMgrGL_DisplayListClerkGL::HasContent ( u32 queueID ) {

	assert ( queueID < TOTAL_QUEUES );
	return this->mQueues [ queueID ] ? this->mQueues [ queueID ]->HasContent () : false;
}

//----------------------------------------------------------------//
bool MOAIGfxMgrGL_DisplayListClerkGL::IsQueueEnabled ( u32 queueID ) {

	return ( queueID < TOTAL_QUEUES ) && ( this->mQueues [ queueID ] != NULL );
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::LogQueueRender ( ZLGfxRetained& gfx, size_t renderCount, cc8* name ) {

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
MOAIGfxMgrGL_DisplayListClerkGL::MOAIGfxMgrGL_DisplayListClerkGL () :
	mDrawingAPI ( &mGfxImmediate ),
	mDrawCount ( 0 ),
	mQueueRenderCount ( 0 ),
	mEnableQueueLogging ( false ) {
	
	memset ( this->mQueues, 0, sizeof ( this->mQueues ));
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_DisplayListClerkGL::~MOAIGfxMgrGL_DisplayListClerkGL () {

	for ( u32 i = 0; i < TOTAL_QUEUES; ++i ) {
		if ( this->mQueues [ i ]) {
			delete this->mQueues [ i ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::ProcessQueue ( u32 queueID ) {

	assert ( queueID < TOTAL_QUEUES );
	MOAIDisplayListQueueGL* pipeline = this->mQueues [ queueID ];
	
	if ( pipeline && pipeline->mQueue [ MOAIDisplayListQueueGL::QUEUE_FOR_GPU ]) {

		ZLGfxRetained* list = pipeline->mQueue [ MOAIDisplayListQueueGL::QUEUE_FOR_GPU ];

		if ( list->HasContent ()) {

			if ( pipeline->mEnableLogging ) {
			
				cc8* name = queueID == DRAWING_QUEUE ? "drawing" : "loading";
			
				this->LogQueueRender ( *list, pipeline->mRenderCount, name );
			}

			list->Draw ( this->mGfxImmediate );
			
			if ( queueID == LOADING_QUEUE ) {
				this->mGfxImmediate.Flush ( true );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::PublishAndReset ( u32 queueID ) {

	assert ( queueID < TOTAL_QUEUES );
	MOAIDisplayListQueueGL* pipeline = this->mQueues [ queueID ];
	
	if ( pipeline ) {
		pipeline->PublishAndReset ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_DisplayListClerkGL::ResetDrawingAPIs () {

	ZLGfx* loadingAPI = this->SelectDrawingAPI ( LOADING_QUEUE );
	if ( loadingAPI ) {
		this->GetGPUCache ().ResetGPUState ();
	}
	
	ZLGfx* drawingAPI = this->SelectDrawingAPI ( DRAWING_QUEUE );
	if ( drawingAPI && ( loadingAPI != drawingAPI )) {
		this->GetGPUCache ().ResetGPUState ();
	}
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxMgrGL_DisplayListClerkGL::SelectDrawingAPI () {

	this->mDrawingAPI = &this->mGfxImmediate;
	return this->mDrawingAPI;
}

//----------------------------------------------------------------//
ZLGfx* MOAIGfxMgrGL_DisplayListClerkGL::SelectDrawingAPI ( u32 queueID ) {

	this->mDrawingAPI = &this->mGfxImmediate;

	if ( queueID < TOTAL_QUEUES ) {
	
		MOAIDisplayListQueueGL* pipeline = this->mQueues [ queueID ];
		
		if ( pipeline ) {
			
			this->mDrawingAPI = pipeline->mQueue [ MOAIDisplayListQueueGL::QUEUE_FOR_CPU ];
		}
		else if ( queueID == LOADING_QUEUE ) {
		
			this->SelectDrawingAPI ( DRAWING_QUEUE );
		}
	}
	
	return this->mDrawingAPI;
}
