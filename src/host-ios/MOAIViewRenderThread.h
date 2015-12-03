//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "MOAIViewThread.h"

//================================================================//
// MOAIViewRenderThread
//================================================================//
@interface MOAIViewRenderThread : MOAIViewThread {
@private
}

    @property ( readonly, nonatomic ) int multisample;
    @property ( readonly, nonatomic ) BOOL multisampleEnabled;

    //----------------------------------------------------------------//
    -( void )       create                      :( CAEAGLLayer* )layer :( int )multisample;
    -( void )       displayListBeginPhase       :( int )list;
    -( void )       displayListEndPhase         :( int )list;
    -( void )       presentFrame;
    -( void )       render;
    -( void )       resize                      :( CAEAGLLayer* )layer;
    -( void )       shutdown;

@end
