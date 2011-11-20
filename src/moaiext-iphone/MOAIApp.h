// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>
#import <moaiext-iphone/ReachabilityListener.h>

@class MOAIStoreKitListener;

//================================================================//
// LuaAlertView
//================================================================//
// TODO: harebrained
@interface LuaAlertView : UIAlertView < UIAlertViewDelegate > {
@public
	MOAILuaRef callback;
};

	//----------------------------------------------------------------//
	- (id)initWithTitle:(NSString *)title message:(NSString *)message cancelButtonTitle:(NSString *)cancelButtonTitle;
	//- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;

@end

//================================================================//
// MOAIApp
//================================================================//
/**	@name	MOAIApp
	@text	Wrapper for iPhone app delegate.

	@const	ERROR						Event code for errors.
	@const	DID_REGISTER				Event code for registration.
	@const	REMOTE_NOTIFICATION			Event code for notification.
	
	@const	REMOTE_NOTIFICATION_NONE	Alias for 0.
	@const	REMOTE_NOTIFICATION_BADGE	Bitmask for badge notification.
	@const	REMOTE_NOTIFICATION_SOUND	Bitmask for sound notification.
	@const	REMOTE_NOTIFICATION_ALERT	Bitmask for alert notification.
*/
class MOAIApp :
	public MOAIGlobalClass < MOAIApp, MOAILuaObject > {
private:

	enum {
		ERROR,
		DID_REGISTER,
		LOCAL_NOTIFICATION,
		PAYMENT_QUEUE_TRANSACTION,
		PAYMENT_QUEUE_ERROR,
		PRODUCT_REQUEST_RESPONSE,
		REMOTE_NOTIFICATION,
		ASYNC_NAME_RESOLVE,
		TOTAL,
	};
	
	enum {
		TRANSACTION_STATE_PURCHASING,
		TRANSACTION_STATE_PURCHASED,
		TRANSACTION_STATE_FAILED,
		TRANSACTION_STATE_RESTORED,
		TRANSACTION_STATE_CANCELLED,
	};
		
	enum {
		DOMAIN_DOCUMENTS   = NSDocumentDirectory,
		DOMAIN_APP_SUPPORT = NSApplicationSupportDirectory,
	};
	
	enum {
		APP_OPENED_FROM_URL,
		SESSION_START,
		SESSION_END
	};
	
	UIApplication*			mApplication;
	MOAILuaRef				mListeners [ TOTAL ];
	NSDictionary*			mAppNotificationPayload;
	MOAIStoreKitListener*	mStoreKitListener;

	int						mNumRequests;

	ReachabilityListener*	mReachabilityListener;

	//----------------------------------------------------------------//
	static int		_alert									( lua_State* L );
	static int		_composeTweet							( lua_State* L );
	static int		_canMakePayments						( lua_State* L );
	static int		_canTweet								( lua_State* L );
	static int		_getAppIconBadgeNumber					( lua_State* L );
	static int		_getDirectoryInDomain					( lua_State* L );
	static int		_getNotificationThatStartedApp			( lua_State* L );
	static int		_openURL								( lua_State* L );
	static int		_openURLWithParams						( lua_State* L );
	static int		_presentLocalNotification				( lua_State* L );
	static int		_registerForRemoteNotifications			( lua_State* L );
	static int		_restoreCompletedTransactions			( lua_State* L );
	static int		_requestPaymentForProduct				( lua_State* L );
	static int		_requestProductIdentifiers				( lua_State* L );
	static int		_scheduleLocalNotification				( lua_State* L );
	static int		_setAppIconBadgeNumber					( lua_State* L );
	static int		_setListener							( lua_State* L );

	//----------------------------------------------------------------//
	void			PushPaymentTransaction					( lua_State* L, SKPaymentTransaction* transaction );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	SET ( UIApplication*, Application, mApplication )
	
	//----------------------------------------------------------------//
	void		AppOpenedFromURL											( NSURL* url );
	void		DidFailToRegisterForRemoteNotificationsWithError			( NSError* error );
	void		DidReceiveLocalNotification									( UILocalNotification* notification );
	void		DidReceivePaymentQueueError									( NSError *error, cc8 *extraInfo );
	void		DidReceiveRemoteNotification								( NSDictionary* userInfo );
	void		DidRegisterForRemoteNotificationsWithDeviceToken			( NSData* deviceToken );
	void		DidResolveHostName											( NSString* hostname, cc8* ipAddress );
	void		DidStartSession												();
	void		InitStoreKit												();
				MOAIApp														();
				~MOAIApp													();
	void		OnInit														();
	void		PaymentQueueUpdatedTransactions								( SKPaymentQueue* queue, NSArray* transactions );
	void		ProductsRequestDidReceiveResponse							( SKProductsRequest* request, SKProductsResponse* response );
	void		RegisterLuaClass											( MOAILuaState& state );
	void		Reset														();
	void		SetRemoteNotificationPayload								( NSDictionary* remoteNotificationPayload );
	void		WillEndSession												();
};

#endif
