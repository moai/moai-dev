//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIContextMgr.h"
#import "MOAIViewLoadingThread.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIViewLoadingThread ()
//================================================================//
@interface MOAIViewLoadingThread () {
    
    EAGLContext*		mEAGLContext;
}

	//----------------------------------------------------------------//

@end

//================================================================//
// MOAIViewLoadingThread
//================================================================//
@implementation MOAIViewLoadingThread

    //----------------------------------------------------------------//
    -( void ) create :( EAGLContext* )eaglContext {
    
        void ( ^command )( void ) = ^{
        
            AKUDisplayListEnable ( AKU_DISPLAY_LIST_LOADING );
        
            assert ( eaglContext );
            mEAGLContext = [[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:[ eaglContext sharegroup ]];
            assert ( mEAGLContext );
        };
        
        [ self command:command :YES ];
    }

    //----------------------------------------------------------------//
    -( void ) load {
    
        if ([ self isBusy ]) return;
        if ( !AKUDisplayListHasContent ( AKU_DISPLAY_LIST_LOADING )) return;
    
        void ( ^command )( void ) = ^{
        
            [ EAGLContext setCurrentContext:mEAGLContext ];
            
            [ MOAIContextMgr displayListBeginPhase:AKU_DISPLAY_LIST_LOADING_PHASE ];
            AKUDisplayListProcess ( AKU_DISPLAY_LIST_LOADING );
            [ MOAIContextMgr displayListEndPhase:AKU_DISPLAY_LIST_LOADING_PHASE ];
            
            [ EAGLContext setCurrentContext:nil ];
        };
        
        [ self command:command :NO ];
    }

@end
