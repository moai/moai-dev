// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moaiext-iphone/MOAIMoviePlayerIOS.h>

// TODO:
// Callbacks for started, stopped.
// Allow orientation control.
// MoaiView overlay.
// Allow on-screen controls to be specified.
// Support playing video over WIFI only.
// Support multiple videos to be played in order.

#define DEGREES_TO_RADIANS(x) (M_PI * (x) / 180.0)

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize the video player with the URL of a video to play.
	
	@in		string 	url				The URL of the video to play.
	@out	nil
*/
int MOAIMoviePlayerIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* url = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	
	MOAIMoviePlayerIOS::Get ().mInitialOrientation = [[ UIApplication sharedApplication ] statusBarOrientation ];

	MOAIMoviePlayerIOS::Get ().Reset ();

	MOAIMoviePlayerIOS::Get ().mMoviePlayer = [[ MPMoviePlayerController alloc ] initWithContentURL:[ NSURL URLWithString:url ]];
	
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setShouldAutoplay:NO ];
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setFullscreen:YES ];
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setControlStyle:MPMovieControlStyleNone ];
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setScalingMode:MPMovieScalingModeAspectFit ];      
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setUseApplicationAudioSession:NO ];

	[[ NSNotificationCenter defaultCenter ] addObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate selector:@selector( videoLoadedCallback: ) name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer];

	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer prepareToPlay ];

	return 0;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Play the video as soon as playback is ready.
	
	@out	nil
*/
int MOAIMoviePlayerIOS::_play ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[[ NSNotificationCenter defaultCenter ] addObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate selector:@selector( videoFinishedCallback: ) name:MPMoviePlayerPlaybackDidFinishNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer];

	MOAIMoviePlayerIOS::Get ().mLastOrientation = UIInterfaceOrientationLandscapeRight;
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setTransform:CGAffineTransformMakeRotation( DEGREES_TO_RADIANS( 90 ))];
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setFrame:[[ UIApplication sharedApplication ] keyWindow ].bounds ];
	
	if ( ![[ UIApplication sharedApplication ] isStatusBarHidden ]) {
		
		[[ UIApplication sharedApplication ] setStatusBarOrientation:MOAIMoviePlayerIOS::Get ().mLastOrientation ];
	}

	[[[ UIApplication sharedApplication ] keyWindow ] addSubview:MOAIMoviePlayerIOS::Get ().mMoviePlayer.view ];
	[[[ UIApplication sharedApplication ] keyWindow ] bringSubviewToFront:MOAIMoviePlayerIOS::Get ().mMoviePlayer.view ];

//	[[ NSNotificationCenter defaultCenter ] addObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate selector:@selector( deviceOrientationChanged: ) name:UIDeviceOrientationDidChangeNotification object:nil ];
//	[[ UIDevice currentDevice ] beginGeneratingDeviceOrientationNotifications ];

	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer setShouldAutoplay:YES ];

	if ([ MOAIMoviePlayerIOS::Get ().mMoviePlayer isPreparedToPlay ]) {

		[ MOAIMoviePlayerIOS::Get ().mMoviePlayer play ];	
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pause
	@text	Pause video playback.
	
	@out	nil
*/
int MOAIMoviePlayerIOS::_pause ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer pause ];
	
	[[ NSNotificationCenter defaultCenter ] removeObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate name:MPMoviePlayerPlaybackDidFinishNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMoviePlayerIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIMoviePlayerIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stop video playback and reset the video player.
	
	@out	nil
*/
int MOAIMoviePlayerIOS::_stop ( lua_State* L ) {
	
	MOAILuaState state ( L );

	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer stop ];
	
	MOAIMoviePlayerIOS::Get ().Reset ();
	
	return 0;
}

//================================================================//
// MOAIMoviePlayerIOS
//================================================================//

//----------------------------------------------------------------//
MOAIMoviePlayerIOS::MOAIMoviePlayerIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	mMoviePlayer = nil;
	mPlaybackDelegate = [[ MOAIMoviePlayerPlaybackDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIMoviePlayerIOS::~MOAIMoviePlayerIOS () {

	[ mMoviePlayer release ];
	[ mPlaybackDelegate release ];
}

//----------------------------------------------------------------//
void MOAIMoviePlayerIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	luaL_Reg regTable[] = {
		{ "init",			_init },
		{ "pause",			_pause },
		{ "play",			_play },
		{ "setListener",	_setListener },
		{ "stop",			_stop },
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMoviePlayerIOS::Reset () {
	
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view removeFromSuperview ];
    
	[[ NSNotificationCenter defaultCenter ] removeObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer ];
	[[ NSNotificationCenter defaultCenter ] removeObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate name:MPMoviePlayerPlaybackDidFinishNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer];	

    // [[ NSNotificationCenter defaultCenter ] removeObserver:MOAIMoviePlayerIOS::Get ().mPlaybackDelegate name:UIDeviceOrientationDidChangeNotification object:nil ];        
    // [[ UIDevice currentDevice ] endGeneratingDeviceOrientationNotifications ];

	if ( ![[ UIApplication sharedApplication ] isStatusBarHidden ]) {
		
		[[ UIApplication sharedApplication ] setStatusBarOrientation:MOAIMoviePlayerIOS::Get ().mInitialOrientation ];
	}
	
	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer release ];
	MOAIMoviePlayerIOS::Get ().mMoviePlayer = nil;
}

@implementation MOAIMoviePlayerPlaybackDelegate

	- ( void ) videoLoadedCallback:( NSNotification * )notification {
		
		UNUSED ( notification );

		[[ NSNotificationCenter defaultCenter ] removeObserver:self name:MPMediaPlaybackIsPreparedToPlayDidChangeNotification object:MOAIMoviePlayerIOS::Get ().mMoviePlayer ];
	}
	
	- ( void ) videoFinishedCallback:( NSNotification * )notification {
		
		UNUSED ( notification );

		MOAIMoviePlayerIOS::Get ().Reset ();
	}
	
	- ( void ) deviceOrientationChanged:( NSNotification * )notification {
		
		UNUSED ( notification );

		// if ([[ UIDevice currentDevice ] orientation ] == UIDeviceOrientationLandscapeLeft ) {
		// 
		// 	MOAIMoviePlayerIOS::Get ().mLastOrientation = UIInterfaceOrientationLandscapeRight;
		// 	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setTransform:CGAffineTransformMakeRotation( DEGREES_TO_RADIANS( 90 ))];
		// 	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setFrame:[[ UIApplication sharedApplication ] keyWindow ].bounds ];
		// } else if ([[ UIDevice currentDevice ] orientation ] == UIDeviceOrientationLandscapeRight ) {
		// 
		// 	MOAIMoviePlayerIOS::Get ().mLastOrientation = UIInterfaceOrientationLandscapeLeft;
		// 	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setTransform:CGAffineTransformMakeRotation( DEGREES_TO_RADIANS( -90 ))];
		// 	[ MOAIMoviePlayerIOS::Get ().mMoviePlayer.view setFrame:[[ UIApplication sharedApplication ] keyWindow ].bounds ];
		// }
		// 
		// if ( ![[ UIApplication sharedApplication ] isStatusBarHidden ]) {
		// 
		// 	[[ UIApplication sharedApplication ] setStatusBarOrientation:MOAIMoviePlayerIOS::Get ().mLastOrientation ];
		// }
	}

@end