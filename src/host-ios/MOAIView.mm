//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIContextMgr.h"
#import "MOAIView.h"
#import "MOAIRenderer.h"
#import "MOAIRendererAsync.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <host-modules/aku_modules_ios.h>
#import <moai-ios/host.h>

//#define AKU_HOST_FULL_ASYNC

enum {
    MOAI_INPUT_DEVICE,
    TOTAL_INPUT_DEVICES,
};

enum {
    MOAI_TOUCH_SENSOR,
    TOTAL_SENSORS,
};

//================================================================//
// MOAIView ()
//================================================================//
@interface MOAIView () {

    AKUContextID        mAKUContext;
    NSTimeInterval      mAnimInterval;
    CADisplayLink*      mDisplayLink;
    
    BOOL                mGCDetected;
    BOOL				mSimStarted;
    
    id < MOAIRendererProtocol >   mRenderer;
}

	//----------------------------------------------------------------//
    -( void )       dummyFunc;
	-( void )       handleTouches           :( NSSet* )touches :( BOOL )down;
    -( void )       onUpdateAnim;
	-( void )       startAnimation;
	-( void )       stopAnimation;
@end

//================================================================//
// MOAIView
//================================================================//
@implementation MOAIView

    //----------------------------------------------------------------//
    +( void ) appInitialize {

        AKUAppInitialize ();

        static const int length = 255;
		char version [ length ];
		AKUGetMoaiVersion ( version, length );
		printf ( "%s\n", version );
    }

    //----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {
        ( void )application;
    
		AKUIosNotifyRemoteNotificationRegistrationComplete ( nil, error );
	}

    //----------------------------------------------------------------//
    +( BOOL ) application :( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
    
        AKUAppInitialize ();
        if ( AKUModulesIosApplicationDidFinishLaunchingWithOptions ( application, launchOptions ) == NO ) return NO;

        static const int length = 255;
		char version [ length ];
		AKUGetMoaiVersion ( version, length );
		printf ( "%s\n", version );
        
        return YES;
    }

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {
		( void )application;
        
		AKUIosNotifyRemoteNotificationReceived ( pushBundle );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
        ( void )application;
    
    	// TODO: why did we need this? why not do it in Lua?
    	//NSString* strData = [[[[deviceToken description]
		//	stringByReplacingOccurrencesOfString: @"<" withString: @""]
		//	stringByReplacingOccurrencesOfString: @">" withString: @""]
		//	stringByReplacingOccurrencesOfString: @" " withString: @""];
        //NSLog(@"%@", strData);
		AKUIosNotifyRemoteNotificationRegistrationComplete ( deviceToken, nil );
	}
		
    //----------------------------------------------------------------//
    -( BOOL ) application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {

        if ( AKUModulesIosApplicationOpenURL ( application, url, sourceApplication, annotation ) == NO ) {
            AKUIosOpenUrl ( url, sourceApplication );
        }
        return YES;
    }

	//----------------------------------------------------------------//
	-( void ) dealloc {
		
        AKUDeleteContext ( mAKUContext );
        [ mRenderer shutdown ];
	}
	
    //----------------------------------------------------------------//
    -( void ) dummyFunc {
        //dummy to fix weird input bug
    }

    //----------------------------------------------------------------//
    // Since iOS 8.0 both [[UIScreen mainScreen] bounds/applicationFrame] are interface oriented.
	// Before iOS 8.0, only the portrait "bounds" where returned.
	// The function below ensure Portrait bounds are returned.
    +( CGRect ) getScreenBoundsFromCurrentOrientation :( CGRect )bounds {
    
        bool lessThaniOS8 = [ MOAIView isSystemVersionLessThan:@"8.0" ];
        if ( lessThaniOS8 || UIInterfaceOrientationIsPortrait ([ UIApplication sharedApplication ].statusBarOrientation )) {
            return bounds;
        }
        return CGRectMake ( bounds.origin.y, bounds.origin.x, bounds.size.height, bounds.size.width );
    }

    //----------------------------------------------------------------//
	-( int ) guessScreenDpi {
    
		float dpi;
		float scale = 1;

		if ([[ UIScreen mainScreen ] respondsToSelector:@selector ( scale )]) {
			scale = [[ UIScreen mainScreen ] scale];
		}
		if ( UI_USER_INTERFACE_IDIOM () == UIUserInterfaceIdiomPad ) {
			//Not working for iPad Mini, but appropriate solution doesn't exist yet
			dpi = 132 * scale;
		}
        else {
			dpi = 163 * scale;
		}
		return dpi;
	}

	//----------------------------------------------------------------//
	-( void ) handleTouches :( NSSet* )touches :( BOOL )down {
	
		for ( UITouch* touch in touches ) {
			
			CGPoint p = [ touch locationInView:self ];
			
			AKUEnqueueTouchEvent (
				MOAI_INPUT_DEVICE,
				MOAI_TOUCH_SENSOR,
				( int )( long ) touch, // use the address of the touch as a unique id
				down,
				p.x * [[ UIScreen mainScreen ] scale ],
				p.y * [[ UIScreen mainScreen ] scale ]
			);
		}
	}

    //----------------------------------------------------------------//
    -( id ) initWithCoder:( NSCoder* )aDecoder {
    
        // make sure to do self init here
        if ( self = [ super initWithCoder:aDecoder ]) {
        }
        return self;
    }

    //----------------------------------------------------------------//
    +( BOOL ) isSystemVersionLessThan :( NSString* )version {

        return ([[[ UIDevice currentDevice ] systemVersion ] compare:version options:NSNumericSearch ] == NSOrderedAscending );
    }

    //----------------------------------------------------------------//
	// You must implement this method
	+( Class ) layerClass {
		return [ CAEAGLLayer class ];
	}

    //----------------------------------------------------------------//
	-( void ) layoutSubviews {
    
	    // It is necessary to call super here
	    [ super layoutSubviews ];
	    
        [ mRenderer resize:( CAEAGLLayer* )self.layer ];
	}

    //----------------------------------------------------------------//
	-( void ) moaiInit {
        [self moaiInitWithMultisample:4 ];
    }

	//----------------------------------------------------------------//
	-( void ) moaiInitWithMultisample :( int )multisample {
    
        NSAssert ((( multisample == 1 ) || ( multisample == 4 )), @"Invalid multisample" );
    
		mAKUContext = AKUCreateContext ();
		AKUSetUserdata (( __bridge void* )self );
		
		AKUModulesContextInitialize ();
		
		AKUSetInputConfigurationName ( "iPhone" );

		AKUReserveInputDevices			( TOTAL_INPUT_DEVICES );
		AKUSetInputDevice				( MOAI_INPUT_DEVICE, "device" );
		
		AKUReserveInputDeviceSensors	( MOAI_INPUT_DEVICE, TOTAL_SENSORS );
		AKUSetInputDeviceTouch			( MOAI_INPUT_DEVICE, MOAI_TOUCH_SENSOR,		"touch" );
		
        CGRect screenRect = [ MOAIView getScreenBoundsFromCurrentOrientation:[[ UIScreen mainScreen ] bounds ]];
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		
		AKUSetScreenDpi ([ self guessScreenDpi ]);
        
        #if defined ( AKU_HOST_GFX_ASYNC ) || defined ( AKU_HOST_FULL_ASYNC )
        
            BOOL useLoadingThread = NO;
        
            #ifdef AKU_HOST_FULL_ASYNC
                useLoadingThread = YES;
            #endif
        
            mRenderer = [[ MOAIRendererAsync alloc ] initWithLayer :( CAEAGLLayer* )self.layer :multisample :useLoadingThread ];
        
        #else
        
            mRenderer = [[ MOAIRenderer alloc ] initWithLayer :( CAEAGLLayer* )self.layer :multisample ];
        
        #endif
        
        AKUModulesRunLuaAPIWrapper ();
        
        // start to run the moai thread immediately so it renders the view before returning from here
        // to get a chance to display a splash screen for example while the rest loads
        mAnimInterval = 1; // 1 for 60fps, 2 for 30fps
        
        [ self pause:false ];
	}

    //----------------------------------------------------------------//
	-( void ) onUpdateAnim {
        
        [ mRenderer presentFrame ];
        
        AKUSetContext ( mAKUContext );
        AKUModulesUpdate ();
        self.opaque = AKUIsGfxBufferOpaque () != 0;
        
        [ mRenderer render ];
        
        //sometimes the input handler will get 'locked out' by the render, this will allow it to run
        [ self performSelector:@selector( dummyFunc ) withObject:self afterDelay: 0 ];
	}
	
	//----------------------------------------------------------------//
	-( void ) pause :( BOOL )paused {
	
        AKUModulesPause ( paused );
    
		if ( paused ) {
			[ self stopAnimation ];
		}
		else {
			[ self startAnimation ];
		}
	}

	//----------------------------------------------------------------//
	-( void ) run :( NSString* )filename {
	
		AKUSetContext ( mAKUContext );
        AKULoadFuncFromFile ([ filename UTF8String ]);
		AKUCallFunc ();
	}

    //----------------------------------------------------------------//
    -( void ) setWorkingDirectory :( NSString* )path {
    
        AKUSetContext ( mAKUContext );
        AKUSetWorkingDirectory ([ path UTF8String ]);
    }

	//----------------------------------------------------------------//
	-( void ) startAnimation {
		
		if ( !mDisplayLink ) {
			CADisplayLink* aDisplayLink = [[ UIScreen mainScreen ] displayLinkWithTarget:self selector:@selector( onUpdateAnim )];
			[ aDisplayLink setFrameInterval:mAnimInterval ];
			[ aDisplayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ]; // or NSRunLoopCommonModes
			mDisplayLink = aDisplayLink;
		}
	}

	//----------------------------------------------------------------//
	-( void ) stopAnimation {

        [ mDisplayLink invalidate ];
        mDisplayLink = nil;
	}

	//----------------------------------------------------------------//
	-( void )touchesBegan:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
	//----------------------------------------------------------------//
	-( void )touchesCancelled:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )touches;
		( void )event;
		
		AKUEnqueueTouchEventCancel ( MOAI_INPUT_DEVICE, MOAI_TOUCH_SENSOR );
	}
	
	//----------------------------------------------------------------//
	-( void )touchesEnded:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :NO ];
	}

	//----------------------------------------------------------------//
	-( void )touchesMoved:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
@end
