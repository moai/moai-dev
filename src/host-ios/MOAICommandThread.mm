//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAICommandThread.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAICommandThread ()
//================================================================//
@interface MOAICommandThread () {
	
    NSThread*           mThread;
    
    NSCondition*        mCommandCondition;
    NSCondition*        mDoneCondition;
    
    void                ( ^mCommand )( void );
    
    BOOL                mIsRunning;
    BOOL                mIsBusy;
    BOOL                mIsDone;
}

	//----------------------------------------------------------------//
    -( void )           main            :( id* )argument;

@end

//================================================================//
// MOAICommandThread
//================================================================//
@implementation MOAICommandThread

    @synthesize isBusy = mIsBusy;

    //----------------------------------------------------------------//
    -( void ) command :( void (^)( void ))command :( BOOL )waitDone {
    
        if ( !command ) return;
        
        if ( !mIsRunning ) {
            mIsRunning = YES;
            mThread = [[ NSThread alloc ] initWithTarget:self selector:@selector ( main: ) object:nil ];
            [ mThread start ];
        }

        [ mDoneCondition lock ]; // block until done
        
        [ mCommandCondition lock ];
        mCommand = command;
        [ mCommandCondition signal ];
        
        if ( !waitDone ) {
            [ mDoneCondition unlock ]; // since we locked it, unlock it so command can proceed
        }
        
        [ mCommandCondition unlock ];
        
        if ( waitDone ) {
            while ( !mIsDone ) {
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
		}
        return self;
	}

    //----------------------------------------------------------------//
    -( void ) main :( id* )argument {
        
        [ mDoneCondition lock ];
        
        while ( mIsRunning ) {
        
            [ mCommandCondition lock ];
            [ mDoneCondition unlock ];
            
            while ( !mCommand ) {
                [ mCommandCondition wait ]; // unlock the command condition and wait for the signal
            }
            
            [ self setIsBusy:YES ];
            
            void ( ^command )( void ) = mCommand;
            mCommand = 0;
            mIsDone = NO;
            
            [ mDoneCondition lock ];
            [ mCommandCondition unlock ];
            
            command ();
            
            [ self setIsBusy:NO ];
            mIsDone = YES;
            
            [ mDoneCondition signal ];
        }
        
        [ mDoneCondition unlock ];
    }

    //----------------------------------------------------------------//
    -( void ) stop {
    
        void ( ^command )( void ) = ^{
            mIsRunning = nil;
        };
        
        [ self command:command :YES ];
    }

@end
