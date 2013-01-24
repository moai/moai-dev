// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moaiext-iphone/MOAIAppIOS.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>
#import <moaiext-iphone/MOAITakeCameraListener.h>

//================================================================//
// lua
//================================================================//

/** @name _takeCamera
	@text Allows to pick a photo from the CameraRoll or from the Camera
	@in function	callback
	@in NSUInteger	input camera source
	@in int			if device is an ipad x coordinate of Popover
	@in int			if device is an ipad y coordinate of Popover
	@in int			if device is an ipad width coordinate of Popover
	@in int			if device is an ipad height coordinate of Popover

 */
 
int MOAIAppIOS::_takeCamera( lua_State* L ) {
	
	int x, y, width, height = 0;
	NSUInteger sourceType;
	
	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TFUNCTION )) {
		MOAIAppIOS::Get ().mOnTakeCameraCallback.SetStrongRef ( state, 1 );
	}
	
	sourceType = state.GetValue < NSUInteger >( 2, 0 );
	x = state.GetValue < int >( 3, 0 );
	y = state.GetValue < int >( 4, 0 );
	width = state.GetValue < int >( 5, 0 );
	height = state.GetValue < int >( 6, 0 );
	
	UIImagePickerController *ipc = [[UIImagePickerController alloc]
									init]; 
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];

	ipc.delegate = MOAIAppIOS::Get ().mTakeCameraListener;
	ipc.sourceType = sourceType;
	
	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
		MOAIAppIOS::Get().mImagePickerPopover = [[UIPopoverController alloc] 
												   initWithContentViewController: ipc];
		[MOAIAppIOS::Get ().mTakeCameraListener setPopover:MOAIAppIOS::Get().mImagePickerPopover];
		MOAIAppIOS::Get().mImagePickerPopover.delegate = MOAIAppIOS::Get ().mTakeCameraListener;
		CGRect rect = CGRectMake(x,y,10,10);
		[MOAIAppIOS::Get().mImagePickerPopover presentPopoverFromRect:rect inView:[rootVC view] 
						  permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];

	} else {
		[rootVC presentModalViewController:ipc animated:YES];
	}
	
	return 0;
}

void MOAIAppIOS::callTakeCameraLuaCallback (NSString *imagePath) {
	MOAILuaRef& callback = MOAIAppIOS::Get ().mOnTakeCameraCallback;
	MOAILuaStateHandle state = callback.GetSelf ();
	state.Push ([imagePath UTF8String]);
	state.DebugCall ( 1, 0 );
}

//----------------------------------------------------------------//
/**	@name	getDirectoryInDomain
	@text	Search the platform's internal directory structure for 
			a special directory as defined by the platform.
 
	@in		string domain		The domain to search.
	@out	string directory	The directory associated with the given domain.
*/
int MOAIAppIOS::_getDirectoryInDomain ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 dirCode = state.GetValue<u32>( 1, 0 ); 
	
	if( dirCode == 0 ) {
		
		lua_pushstring ( L, "" );
	}
	else {
	
		NSString *dir = [ NSSearchPathForDirectoriesInDomains ( dirCode, NSUserDomainMask, YES ) lastObject ];

		if ( ![[ NSFileManager defaultManager ] fileExistsAtPath:dir ]) {
			
			NSError *error;
			if ( ![[ NSFileManager defaultManager ] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:&error ]) {
				
				NSLog ( @"Error creating directory %@: %@", dir, error );

				lua_pushstring ( L, "" );

				return 1;
			}
		}
	
		[ dir toLua:L ];
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getUTCTime
 @text	Get the current UTC time in seconds
 
 @in	nil
 @out	num UTC time in seconds
 */
int MOAIAppIOS::_getUTCTime ( lua_State* L ) {
	
	MOAILuaState state ( L );

	lua_pushnumber ( state, [[ NSDate date ] timeIntervalSince1970 ]);

	return 1;
}

//----------------------------------------------------------------//
/**	@name	sendMail
 @text	Send a mail with the passed in default values
 
 @in	string recipient
 @in	string subject
 @in	string message
 @out	nil
 */
int MOAIAppIOS::_sendMail ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* recipient = state.GetValue < cc8* >( 1, "" );
	cc8* subject = state.GetValue < cc8* >( 1, "" );
	cc8* message = state.GetValue < cc8* >( 1, "" );
	
	MFMailComposeViewController* controller = [[ MFMailComposeViewController alloc ] init ];
	controller.mailComposeDelegate = MOAIAppIOS::Get ().mMailDelegate;
	
	NSArray* to = [[ NSArray alloc ] arrayByAddingObject:[[ NSString alloc ] initWithUTF8String:recipient ]];
	
	[ controller setToRecipients:to ];
	[ controller setSubject:[[ NSString alloc ] initWithUTF8String:subject ]];
	[ controller setMessageBody:[[ NSString alloc ] initWithUTF8String:message ] isHTML:NO ]; 
	
	if (controller) {
				
		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		UIViewController* rootVC = [ window rootViewController ];	
		[ rootVC presentModalViewController:controller animated:YES];
	}
	
	[controller release];
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIAppIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAIAppIOS
//================================================================//

//----------------------------------------------------------------//
MOAIAppIOS::MOAIAppIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mReachabilityListener = [ ReachabilityListener alloc ];
	[ this->mReachabilityListener startListener ];	
	
	mMailDelegate = [ MoaiMailComposeDelegate alloc ];
	this->mTakeCameraListener = [MOAITakeCameraListener alloc];
}

//----------------------------------------------------------------//
MOAIAppIOS::~MOAIAppIOS () {

	[ mMailDelegate release ];
	[ this->mTakeCameraListener release];
}

//----------------------------------------------------------------//
void MOAIAppIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "APP_OPENED_FROM_URL",	( u32 )APP_OPENED_FROM_URL );
	state.SetField ( -1, "SESSION_START",		( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			( u32 )SESSION_END );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",	( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",	( u32 )DOMAIN_APP_SUPPORT );
	state.SetField ( -1, "DOMAIN_CACHES",		( u32 )DOMAIN_CACHES );
	
	luaL_Reg regTable [] = {
		{ "getDirectoryInDomain",	_getDirectoryInDomain },
		{ "getUTCTime",				_getUTCTime },
		{ "sendMail",				_sendMail },
		{ "setListener",			_setListener },
		{ "takeCamera",             _takeCamera },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppIOS::AppOpenedFromURL ( NSURL* url ) {

	MOAILuaRef& callback = this->mListeners [ APP_OPENED_FROM_URL ];

	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		[[ url absoluteString ] toLua:state ];

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::DidStartSession ( bool resumed ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushboolean ( state, resumed );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillEndSession ( ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MoaiMailComposeDelegate
//================================================================//
@implementation MoaiMailComposeDelegate

//================================================================//
#pragma mark -
#pragma mark Protocol MoaiMailComposeDelegate
//================================================================//

- (void)mailComposeController:(MFMailComposeViewController*)controller  
          didFinishWithResult:(MFMailComposeResult)result 
                        error:(NSError*)error {
	UNUSED ( controller );
	UNUSED ( result );
	UNUSED ( error );
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	if ( rootVC ) {
		[ rootVC dismissModalViewControllerAnimated:YES ];
	}
}

@end