// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPIOS_H
#define	MOAIAPPIOS_H

#import <Foundation/Foundation.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <UIKit/UIKit.h>

#import <moai-core/headers.h>
#import <moai-iphone/ReachabilityListener.h>

@class MoaiMailComposeDelegate;
@class MOAITakeCameraListener;

//================================================================//
// MOAIAppIOS
//================================================================//
/**	@name	MOAIAppIOS
	@text	Wrapper for base application class on iOS devices.
			Exposed to lua via MOAIApp on all mobile platforms.

	@const	APP_OPENED_FROM_URL		Event code indicating that the app was stared via a URL click.
	@const	SESSION_START			Event code indicating the beginning of an app session.
	@const	SESSION_END				Event code indicating the end of an app sessions.

	@const	DOMAIN_DOCUMENTS		Directory domain 'documents'.
	@const	DOMAIN_APP_SUPPORT		Directory domain 'application support'.
	@const	DOMAIN_CACHES			Directory domain 'caches'.

	@const	INTERFACE_ORIENTATION_PORTRAIT				Interface orientation UIInterfaceOrientationPortrait.
	@const	INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN	Interface orientation UIInterfaceOrientationPortraitUpsideDown.
	@const	INTERFACE_ORIENTATION_LANDSCAPE_LEFT		Interface orientation UIInterfaceOrientationLandscapeLeft.
	@const	INTERFACE_ORIENTATION_LANDSCAPE_RIGHT		Interface orientation UIInterfaceOrientationLandscapeRight.
*/
class MOAIAppIOS :
	public MOAIGlobalClass < MOAIAppIOS, MOAILuaObject > {
private:

	MoaiMailComposeDelegate* mMailDelegate;
		
	//----------------------------------------------------------------//
	static int	_getDirectoryInDomain		( lua_State* L );
	static int	_getInterfaceOrientation	( lua_State* L );
	static int	_getIPAddress				( lua_State* L );
	static int	_getUTCTime					( lua_State* L );
	static int	_sendMail					( lua_State* L );
	static int	_setListener				( lua_State* L );
	static int	_takeCamera					( lua_State* L );
		
public:
	
	DECL_LUA_SINGLETON ( MOAIAppIOS )
	
	SET ( UIApplication*, Application, mApplication )
	
	enum {
		APP_OPENED_FROM_URL,
		SESSION_START,
		SESSION_END,
		TOTAL,
	};
	
	enum {
		DOMAIN_DOCUMENTS   = NSDocumentDirectory,
		DOMAIN_APP_SUPPORT = NSApplicationSupportDirectory,
		DOMAIN_CACHES      = NSCachesDirectory,
	};

	enum {
		INTERFACE_ORIENTATION_PORTRAIT             = UIInterfaceOrientationPortrait,
		INTERFACE_ORIENTATION_PORTRAIT_UPSIDE_DOWN = UIInterfaceOrientationPortraitUpsideDown,
		INTERFACE_ORIENTATION_LANDSCAPE_LEFT       = UIInterfaceOrientationLandscapeLeft,
		INTERFACE_ORIENTATION_LANDSCAPE_RIGHT      = UIInterfaceOrientationLandscapeRight,
	};

	UIApplication*				mApplication;
	MOAILuaStrongRef			mListeners [ TOTAL ];
	ReachabilityListener*		mReachabilityListener;
	MOAILuaStrongRef			mOnTakeCameraCallback;
	MOAITakeCameraListener*		mTakeCameraListener;
	UIPopoverController*		mImagePickerPopover;

			MOAIAppIOS			();
			~MOAIAppIOS			();
	
	void	AppOpenedFromURL	( NSURL* url );
	void	DidStartSession		( bool resumed );
	void	RegisterLuaClass	( MOAILuaState& state );
	void	WillEndSession		();
	static void		callTakeCameraLuaCallback									(NSString* imagePath);
};

//================================================================//
// MoaiMailComposeDelegate
//================================================================//
@interface MoaiMailComposeDelegate : NSObject < MFMailComposeViewControllerDelegate > {
@private
}
@end

#endif