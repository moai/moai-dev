//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <aku/AKU-luaext.h>
#include <aku/AKU-untz.h>

#import "LocationObserver.h"
#import "MoaiView.h"

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
	-( void )	handleTouches		:( NSSet* )touches :( BOOL )down;
	-( void )	moaiInit;
	-( void )	onUpdateAnim;
	-( void )	onUpdateHeading		:( LocationObserver* )observer;
	-( void )	onUpdateLocation	:( LocationObserver* )observer;
	-( void )	openContext;
	-( void )	setGlobalPaths;
	-( void )	startAnimation;
	-( void )	stopAnimation;

@end

//================================================================//
// AKU callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUHideLoadingScreenFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );
void	_AKUShowLoadingScreenFunc		();
void	_AKUShowSoftwareKeyboardFunc	();
void	_AKUStartGameLoopFunc			();

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {
}

//----------------------------------------------------------------//
void _AKUHideLoadingScreenFunc () {
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	( void )title;
	( void )width;
	( void )height;
	
}

//----------------------------------------------------------------//
void _AKUShowLoadingScreenFunc () {
}

//----------------------------------------------------------------//
void _AKUShowSoftwareKeyboardFunc () {
}

//----------------------------------------------------------------//
void _AKUStartGameLoopFunc () {
	
	MoaiView* moaiView = ( MoaiView* )AKUGetContextUser ();
	[ moaiView startAnimation ];
}

//================================================================//
// MoaiView
//================================================================//
@implementation MoaiView

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
	-( void ) dealloc {
	
		AKUDeleteContext ( mContext );
		
		[ super dealloc ];
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
		
		[ self beginDrawing ];
		
		AKUSetContext ( mAku );
		AKUResize ( mWidth, mHeight );
		AKURender ();

		[ self endDrawing ];
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
				p.x,
				p.y,
				[ touch tapCount ]
			);
		}
	}
	
	//----------------------------------------------------------------//
	-( id )init {
		
		self = [ super init ];
		if ( self ) {
		}
		return self;
	}

	//----------------------------------------------------------------//
	-( id ) initWithCoder:( NSCoder* )encoder {
	
		self = [ super initWithCoder:encoder ];
		if ( self ) {
		
			[ self moaiInit ];
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( id ) initWithFrame :( CGRect )frame {
	
		self = [ super initWithFrame:frame ];
		if ( self ) {
		
			[ self moaiInit ];
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( void ) moaiInit {
	
		mAku = AKUCreateContext ( self );
		
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuasocket ();
		
		AKUUntzInit ();
		
		AKUSetInputConfigurationName ( "iPhone" );

		AKUReserveInputDevices			( MoaiInputDeviceID::TOTAL );
		AKUSetInputDevice				( MoaiInputDeviceID::DEVICE, "device" );
		
		AKUReserveInputDeviceSensors	( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOTAL );
		AKUSetInputDeviceCompass		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::COMPASS,		"compass" );
		AKUSetInputDeviceLevel			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LEVEL,		"level" );
		AKUSetInputDeviceLocation		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LOCATION,		"location" );
		AKUSetInputDeviceTouch			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH,		"touch" );
		
		AKUSetFunc_EnterFullscreenMode	( _AKUEnterFullscreenModeFunc );
		AKUSetFunc_ExitFullscreenMode	( _AKUExitFullscreenModeFunc );
		AKUSetFunc_OpenWindow			( _AKUOpenWindowFunc );
		AKUSetFunc_StartGameLoop		( _AKUStartGameLoopFunc );
		
		[ self setGlobalPaths ];
		
		mAnimInterval = 1.0f / 60.0f;
		
		mLocationObserver = [[[ LocationObserver alloc ] init ] autorelease ];
		
		[ mLocationObserver setHeadingDelegate:self :@selector ( onUpdateHeading: )];
		[ mLocationObserver setLocationDelegate:self :@selector ( onUpdateLocation: )];
		
		UIAccelerometer* accel = [ UIAccelerometer sharedAccelerometer ];
		accel.delegate = self;
		accel.updateInterval = mAnimInterval;
	}
	
	//----------------------------------------------------------------//
	-( void ) onUpdateAnim {
		
		[ self openContext ];
		AKUSetContext ( mAku );
		AKUUpdate ();
		
		[ self drawView ];
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
	-( void ) openContext {
		
		if ([ EAGLContext currentContext ] != mContext ) {
			[ EAGLContext setCurrentContext:mContext ];
		}
	}
	
	//----------------------------------------------------------------//
	-( void ) run :( NSString* )filename {
	
		AKUSetContext ( mAku );
		AKURunScript ([ filename UTF8String ]);
	}
	
	//----------------------------------------------------------------//
	-( void ) setGlobalPaths {
	
		lua_State* L = AKUGetLuaState ();
		lua_newtable ( L );
		
		NSString* path;
		NSArray* paths;
		
		paths = NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES );
		path = [ paths objectAtIndex :0 ];
		NSLog ( @"%@", path );
		lua_pushstring ( L, [ path UTF8String ]);
		lua_setfield ( L, -2, "documents" );
		
		paths = NSSearchPathForDirectoriesInDomains ( NSCachesDirectory, NSUserDomainMask, YES );
		path = [ paths objectAtIndex:0 ];
		NSLog ( @"%@", path );
		lua_pushstring ( L, [ path UTF8String ]);
		lua_setfield ( L, -2, "caches" );
		
		path = [[ NSBundle mainBundle ] resourcePath ];
		NSLog ( @"%@", path );
		lua_pushstring ( L, [ path UTF8String ]);
		lua_setfield ( L, -2, "resources" );
		
		lua_setglobal ( L, "iphone" );
	}
	
	//----------------------------------------------------------------//
	-( void ) startAnimation {
		
		if ( mAnimInterval > 0.0 ) {
			
			if ( !mAnimTimer ) {
				
				mAnimTimer = [
					NSTimer
					scheduledTimerWithTimeInterval:mAnimInterval
					target:self
					selector:@selector( onUpdateAnim )
					userInfo:nil
					repeats:YES
				];
			}
		}
	}

	//----------------------------------------------------------------//
	-( void ) stopAnimation {
		
		if ( mAnimTimer && [ mAnimTimer isValid ]) {
			[ mAnimTimer invalidate ];
			mAnimTimer = nil;
		}
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
