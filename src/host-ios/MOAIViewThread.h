//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAIViewThread
//================================================================//
@interface MOAIViewThread : NSObject {
@protected

	NSThread*           mThread;
	bool                mIsDone;
	EAGLContext*		mEAGLContext;
	int                 mMask;
}

    //----------------------------------------------------------------//
    -( void )           command         :( void (^)( void ))command :( BOOL )waitStart :( BOOL )waitDone;
    -( void )           lock;
    //-( void )           onCommand       :( int )command;
    //-( void )           onStart;
    -( void )           signal;
    -( void )           start;
    -( void )           stop;
    -( void )           unlock;

@end
