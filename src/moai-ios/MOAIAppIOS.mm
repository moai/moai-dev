// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/NSData+MOAILib.h>
#import <moai-ios/NSDate+MOAILib.h>
#import <moai-ios/NSDictionary+MOAILib.h>
#import <moai-ios/NSError+MOAILib.h>
#import <moai-ios/NSString+MOAILib.h>

#import <moai-ios/MOAITakeCameraListener.h>

#import <ifaddrs.h>
#import <arpa/inet.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDirectoryInDomain
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
/**	@lua	getInterfaceOrientation
 @text	Get the current orientation of the user interface
 
 @in	nil
 @out	number Interface orientation
 */
int MOAIAppIOS::_getInterfaceOrientation ( lua_State* L ) {
	
	MOAILuaState state ( L );

	UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;

	lua_pushnumber ( state, orientation );

	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_getIPAddress ( lua_State* L ) {

	MOAILuaState state ( L );
	
	struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    NSString *wifiAddress = nil;
    NSString *cellAddress = nil;
	
    // retrieve the current interfaces - returns 0 on success
    if( !getifaddrs ( &interfaces )) {
		
        // Loop through linked list of interfaces
        temp_addr = interfaces;
		
        while ( temp_addr != NULL ) {
			
            sa_family_t sa_type = temp_addr->ifa_addr->sa_family;
			
            if ( sa_type == AF_INET || sa_type == AF_INET6 ) {
				
                NSString *name = [ NSString stringWithUTF8String:temp_addr->ifa_name ];
                NSString *addr = [ NSString stringWithUTF8String:inet_ntoa ((( struct sockaddr_in * )temp_addr->ifa_addr )->sin_addr )]; // pdp_ip0
				
                if([ name isEqualToString:@"en0" ]) {
					
                    // Interface is the wifi connection on the iPhone
                    wifiAddress = addr;
					
                } else if([ name isEqualToString:@"pdp_ip0" ]) {
					
					// Interface is the cell connection on the iPhone
					cellAddress = addr;
				}
            }
            temp_addr = temp_addr->ifa_next;
        }
        // Free memory
        freeifaddrs(interfaces);
    }
    NSString *addr = wifiAddress ? wifiAddress : cellAddress;
	addr = addr ? addr : @"0.0.0.0";
	
	lua_pushstring ( L, [ addr UTF8String ]);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getUTCTime
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
/**	@lua	sendMail
 @text	Send a mail with the passed in default values
 
 @in	string recipient
 @in	string subject
 @in	string message
 @out	nil
 */
int MOAIAppIOS::_sendMail ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
//	cc8* recipient = state.GetValue < cc8* >( 1, "" );
//	cc8* subject = state.GetValue < cc8* >( 2, "" );
//	cc8* message = state.GetValue < cc8* >( 3, "" );
//	
//	MFMailComposeViewController* controller = [[ MFMailComposeViewController alloc ] init ];
//	controller.mailComposeDelegate = MOAIAppIOS::Get ().mMailDelegate;
//	
//	NSArray* to = [ NSArray arrayWithObject:[ NSString  stringWithUTF8String:recipient ]];
//	
//	[ controller setToRecipients:to ];
//	[ controller setSubject:[ NSString stringWithUTF8String:subject ]];
//	[ controller setMessageBody:[ NSString stringWithUTF8String:message ] isHTML:NO ]; 
//	
//	if (controller) {
//				
//		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
//		UIViewController* rootVC = [ window rootViewController ];	
//		[ rootVC presentViewController:controller animated:YES completion:nil];
//	}
//	
//	[controller release];
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIAppIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		
		MOAIAppIOS::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/** @lua _takeCamera
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
		MOAIAppIOS::Get ().mOnTakeCameraCallback.SetRef ( state, 1 );
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
	MOAIScopedLuaState state = callback.GetSelf ();
	state.Push ([imagePath UTF8String]);
	state.DebugCall ( 1, 0 );
}

//================================================================//
// MOAIAppIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIAppIOS::DidBecomeActive () {

	MOAILuaRef& callback = this->mListeners [ DID_BECOME_ACTIVE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
MOAIAppIOS::MOAIAppIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mReachabilityListener = [ MOAIReachabilityListener alloc ];
	[ this->mReachabilityListener startListener ];

	//this->mMailDelegate = [ MoaiMailComposeDelegate alloc ];
	this->mTakeCameraListener = [MOAITakeCameraListener alloc];
}

//----------------------------------------------------------------//
MOAIAppIOS::~MOAIAppIOS () {

	//[ this->mMailDelegate release ];
	[ this->mTakeCameraListener release];
}

//----------------------------------------------------------------//
void MOAIAppIOS::OnGlobalsFinalize () {

	[ this->mReachabilityListener stopListener ];
	[ this->mReachabilityListener release ];
	this->mReachabilityListener = nil;
}

//----------------------------------------------------------------//
void MOAIAppIOS::OpenUrl ( NSURL* url, NSString* sourceApplication ) {

	MOAILuaRef& callback = this->mListeners [ OPEN_URL ];

	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		[[ url absoluteString ] toLua:state ];
		[ sourceApplication toLua:state ];
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DID_BECOME_ACTIVE",	( u32 )DID_BECOME_ACTIVE );
	state.SetField ( -1, "OPEN_URL",			( u32 )OPEN_URL );
	state.SetField ( -1, "WILL_RESIGN_ACTIVE",	( u32 )WILL_RESIGN_ACTIVE );
	state.SetField ( -1, "WILL_TERMINATE",		( u32 )WILL_TERMINATE );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",	( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",	( u32 )DOMAIN_APP_SUPPORT );
	state.SetField ( -1, "DOMAIN_CACHES",		( u32 )DOMAIN_CACHES );
	
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT",				( u32 )INTERFACE_ORIENTATION_PORTRAIT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN",	( u32 )INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_LEFT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_LEFT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_RIGHT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_RIGHT );

	luaL_Reg regTable [] = {
		{ "getDirectoryInDomain",		_getDirectoryInDomain },
		{ "getInterfaceOrientation",	_getInterfaceOrientation },
		{ "getIPAddress",				_getIPAddress },
		{ "getUTCTime",					_getUTCTime },
		{ "sendMail",					_sendMail },
		{ "setListener",				_setListener },
		{ "takeCamera",					_takeCamera },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppIOS::UpdateReachability () {

	[ this->mReachabilityListener updateMoaiEnvironment ];
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillResignActive () {

	MOAILuaRef& callback = this->mListeners [ WILL_RESIGN_ACTIVE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::WillTerminate () {

	MOAILuaRef& callback = this->mListeners [ WILL_TERMINATE ];
	
	if ( callback ) {
		MOAIScopedLuaState state = callback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// MoaiMailComposeDelegate
//================================================================//
//@implementation MoaiMailComposeDelegate
//
////================================================================//
//#pragma mark -
//#pragma mark Protocol MoaiMailComposeDelegate
////================================================================//
//
//- (void)mailComposeController:(MFMailComposeViewController*)controller
//          didFinishWithResult:(MFMailComposeResult)result
//                        error:(NSError*)error {
//	UNUSED ( controller );
//	UNUSED ( result );
//	UNUSED ( error );
//
//	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
//	UIViewController* rootVC = [ window rootViewController ];
//
//	if ( rootVC ) {
//		[ rootVC dismissViewControllerAnimated:YES completion:nil ];
//	}
//}
//
//@end