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
    
    void                ( ^mCommand )( void );
    
    BOOL                mIsRunning;
    BOOL                mIsBusy;
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
        
        [ mCommandCondition lock ];
        
        if ( !mIsRunning ) {

            mThread = [[ NSThread alloc ] initWithTarget:self selector:@selector ( main: ) object:nil ];
            
            [ mThread start ];
            
            while ( !mIsRunning ) {
                [ mCommandCondition wait ]; // unlock the command condition and wait for the signal
            }
        }
        
        mCommand = command;
        [ mCommandCondition signal ];
        
        if ( waitDone ) {
            while ( mCommand ) {
                [ mCommandCondition wait ]; // unlock the command condition and wait for the signal
            }
        }
        
        [ mCommandCondition unlock ];
    }

    //----------------------------------------------------------------//
	-( id ) init {
		
		if ( self = [ super init ]) {
        
            mCommandCondition = [[ NSCondition alloc ] init ];
		}
        return self;
	}

    //----------------------------------------------------------------//
    -( void ) main :( id* )argument {
        
        [ mCommandCondition lock ];
        
        mIsRunning = YES;
        [ mCommandCondition signal ];
        
        while ( mIsRunning ) {
            
            while ( !mCommand ) {
                [ mCommandCondition wait ]; // unlock the command condition and wait for the signal
            }
            
            [ self setIsBusy:YES ];

            mCommand ();
            mCommand = 0;
            
            [ self setIsBusy:NO ];
            [ mCommandCondition signal ];
        }
        
        [ mCommandCondition unlock ];
    }

    //----------------------------------------------------------------//
    -( void ) start {
    
        void ( ^command )( void ) = ^{
        };
        
        [ self command:command :YES ];
    }

    //----------------------------------------------------------------//
    -( void ) stop {
    
        void ( ^command )( void ) = ^{
            mIsRunning = nil;
        };
        
        [ self command:command :YES ];
    }

@end
