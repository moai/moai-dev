// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPIOS_H
#define	MOAIAPPIOS_H

#import <Foundation/Foundation.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <UIKit/UIKit.h>

#import <moai-core/host.h>
#import <moai-core/headers.h>

// TODO: rename to MOAIMailComposeDelegate
@class MoaiMailComposeDelegate;
@class MOAITakeCameraListener;

//================================================================//
// MOAIAppIOS
//================================================================//
/**	@lua	MOAIAppIOS
	@text	Wrapper for base application class on iOS devices.
			Exposed to Lua via MOAIApp on all mobile platforms.

	@const	DID_BECOME_ACTIVE
	@const	OPEN_URL
	@const	WILL_RESIGN_ACTIVE
	@const	WILL_TERMINATE

	@const	DOMAIN_DOCUMENTS		Directory domain 'documents'.
	@const	DOMAIN_APP_SUPPORT		Directory domain 'application support'.
	@const	DOMAIN_CACHES			Directory domain 'caches'.

	@const	INTERFACE_ORIENTATION_PORTRAIT				Interface orientation UIInterfaceOrientationPortrait.
	@const	INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN	Interface orientation UIInterfaceOrientationPortraitUpsideDown.
	@const	INTERFACE_ORIENTATION_LANDSCAPE_LEFT		Interface orientation UIInterfaceOrientationLandscapeLeft.
	@const	INTERFACE_ORIENTATION_LANDSCAPE_RIGHT		Interface orientation UIInterfaceOrientationLandscapeRight.
*/
class MOAIAppIOS :
	public MOAIGlobalClass < MOAIAppIOS, MOAIGlobalEventSource > {
private:
	
	typedef STLMap < STLString, u32 >::iterator NotificationListenerMapIt;
	STLMap < STLString, u32 >	mNotificationListenerMap;
	
	typedef STLList < id >::iterator NotificationObserverIt;
	STLList < id >	mNotificationObservers;

	MOAILuaStrongRef			mOnTakeCameraCallback;
	MOAITakeCameraListener*		mTakeCameraListener;
	//MoaiMailComposeDelegate*	mMailDelegate;
	UIPopoverController*		mImagePickerPopover;
	
	//----------------------------------------------------------------//
	static int		_canOpenURL						( lua_State* L );
	static int		_getAvailableStorage			( lua_State* L );
	static int		_getDirectoryInDomain			( lua_State* L );
	static int		_getInterfaceOrientation		( lua_State* L );
	static int		_getIPAddress					( lua_State* L );
	static int		_getUTCTime						( lua_State* L );
	static int		_openURL						( lua_State* L );
	static int		_openURLWithParams				( lua_State* L );
	static int		_sendMail						( lua_State* L );
	static int		_takeCamera						( lua_State* L );
	
	//----------------------------------------------------------------//
	void			RegisterNotificationListeners	();
	void			RemoveNotificationListeners		();
	
public:
	
	DECL_LUA_SINGLETON ( MOAIAppIOS )
	
	enum {
		DID_BECOME_ACTIVE,
		DID_ENTER_BACKGROUND,
		DID_RECEIVE_MEMORY_WARNING,
		OPEN_URL,
		WILL_ENTER_FOREGROUND,
		WILL_RESIGN_ACTIVE,
		WILL_TERMINATE,
		TOTAL,
	};
	
	enum {
		DOMAIN_DOCUMENTS   = NSDocumentDirectory,
		DOMAIN_APP_SUPPORT = NSApplicationSupportDirectory,
		DOMAIN_CACHES      = NSCachesDirectory,
	};

	enum {
		INTERFACE_ORIENTATION_PORTRAIT					= UIInterfaceOrientationPortrait,
		INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN		= UIInterfaceOrientationPortraitUpsideDown,
		INTERFACE_ORIENTATION_LANDSCAPE_LEFT			= UIInterfaceOrientationLandscapeLeft,
		INTERFACE_ORIENTATION_LANDSCAPE_RIGHT			= UIInterfaceOrientationLandscapeRight,
	};

	//----------------------------------------------------------------//
	static CGRect		GetScreenBoundsFromCurrentOrientation	( const CGRect& bounds ); // TODO: move to MOAIWebView or MOAIAppDelegate when those are added later
	static BOOL			IsSystemVersionLessThan					( NSString* version ); // TODO: move to MOAIWebView or MOAIAppDelegate when those are added later
						MOAIAppIOS								();
						~MOAIAppIOS								();
	void				OnGlobalsFinalize						();
	void				OpenUrl									( NSURL* url, NSString* sourceApplication );
	void				RegisterLuaClass						( MOAILuaState& state );

	static void			callTakeCameraLuaCallback				( NSString* imagePath );
};

//================================================================//
// MoaiMailComposeDelegate
//================================================================//
//@interface MoaiMailComposeDelegate : NSObject < MFMailComposeViewControllerDelegate > {
//@private
//}
//@end

#endif