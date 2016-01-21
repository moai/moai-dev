//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAICommandThread
//================================================================//
@interface MOAICommandThread : NSObject {
@protected
}

    @property ( atomic ) BOOL isBusy;

    //----------------------------------------------------------------//
    -( void )           command         :( void (^)( void ))command :( BOOL )waitDone; // calling thread will *block* until prev command is finished (if any). may optionally block until new command is finished.
    -( void )           start;
    -( void )           stop;

@end
