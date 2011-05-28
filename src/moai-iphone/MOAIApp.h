// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

@class MOAIStoreKitListener;

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
	public USGlobalClass < MOAIApp, USLuaObject > {
private:

	enum {
		ERROR,
		DID_REGISTER,
		LOCAL_NOTIFICATION,
		PAYMENT_QUEUE_TRANSACTION,
		PRODUCT_REQUEST_RESPONSE,
		REMOTE_NOTIFICATION,
		TOTAL,
	};
	
	enum {
		TRANSACTION_STATE_PURCHASING,
		TRANSACTION_STATE_PURCHASED,
		TRANSACTION_STATE_FAILED,
		TRANSACTION_STATE_RESTORED,
	};
	
	UIApplication*			mApplication;
	USLuaRef				mListeners [ TOTAL ];
	MOAIStoreKitListener*	mStoreKitListener;

	//----------------------------------------------------------------//
	static int		_canMakePayments						( lua_State* L );
	static int		_getAppIconBadgeNumber					( lua_State* L );
	static int		_registerForRemoteNotifications			( lua_State* L );
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
	void		DidFailToRegisterForRemoteNotificationsWithError			( NSError* error );
	void		DidReceiveLocalNotification									( UILocalNotification* notification );
	void		DidReceiveRemoteNotification								( NSDictionary* userInfo );
	void		DidRegisterForRemoteNotificationsWithDeviceToken			( NSData* deviceToken );
	void		InitStoreKit												();
				MOAIApp														();
				~MOAIApp													();
	void		OnInit														();
	void		PaymentQueueUpdatedTransactions								( SKPaymentQueue* queue, NSArray* transactions );
	void		ProductsRequestDidReceiveResponse							( SKProductsRequest* request, SKProductsResponse* response );
	void		RegisterLuaClass											( USLuaState& state );
};

#endif
