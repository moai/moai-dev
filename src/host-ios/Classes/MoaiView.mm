//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

//extern "C" {
//	#include <lua.h>
//	#include <lauxlib.h>
//	#include <lualib.h>
//}

#import <host-modules/aku_modules.h>
#import <moai-iphone/AKU-iphone.h>

#import <moai-audiosampler/MOAIAudioSampler.h>
#import <moai-audiosampler/AKU-audiosampler.h>


#import "LocationObserver.h"
#import "MoaiView.h"
#import "ParticlePresets.h"

namespace MoaiInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace MoaiInputDeviceSensorID {
	enum {
		COMPASS,
		LEVEL,
		LOCATION,
		TOUCH,
		TOTAL,
	};
}

//================================================================//
// MoaiView ()
//================================================================//
@interface MoaiView ()

	//----------------------------------------------------------------//
	-( void )	drawView;
	-( void )	handleTouches		:( NSSet* )touches :( BOOL )down;
	-( void )	onUpdateAnim;
	-( void )	onUpdateHeading		:( LocationObserver* )observer;
	-( void )	onUpdateLocation	:( LocationObserver* )observer;
	-( void )	startAnimation;
	-( void )	stopAnimation;
    -( void )   dummyFunc;

@end

//================================================================//
// MoaiView
//================================================================//
@implementation MoaiView
    SYNTHESIZE	( GLint, width, Width );
    SYNTHESIZE	( GLint, height, Height );

	//----------------------------------------------------------------//
	-( void ) accelerometer:( UIAccelerometer* )acel didAccelerate:( UIAcceleration* )acceleration {
		( void )acel;
		
		AKUEnqueueLevelEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::LEVEL,
			( float )acceleration.x,
			( float )acceleration.y,
			( float )acceleration.z
		);
	}

    //----------------------------------------------------------------//
    -( AKUContextID ) akuInitialized {

        return mAku;
    }

	//----------------------------------------------------------------//
	-( void ) dealloc {
	
		AKUDeleteContext ( mContext );
		
		[ super dealloc ];
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
		
		[ self beginDrawing ];
		
		AKUSetContext ( mAku );
		AKURender ();

		[ self endDrawing ];
	}
	
    //----------------------------------------------------------------//
    -( void ) dummyFunc {
        //dummy to fix weird input bug
    }

	//----------------------------------------------------------------//
	-( void ) handleTouches :( NSSet* )touches :( BOOL )down {
	
		for ( UITouch* touch in touches ) {
			
			CGPoint p = [ touch locationInView:self ];
			
			AKUEnqueueTouchEvent (
				MoaiInputDeviceID::DEVICE,
				MoaiInputDeviceSensorID::TOUCH,
				( int )touch, // use the address of the touch as a unique id
				down,
				p.x * [[ UIScreen mainScreen ] scale ],
				p.y * [[ UIScreen mainScreen ] scale ]
			);
		}
	}
	
	//----------------------------------------------------------------//
	-( id )init {
		
        mAku = 0;
		self = [ super init ];
		if ( self ) {
		}
		return self;
	}

	//----------------------------------------------------------------//
	-( id ) initWithCoder:( NSCoder* )encoder {

        mAku = 0;
		self = [ super initWithCoder:encoder ];
		if ( self ) {
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( id ) initWithFrame :( CGRect )frame {

        mAku = 0;
		self = [ super initWithFrame:frame ];
		if ( self ) {
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( void ) moaiInit :( UIApplication* )application {
	
		mAku = AKUCreateContext ();
		AKUSetUserdata ( self );
		
		AKUModulesContextInitialize ();
		AKUAudioSamplerInit ();
        AKUModulesRunLuaAPIWrapper ();
		
		AKUSetInputConfigurationName ( "iPhone" );

		AKUReserveInputDevices			( MoaiInputDeviceID::TOTAL );
		AKUSetInputDevice				( MoaiInputDeviceID::DEVICE, "device" );
		
		AKUReserveInputDeviceSensors	( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOTAL );
		AKUSetInputDeviceCompass		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::COMPASS,		"compass" );
		AKUSetInputDeviceLevel			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LEVEL,		"level" );
		AKUSetInputDeviceLocation		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LOCATION,		"location" );
		AKUSetInputDeviceTouch			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH,		"touch" );
		
		CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;
		
		AKUSetScreenSize ( screenWidth, screenHeight );
		AKUSetScreenDpi([ self guessScreenDpi ]);
		AKUSetViewSize ( mWidth, mHeight );
		
        AKUSetFrameBuffer ( mFramebuffer );
		AKUDetectGfxContext ();
		
		mAnimInterval = 1; // 1 for 60fps, 2 for 30fps
		
		mLocationObserver = [[[ LocationObserver alloc ] init ] autorelease ];
		
		[ mLocationObserver setHeadingDelegate:self :@selector ( onUpdateHeading: )];
		[ mLocationObserver setLocationDelegate:self :@selector ( onUpdateLocation: )];
		
		UIAccelerometer* accel = [ UIAccelerometer sharedAccelerometer ];
		accel.delegate = self;
		accel.updateInterval = mAnimInterval / 60;
		
		// init aku
		AKUIphoneInit ( application );
		
		// add in the particle presets
		ParticlePresets ();
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
	-( void ) onUpdateAnim {
		
		[ self openContext ];
		AKUSetContext ( mAku );
		AKUModulesUpdate ();
		[ self drawView ];
        
        //sometimes the input handler will get 'locked out' by the render, this will allow it to run
        [ self performSelector: @selector(dummyFunc) withObject:self afterDelay: 0 ];
	}
	
	//----------------------------------------------------------------//
	-( void ) onUpdateHeading :( LocationObserver* )observer {
	
		AKUEnqueueCompassEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::COMPASS,
			( float )[ observer heading ]
		);
	}
	
	//----------------------------------------------------------------//
	-( void ) onUpdateLocation :( LocationObserver* )observer {
	
		AKUEnqueueLocationEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::LOCATION,
			[ observer longitude ],
			[ observer latitude ],
			[ observer altitude ],
			( float )[ observer hAccuracy ],
			( float )[ observer vAccuracy ],
			( float )[ observer speed ]
		);
	}
	
	//----------------------------------------------------------------//
	-( void ) pause :( BOOL )paused {
	
		if ( paused ) {
			AKUModulesPause ();
			[ self stopAnimation ];
		}
		else {
			[ self startAnimation ];
			AKUModulesResume ();
		}
	}
	
	//----------------------------------------------------------------//
	-( void ) run :( NSString* )filename {
	
		AKUSetContext ( mAku );
		AKURunScript ([ filename UTF8String ]);
	}
	
	//----------------------------------------------------------------//
	-( void ) startAnimation {
		
		if ( !mDisplayLink ) {
			CADisplayLink* aDisplayLink = [[ UIScreen mainScreen ] displayLinkWithTarget:self selector:@selector( onUpdateAnim )];
			[ aDisplayLink setFrameInterval:mAnimInterval ];
			[ aDisplayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ];
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
		
		AKUEnqueueTouchEventCancel ( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH );
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
