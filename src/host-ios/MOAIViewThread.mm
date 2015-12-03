//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIViewThread.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIViewThread ()
//================================================================//
@interface MOAIViewThread () {
	
    BOOL                mIsRunning;
    //int					mCommand;
    
    NSCondition*        mCommandCondition;
    NSCondition*        mDoneCondition;
    
    NSLock*             mLock;
    
    void                ( ^mCommand )( void );
}

	//----------------------------------------------------------------//
    -( void )           main            :( id* )argument;

@end

//================================================================//
// MOAIViewThread
//================================================================//
@implementation MOAIViewThread

    //----------------------------------------------------------------//
    -( void ) command :( void (^)( void ))command :( BOOL )waitStart :( BOOL )waitDone {
    
        if ( !command ) return;
    
        if ( waitStart ) {
            [ mDoneCondition lock ]; // block until done
        }
        
        [ mCommandCondition lock ];
        mCommand = command;
        [ mCommandCondition signal ];
        
        if ( !( waitStart && waitDone )) {
        
            if ( waitStart ) {
                [ mDoneCondition unlock ]; // since we locked it, unlock it so command can proceed
            }
            
            if ( waitDone ) {
                [ mDoneCondition lock ]; // this will prevent the command from starting until we wait
            }
        }
        
        [ mCommandCondition unlock ];
        
        if ( waitDone ) {
            while ( mCommand ) {
                [ mDoneCondition wait ]; // unlock the done condition and wait for the signal
            }
            [ mDoneCondition unlock ];
        }
    }

    //----------------------------------------------------------------//
	-( id ) init {
		
		if ( self = [ super init ]) {
        
            mCommandCondition = [[ NSCondition alloc ] init ];
            mDoneCondition = [[ NSCondition alloc ] init ];
            mLock = [[ NSLock alloc ] init ];
            
			//sDisplayListMutex = SDL_CreateMutex ();
		}
        return self;
	}

    //----------------------------------------------------------------//
    -( void ) lock {
    
        [ mLock lock ];
    }

    //----------------------------------------------------------------//
    -( void ) main :( id* )argument {
        
        printf ( "INITTTING THEARD MAIN YAYYY!\n" );
        
        [ mDoneCondition lock ];
        
        while ( mIsRunning ) {
        
            [ mCommandCondition lock ];
            [ mDoneCondition unlock ];
            
            while ( !mCommand ) {
                [ mCommandCondition wait ]; // unlock the command condition and wait for the signal
            }
            
            void ( ^command )( void ) = mCommand;
            mCommand = 0;
            
            [ mDoneCondition lock ];
            [ mCommandCondition unlock ];
            
            command ();
            
            [ mDoneCondition signal ];
        }
        
        [ mDoneCondition unlock ];
    }

    //----------------------------------------------------------------//
//    -( void ) onCommand :( int )command {
//    }

	//----------------------------------------------------------------//
	-( void ) signal {
	
//		bool hasContent = false;
//		
//		if (( this->mMask & LOADING_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_LOADING )) {
//			hasContent = true;
//		}
//		
//		if (( this->mMask & RENDER_FLAG ) && AKUDisplayListHasContent ( AKU_DISPLAY_LIST_DRAWING )) {
//			hasContent = true;
//		}
//	
//		if ( hasContent ) {
//			SDL_LockMutex ( this->mConditionMutex );
//			SDL_CondSignal ( this->mCondition );
//			SDL_UnlockMutex ( this->mConditionMutex );
//		}
	}
	
	//----------------------------------------------------------------//
	-( void ) start {

        mIsRunning = YES;

        mThread = [[ NSThread alloc ] initWithTarget:self selector:@selector ( main: ) object:nil ];
        [ mThread start ];

//		this->mContext = SDL_GL_CreateContext ( sWindow );
//		SDL_GL_SetSwapInterval ( 1 );
//
//		this->mMask					= mask;
//		this->mCondition			= SDL_CreateCond ();
//		this->mConditionMutex		= SDL_CreateMutex ();
//		this->mThread				= SDL_CreateThread ( WorkerThreadInfo::ThreadMain, name, this );
	}
	
	//----------------------------------------------------------------//
	-( void ) stop {
	
//		SDL_LockMutex ( this->mConditionMutex );
//		this->mIsDone = true;
//		SDL_CondSignal ( this->mCondition );
//		SDL_UnlockMutex ( this->mConditionMutex );
//		SDL_WaitThread ( this->mThread, 0 );
	}

    //----------------------------------------------------------------//
    -( void ) unlock {
    
        [ mLock unlock ];
    }

@end
