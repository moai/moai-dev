//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "MOAIRenderer.h"

//================================================================//
// MOAIRendererAsync
//================================================================//
@interface MOAIRendererAsync : NSObject < MOAIRendererProtocol > {
@private
}

    //----------------------------------------------------------------//
    -( id )         initWithLayer               :( CAEAGLLayer* )layer :( int )multisample :( BOOL )useLoadingThread;

@end
