//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAICommandThread.h"
#import "MOAIContextMgr.h"
#import "MOAIRendererAsync.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//================================================================//
// MOAIRendererAsync ()
//================================================================//
@interface MOAIRendererAsync () {
    
    MOAICommandThread*  mLoadingThread;
    MOAICommandThread*  mDrawingThread;
    
    MOAIRenderer*       mRenderer;
    
    EAGLContext*		mLoadingContext;
}

    @property ( readonly, nonatomic ) int multisample;
    @property ( readonly, nonatomic ) BOOL multisampleEnabled;

	//----------------------------------------------------------------//
    -( void )       load;

@end

//================================================================//
// MOAIRendererAsync
//================================================================//
@implementation MOAIRendererAsync

    //----------------------------------------------------------------//
    -( id ) initWithLayer :( CAEAGLLayer* )layer :( int )multisample :( BOOL )useLoadingThread {
        
        self = [ super init];
		if ( self != nil ) {
        
            mDrawingThread = [[ MOAICommandThread alloc ] init ];
            
            void ( ^command )( void ) = ^{
            
                AKUDisplayListEnable ( AKU_DISPLAY_LIST_DRAWING );
                mRenderer = [[ MOAIRenderer alloc ] initWithLayer :layer :multisample ];
                
                if ( useLoadingThread ) {
                
                    AKUDisplayListEnable ( AKU_DISPLAY_LIST_LOADING );
                    mLoadingContext = [[ EAGLContext alloc ] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:[[ mRenderer eaglContext ] sharegroup ]];
                    
                    mLoadingThread = [[ MOAICommandThread alloc ] init ];
                    [ mLoadingThread start ];
                }
            };
            
            [ mDrawingThread command:command :YES ];
        }
        return self;
    }

    //----------------------------------------------------------------//
    -( void ) load {
        
        if ( !mLoadingThread ) return;
        if ([ mLoadingThread isBusy ]) return;
        if ( !AKUDisplayListHasContent ( AKU_DISPLAY_LIST_LOADING )) return;
        
        void ( ^command )( void ) = ^{
            
            [ EAGLContext setCurrentContext:mLoadingContext ];
            
            [ MOAIContextMgr displayListBeginPhase:AKU_DISPLAY_LIST_LOADING_PHASE ];
            AKUDisplayListProcess ( AKU_DISPLAY_LIST_LOADING );
            [ MOAIContextMgr displayListEndPhase:AKU_DISPLAY_LIST_LOADING_PHASE ];
            
            [ EAGLContext setCurrentContext:nil ];
        };
        
        [ mLoadingThread command:command :NO ];
    }

    //----------------------------------------------------------------//
    -( void ) presentFrame {
    
        if ([ mDrawingThread isBusy ]) return;
    
        void ( ^command )( void ) = ^{
            [ mRenderer presentFrame ];
        };
        
        [ mDrawingThread command:command :YES ];
    }

    //----------------------------------------------------------------//
    -( void ) render {
        
        [ MOAIContextMgr displayListBeginPhase :AKU_DISPLAY_LIST_LOGIC_PHASE ];
        AKURender ();
        [ MOAIContextMgr displayListEndPhase :AKU_DISPLAY_LIST_LOGIC_PHASE ];
        
        [ self load ];
        
        if ([ mDrawingThread isBusy ]) return;
        if ( !AKUDisplayListHasContent ( AKU_DISPLAY_LIST_DRAWING )) return;
        
        void ( ^command )( void ) = ^{
            [ mRenderer render ];
        };
        
        [ mDrawingThread command:command :NO ];
    }

    //----------------------------------------------------------------//
    -( void ) resize :( CAEAGLLayer* )layer {
    
        void ( ^command )( void ) = ^{
            [ mRenderer resize :layer ];
        };
        
        [ mDrawingThread command:command :YES ];
    }

    //----------------------------------------------------------------//
    -( void ) shutdown {
    
        void ( ^command )( void ) = ^{
            
            [ mRenderer shutdown ];
        };
        
        [ mDrawingThread command:command :YES ];
        [ mDrawingThread stop ];
    }

@end
