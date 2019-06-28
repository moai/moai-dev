// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-core/host.h>
#include <moai-sdl/host.h>
#include <host-modules/aku_modules.h>

#include "SDLHostGLMultiThread.h"

// counters for testing async timing issues (dropped frames)
static const int SIM_UPDATE_INTERVAL		= 1;
static const int LOAD_UPDATE_INTERVAL		= 1;
static const int RENDER_UPDATE_INTERVAL		= 1;

//================================================================//
// WorkerThreadInfo
//================================================================//
class WorkerThreadInfo {
public:

	enum {
		LOADING_FLAG		= 0x01,
		RENDER_FLAG			= 0x02,
	};
	
private:

	static SDL_mutex*		sDisplayListMutex;
	//static SDL_GLContext	sSharedContext;

	SDL_Thread*		mThread;
	SDL_cond*		mCondition;
	SDL_mutex*		mConditionMutex;
	bool			mIsDone;
	
	SDL_GLContext	mContext;
	
	int				mMask;
	
	//----------------------------------------------------------------//
	static int ThreadMain ( void* data ) {

		SDL_LockMutex ( sDisplayListMutex );

		WorkerThreadInfo* info = ( WorkerThreadInfo* )data;

		SDL_GL_MakeCurrent ( sWindow, info->mContext );
		
		if ( info->mMask & LOADING_FLAG ) {
			AKUDisplayListEnable ( AKU_DISPLAY_LIST_LOADING );
		}
		
		if ( info->mMask & RENDER_FLAG ) {
//			AKUDetectGfxContext ();
			AKUDisplayListEnable ( AKU_DISPLAY_LIST_DRAWING );
		}

		SDL_UnlockMutex ( sDisplayListMutex );

		int loadUpdateCounter = 0;
		int renderUpdateCounter = 0;

		while ( info->mIsDone == false ) {
		
			SDL_LockMutex ( info->mConditionMutex );
			SDL_CondWait ( info->mCondition, info->mConditionMutex );
			SDL_UnlockMutex ( info->mConditionMutex );
			
			
			if ( loadUpdateCounter >= ( LOAD_UPDATE_INTERVAL - 1 )) {
				loadUpdateCounter = 0;
			
				if ( info->mMask & LOADING_FLAG ) {
				
					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
					AKUDisplayListProcess ( AKU_DISPLAY_LIST_LOADING );
					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
				}
			}
			
			if ( renderUpdateCounter >= ( RENDER_UPDATE_INTERVAL - 1 )) {
				renderUpdateCounter = 0;
			
				if ( info->mMask & RENDER_FLAG ) {
					
					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
					AKUDisplayListProcess ( AKU_DISPLAY_LIST_DRAWING );
					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
					
					SDL_GL_SwapWindow ( sWindow );
				}
			}
			
			loadUpdateCounter++;
			renderUpdateCounter++;
		}
		
		SDL_GL_DeleteContext ( info->mContext );
		
		return 0;
	}

public:

	//----------------------------------------------------------------//
	static void DisplayListBeginPhase ( int list ) {
	
		SDL_LockMutex ( sDisplayListMutex );
		AKUDisplayListBeginPhase ( list );
		SDL_UnlockMutex ( sDisplayListMutex );
	}
	
	//----------------------------------------------------------------//
	static void DisplayListEndPhase ( int list ) {
	
		SDL_LockMutex ( sDisplayListMutex );
		AKUDisplayListEndPhase ( list );
		SDL_UnlockMutex ( sDisplayListMutex );
	}

	//----------------------------------------------------------------//
	void Signal () {
	
		bool hasContent = false;
		
		if (( this->mMask & LOADING_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_LOADING )) {
			hasContent = true;
		}
		
		if (( this->mMask & RENDER_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_DRAWING )) {
			hasContent = true;
		}
	
		if ( hasContent ) {
			SDL_LockMutex ( this->mConditionMutex );
			SDL_CondSignal ( this->mCondition );
			SDL_UnlockMutex ( this->mConditionMutex );
		}
	}
	
	//----------------------------------------------------------------//
	void Start ( int mask, const char* name ) {

		this->mContext = SDL_GL_CreateContext ( sWindow );

		this->mMask					= mask;
		this->mCondition			= SDL_CreateCond ();
		this->mConditionMutex		= SDL_CreateMutex ();
		this->mThread				= SDL_CreateThread ( WorkerThreadInfo::ThreadMain, name, this );
	}
	
	//----------------------------------------------------------------//
	void Stop () {
	
		SDL_LockMutex ( this->mConditionMutex );
		this->mIsDone = true;
		SDL_CondSignal ( this->mCondition );
		SDL_UnlockMutex ( this->mConditionMutex );
		SDL_WaitThread ( this->mThread, 0 );
	}
	
	//----------------------------------------------------------------//
	WorkerThreadInfo () :
		mThread ( 0 ),
		mCondition ( 0 ),
		mConditionMutex ( 0 ),
		mIsDone ( false ) {
		
		if ( !sDisplayListMutex ) {
			sDisplayListMutex = SDL_CreateMutex ();
		}
	}
};

SDL_mutex* WorkerThreadInfo::sDisplayListMutex = 0;

//================================================================//
// SDLHostGLMultiThread
//================================================================//

//----------------------------------------------------------------//
SDLHostGLMultiThread::SDLHostGLMultiThread () :
	mSimUpdateCounter ( 0 ) {
}

//----------------------------------------------------------------//
SDLHostGLMultiThread::~SDLHostGLMultiThread () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void SDLHostGLMultiThread::SDLAbstractHost_MainLoopDidFinish () {

	loadingThread.Stop ();
	renderThread.Stop ();
}

//----------------------------------------------------------------//
void SDLHostGLMultiThread::SDLAbstractHost_MainLoopDidStart () {
	
	WorkerThreadInfo loadingThread;
	loadingThread.Start ( WorkerThreadInfo::LOADING_FLAG, "Loading Thread" );

	WorkerThreadInfo renderThread;
	renderThread.Start ( WorkerThreadInfo::RENDER_FLAG, "Render Thread" );

	SDL_GL_MakeCurrent ( this->mWindow, NULL );
}

//----------------------------------------------------------------//
void SDLHostGLMultiThread::SDLAbstractHost_Render () {
	
	if ( this->mSimUpdateCounter >= ( SIM_UPDATE_INTERVAL - 1 )) {
		this->mSimUpdateCounter = 0;
	
		WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_LOGIC_PHASE );
		AKURender ();
		WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_LOGIC_PHASE );

		loadingThread.Signal ();
		renderThread.Signal ();
	}
	this->mSimUpdateCounter++;
}
