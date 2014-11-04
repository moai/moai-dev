//----------------------------------------------------------------//
// Copyright (c) 2014 Plumzi, Inc.
// All Rights Reserved.
//----------------------------------------------------------------//

#import "MOAIView.h"

#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import <host-modules/aku_modules.h>
#import <moai-ios/host.h>

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
    AKUContextID                mAKUContext;
    NSTimeInterval              mAnimInterval;
    CADisplayLink*              mDisplayLink;
    CMMotionManager*            mMotionManager;
}

	//----------------------------------------------------------------//
	-( void )	drawView;
    -( void )   dummyFunc;
	-( void )	handleTouches		:( NSSet* )touches :( BOOL )down;
	-( void )	onUpdateAnim;
	-( void )	startAnimation;
	-( void )	stopAnimation;
@end

//================================================================//
// MOAIView
//================================================================//
@implementation MOAIView

    static bool gOpaque = YES;

    //----------------------------------------------------------------//
    +( void ) appFinalize {
        AKUAppFinalize ();
    }

    //----------------------------------------------------------------//
    +( void ) appInitialize {
        AKUAppInitialize ();
    }

    //----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {
        ( void )application;
    
		AKUIosNotifyRemoteNotificationRegistrationComplete ( nil, error );
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
	-( void ) applicationDidBecomeActive:( UIApplication* )application {
        ( void )application;
    
        AKUIosDidBecomeActive ();
		[ self pause:NO ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationDidEnterBackground:( UIApplication* )application {
        ( void )application;
    
        AKUIosDidEnterBackground ();
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillEnterForeground:( UIApplication* )application {
        ( void )application;
    
        AKUIosWillEnterForeground ();
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillResignActive:( UIApplication* )application {
        ( void )application;
    
		// pause moai view
		[ self pause:YES ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {
        ( void )application;
        
		AKUIosWillResignActive ();
		AKUAppFinalize ();
	}
		
    //----------------------------------------------------------------//
    -( BOOL ) application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {
        ( void )application;
        ( void )sourceApplication;
        ( void )annotation;

        AKUIosOpenUrl ( url, sourceApplication );
        return YES;
    }

	//----------------------------------------------------------------//
	-( void ) dealloc {
	
		AKUDeleteContext ( mAKUContext );
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
        
        self.opaque = AKUIsGfxBufferOpaque () != 0;
		
		[ self beginDrawing ];
		
		AKUSetContext ( mAKUContext );
		AKURender ();

		[ self endDrawing ];
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
		if ([[ UIScreen mainScreen ] respondsToSelector:@selector(scale) ]) {
			scale = [[ UIScreen mainScreen ] scale];
		}
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
			//Not working for iPad Mini, but appropriate solution doesn't exist yet
			dpi = 132 * scale;
		}else{
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
				( int ) (long) touch, // use the address of the touch as a unique id
				down,
				p.x * [[ UIScreen mainScreen ] scale ],
				p.y * [[ UIScreen mainScreen ] scale ]
			);
		}
	}

    //----------------------------------------------------------------//
    +( BOOL ) isSystemVersionLessThan :( NSString* )version {

        return ([[[ UIDevice currentDevice ] systemVersion ] compare:version options:NSNumericSearch ] == NSOrderedAscending );
    }

	//----------------------------------------------------------------//
	-( void ) moaiInit :( UIApplication* )application {
        ( void )application;
	
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
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;
		
		AKUSetScreenDpi([ self guessScreenDpi ]);
		
		[ self createContext ];
        
        AKUModulesRunLuaAPIWrapper ();
        
        // start to run the moai thread immediately so it renders the view before returning from here
        // to get a chance to display a splash screen for example while the rest loads
        mAnimInterval = 1; // 1 for 60fps, 2 for 30fps
        [ self pause:false ];
	}

    //----------------------------------------------------------------//
	-( void ) onUpdateAnim {
		
		AKUSetContext ( mAKUContext );
		AKUModulesUpdate ();
        
		self.opaque = AKUIsGfxBufferOpaque () != 0;
		
		[ self beginDrawing ];
		AKUSetContext ( mAKUContext );
		AKURender ();
		[ self endDrawing ];
        
        //sometimes the input handler will get 'locked out' by the render, this will allow it to run
        [ self performSelector: @selector(dummyFunc) withObject:self afterDelay: 0 ];
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
