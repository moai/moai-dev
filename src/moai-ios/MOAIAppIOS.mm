// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moai-apple/NSData+MOAILib.h>
#import <moai-apple/NSDate+MOAILib.h>
#import <moai-apple/NSDictionary+MOAILib.h>
#import <moai-apple/NSError+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>

#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAITakeCameraListener.h>

#import <ifaddrs.h>
#import <arpa/inet.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	canOpenURL
 @text	Return true if iOS will handle the passed URL.
 
 @in	string url
 @out	boolean
 */
int MOAIAppIOS::_canOpenURL ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* url = state.GetValue < cc8* >( 1, "" );

	if ( url && url [ 0 ] != '\0' ) {
		NSString *requestString = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSURL * myURL = [[NSURL alloc] initWithString:[requestString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		lua_pushboolean (state, [[ UIApplication sharedApplication ]
								 canOpenURL:myURL]);
		[myURL release];
		return 1;
	}

	lua_pushnil( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAvailableStorage
	@text	Get the available storage size in mb on the system
 
	@out	long size
 */
int MOAIAppIOS::_getAvailableStorage ( lua_State* L ) {
	MOAILuaState state ( L );
	
	NSDictionary *atDict = [[ NSFileManager defaultManager ] attributesOfFileSystemForPath:NSHomeDirectory () error:NULL ];
	unsigned long long freeSize = [[ atDict objectForKey:NSFileSystemFreeSize ] unsignedLongLongValue ] / 1024;
	lua_pushnumber ( state, freeSize);

	return 1;
}

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

	UIInterfaceOrientation orientation = [ UIApplication sharedApplication ].statusBarOrientation;

	lua_pushnumber ( state, orientation );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
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
/**	@lua	openURL
	@text	Open the native device web browser at the specified URL.
 
	@in		string url
	@out	nil
*/
int MOAIAppIOS::_openURL ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* url = state.GetValue < cc8* >( 1, "" );
	
	if ( url && url [ 0 ] != '\0' ) {
		NSString *requestString = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
		NSURL * myURL = [[NSURL alloc] initWithString:[requestString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		[[ UIApplication sharedApplication ] openURL:myURL];
		
		[myURL release];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openURLWithParams
	@text	Open the native device web browser at the specified URL
			with the specified list of query string parameters.
 
	@in		string url
	@in		table params
	@out	nil
*/
int MOAIAppIOS::_openURLWithParams ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* baseURL = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	NSMutableDictionary* params = [[ NSMutableDictionary alloc ] initWithCapacity:5 ];
	[ params initWithLua: state stackIndex: 2 ];
	
	if ( baseURL == NULL || params == NULL ) return 0;
	
	NSURL* parsedURL = [ NSURL URLWithString: baseURL ];
	NSString* urlQueryPrefix = parsedURL.query ? @"&" : @"?";
	
	NSMutableArray* paramPairs = [ NSMutableArray array ];
	for ( NSString* key in [ params keyEnumerator ] ) {
		
		NSString* escapedValue = ( NSString* )CFURLCreateStringByAddingPercentEscapes( NULL, ( CFStringRef )[ params objectForKey: key ], NULL, ( CFStringRef )@"!*'();:@&=+$,/?%#[]", kCFStringEncodingUTF8 );
		[ paramPairs addObject:[ NSString stringWithFormat: @"%@=%@", key, escapedValue ]];
		[ escapedValue release ];
	}
	
	NSString* urlQuery = [ paramPairs componentsJoinedByString: @"&" ];
		
	[[ UIApplication sharedApplication ] openURL:[ NSURL URLWithString:[ NSString stringWithFormat: @"%@%@%@", baseURL, urlQueryPrefix, urlQuery ]]];	

	return 0;
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
// Since iOS 8.0 both [[UIScreen mainScreen] bounds/applicationFrame] are interface oriented.
// Before iOS 8.0, only the portrait "bounds" where returned.
// The function below ensure Portrait bounds are returned.
CGRect MOAIAppIOS::GetScreenBoundsFromCurrentOrientation ( const CGRect& bounds ) {

	bool lessThaniOS8 = MOAIAppIOS::IsSystemVersionLessThan ( @"8.0" );
	if ( lessThaniOS8 || UIInterfaceOrientationIsPortrait ([ UIApplication sharedApplication ].statusBarOrientation )) {
		return bounds;
	}
	return CGRectMake ( bounds.origin.y, bounds.origin.x, bounds.size.height, bounds.size.width );
}

//----------------------------------------------------------------//
BOOL MOAIAppIOS::IsSystemVersionLessThan ( NSString* version ) {

	return ([[[ UIDevice currentDevice ] systemVersion ] compare:version options:NSNumericSearch ] == NSOrderedAscending );
}

//----------------------------------------------------------------//
MOAIAppIOS::MOAIAppIOS () {

	RTTI_SINGLE ( MOAIGlobalEventSource )

	//this->mMailDelegate = [ MoaiMailComposeDelegate alloc ];
	this->mTakeCameraListener = [ MOAITakeCameraListener alloc ];
	
	this->RegisterNotificationListeners ();
}

//----------------------------------------------------------------//
MOAIAppIOS::~MOAIAppIOS () {

	RemoveNotificationListeners ();

	//[ this->mMailDelegate release ];
	[ this->mTakeCameraListener release];
}

//----------------------------------------------------------------//
void MOAIAppIOS::OnGlobalsFinalize () {
}

//----------------------------------------------------------------//
void MOAIAppIOS::OpenUrl ( NSURL* url, NSString* sourceApplication ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( OPEN_URL, state )) {
		[[ url absoluteString ] toLua:state ];
		[ sourceApplication toLua:state ];
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DID_BECOME_ACTIVE",			( u32 )DID_BECOME_ACTIVE );
	state.SetField ( -1, "DID_ENTER_BACKGROUND",		( u32 )DID_ENTER_BACKGROUND );
	state.SetField ( -1, "DID_RECEIVE_MEMORY_WARNING",	( u32 )DID_RECEIVE_MEMORY_WARNING );
	state.SetField ( -1, "OPEN_URL",					( u32 )OPEN_URL );
	state.SetField ( -1, "WILL_ENTER_FOREGROUND",		( u32 )WILL_ENTER_FOREGROUND );
	state.SetField ( -1, "WILL_RESIGN_ACTIVE",			( u32 )WILL_RESIGN_ACTIVE );
	state.SetField ( -1, "WILL_TERMINATE",				( u32 )WILL_TERMINATE );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",			( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",			( u32 )DOMAIN_APP_SUPPORT );
	state.SetField ( -1, "DOMAIN_CACHES",				( u32 )DOMAIN_CACHES );
	
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT",				( u32 )INTERFACE_ORIENTATION_PORTRAIT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN",	( u32 )INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_LEFT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_LEFT );
	state.SetField ( -1, "INTERFACE_ORIENTATION_LANDSCAPE_RIGHT",		( u32 )INTERFACE_ORIENTATION_LANDSCAPE_RIGHT );

	luaL_Reg regTable [] = {
		{ "canOpenURL",					_canOpenURL },
		{ "getAvailableStorage",		_getAvailableStorage },
		{ "getDirectoryInDomain",		_getDirectoryInDomain },
		{ "getInterfaceOrientation",	_getInterfaceOrientation },
		{ "getIPAddress",				_getIPAddress },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIAppIOS > },
		{ "getUTCTime",					_getUTCTime },
		{ "openURL",					_openURL },
		{ "openURLWithParams",			_openURLWithParams },
		{ "sendMail",					_sendMail },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIAppIOS > },
		{ "takeCamera",					_takeCamera },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAppIOS::RegisterNotificationListeners () {

	this->mNotificationListenerMap [ "UIApplicationDidBecomeActiveNotification" ] = DID_BECOME_ACTIVE;
	this->mNotificationListenerMap [ "UIApplicationDidEnterBackgroundNotification" ] = DID_ENTER_BACKGROUND;
	this->mNotificationListenerMap [ "UIApplicationDidReceiveMemoryWarningNotification" ] = DID_RECEIVE_MEMORY_WARNING;
	this->mNotificationListenerMap [ "UIApplicationWillEnterForegroundNotification" ] = WILL_ENTER_FOREGROUND;
	this->mNotificationListenerMap [ "UIApplicationWillResignActiveNotification" ] = WILL_RESIGN_ACTIVE;
	this->mNotificationListenerMap [ "UIApplicationWillTerminateNotification" ] = WILL_TERMINATE;

	NotificationListenerMapIt notificationIt = this->mNotificationListenerMap.begin ();
	for ( ; notificationIt != this->mNotificationListenerMap.end (); ++notificationIt ) {
		NSString* observerName = [ NSString stringWithUTF8String:notificationIt->first ];
		u32 eventID = notificationIt->second;
	
		id observer = [[ NSNotificationCenter defaultCenter ]
			addObserverForName:observerName
			object:[ UIApplication sharedApplication ]
			queue:nil
			usingBlock:^( NSNotification* notification ) {
				NSLog ( @"%@", notification.name );
				this->InvokeListener ( eventID );
				
				if ( eventID == WILL_TERMINATE ) {
					AKUAppFinalize ();
				}
			}
		];
		
		if ( observer ) {
			this->mNotificationObservers.push_back ( observer );
		}
	}
}

//----------------------------------------------------------------//
void MOAIAppIOS::RemoveNotificationListeners () {

	NotificationObserverIt observerIt = this->mNotificationObservers.begin ();
	for ( ; observerIt != this->mNotificationObservers.end (); ++observerIt ) {
		[[ NSNotificationCenter defaultCenter ] removeObserver:*observerIt ];
	}
	this->mNotificationObservers.clear ();
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