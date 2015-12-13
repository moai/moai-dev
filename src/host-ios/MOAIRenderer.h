//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "MOAICommandThread.h"

//================================================================//
// MOAIRendererProtocol
//================================================================//
@protocol MOAIRendererProtocol < NSObject >

    //----------------------------------------------------------------//
    -( void )       presentFrame;
    -( void )       render;
    -( void )       resize                  :( CAEAGLLayer* )layer;
    -( void )       shutdown;

@end

//================================================================//
// MOAIRenderer
//================================================================//
@interface MOAIRenderer : NSObject < MOAIRendererProtocol > {
@private
}

    @property ( readonly, atomic ) EAGLContext* eaglContext;

    //----------------------------------------------------------------//
    -( id )         initWithLayer           :( CAEAGLLayer* )layer :( int )multisample;

@end
