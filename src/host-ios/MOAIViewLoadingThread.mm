//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIViewLoadingThread.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIViewLoadingThread
//================================================================//
@implementation MOAIViewLoadingThread

    //----------------------------------------------------------------//
	-( id ) init {
		
		if ( self = [ super init ]) {
			//sDisplayListMutex = SDL_CreateMutex ();
		}
        return self;
	}

    //----------------------------------------------------------------//
    -( void ) main :( id* )argument {

        while ( YES ) {
        
            printf ( "OH HAI!\n" );
            [ NSThread sleepForTimeInterval:1 ];
        }

//		SDL_LockMutex ( sDisplayListMutex );
//
//		WorkerThreadInfo* info = ( WorkerThreadInfo* )data;
//
//		SDL_GL_MakeCurrent ( sWindow, info->mContext );
//		
//		if ( info->mMask & LOADING_FLAG ) {
//			AKUDisplayListEnable ( AKU_DISPLAY_LIST_LOADING );
//		}
//		
//		if ( info->mMask & RENDER_FLAG ) {
//			AKUDetectGfxContext ();
//			AKUDisplayListEnable ( AKU_DISPLAY_LIST_DRAWING );
//		}
//
//		SDL_UnlockMutex ( sDisplayListMutex );
//
//		int loadUpdateCounter = 0;
//		int renderUpdateCounter = 0;
//
//		while ( info->mIsDone == false ) {
//		
//			SDL_LockMutex ( info->mConditionMutex );
//			SDL_CondWait ( info->mCondition, info->mConditionMutex );
//			SDL_UnlockMutex ( info->mConditionMutex );
//			
//			
//			if ( loadUpdateCounter >= ( LOAD_UPDATE_INTERVAL - 1 )) {
//				loadUpdateCounter = 0;
//			
//				if ( info->mMask & LOADING_FLAG ) {
//				
//					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
//					AKUDisplayListProcess ( AKU_DISPLAY_LIST_LOADING );
//					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_LOADING_PHASE );
//				}
//			}
//			
//			if ( renderUpdateCounter >= ( RENDER_UPDATE_INTERVAL - 1 )) {
//				renderUpdateCounter = 0;
//			
//				if ( info->mMask & RENDER_FLAG ) {
//					
//					WorkerThreadInfo::DisplayListBeginPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
//					AKUDisplayListProcess ( AKU_DISPLAY_LIST_DRAWING );
//					WorkerThreadInfo::DisplayListEndPhase ( AKU_DISPLAY_LIST_DRAWING_PHASE );
//					
//					SDL_GL_SwapWindow ( sWindow );
//				}
//			}
//			
//			loadUpdateCounter++;
//			renderUpdateCounter++;
//		}
//		
//		SDL_GL_DeleteContext ( info->mContext );
//		
//		return 0;
    }

@end
