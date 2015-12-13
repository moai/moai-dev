//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "MOAICommandThread.h"

//================================================================//
// MOAIViewRenderThread
//================================================================//
@interface MOAIViewRenderThread : MOAICommandThread {
@private
}

    @property ( atomic ) EAGLContext* eaglContext;

    //----------------------------------------------------------------//
    -( void )       create                      :( CAEAGLLayer* )layer :( int )multisample;
    -( void )       presentFrame;
    -( void )       render;
    -( void )       resize                      :( CAEAGLLayer* )layer;
    -( void )       shutdown;

@end
