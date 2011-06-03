// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <StoreKit/StoreKit.h>
#import <moai-iphone/MOAIApp.h>
#import <moai-iphone/MOAIStoreKitListener.h>
#import <moai-iphone/NSData+MOAILib.h>
#import <moai-iphone/NSDate+MOAILib.h>
#import <moai-iphone/NSDictionary+MOAILib.h>
#import <moai-iphone/NSError+MOAILib.h>
#import <moai-iphone/NSString+MOAILib.h>

#define UILOCALNOTIFICATION_USER_INFO_KEY	@"userInfo"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canMakePayments
	@text	Verify that app has permission to request payments.
	
	@out	bool canMakePayments
*/
int MOAIApp::_canMakePayments ( lua_State* L ) {
	USLuaState state ( L );
	
	BOOL result = [ SKPaymentQueue canMakePayments ];
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppIconBadgeNumber
	@text	Return the value of the counter shown on the app icon badge.
	
	@out	number badgeNumber
*/
int MOAIApp::_getAppIconBadgeNumber ( lua_State* L ) {
	USLuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	lua_pushnumber ( state, application.applicationIconBadgeNumber );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	registerForRemoteNotifications
	@text	Return the app for remote notifications.
	
	@opt	number notificationTypes	Combination of MOAIApp.REMOTE_NOTIFICATION_BADGE,
										MOAIApp.REMOTE_NOTIFICATION_SOUND, MOAIApp.REMOTE_NOTIFICATION_ALERT.
										Default value is MOAIApp.REMOTE_NOTIFICATION_NONE.
	@out	nil
*/
int MOAIApp::_registerForRemoteNotifications ( lua_State* L ) {
	USLuaState state ( L );
	
	u32 types = state.GetValue < u32 >( 1, ( u32 )UIRemoteNotificationTypeNone );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application registerForRemoteNotificationTypes:( UIRemoteNotificationType )types ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	requestPaymentForProduct
	@text	Request payment for a product.
	
	@in		string productIdentifier
	@opt	number quantity				Default value is 1.
	@out	nil
*/
int MOAIApp::_requestPaymentForProduct ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	int quantity = state.GetValue < int >( 2, 1 );
	
	if ( quantity ) {
		SKMutablePayment* payment = [ SKMutablePayment paymentWithProductIdentifier:[ NSString stringWithUTF8String:identifier ]];
		payment.quantity = quantity;
		[[ SKPaymentQueue defaultQueue ] addPayment:payment ];
	}
	return 0;
}


//----------------------------------------------------------------//
/**	@name	requestProductIdentifiers
	@text	Varify the validity of a set of products.
	
	@in		table productIdentifiers			A table of product identifiers.
	@out	nil
*/
int MOAIApp::_requestProductIdentifiers ( lua_State* L ) {
	USLuaState state ( L );
	
	NSMutableSet* productSet = [[[ NSMutableSet alloc ] init ] autorelease ];
	
	int top = state.GetTop ();
	for ( int i = 1; i <= top; ++i ) {
	
		if ( state.IsType ( i, LUA_TSTRING )) {
			cc8* identifier = state.GetValue < cc8* >( i, "" );
			[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];
		}
	
		if ( state.IsType ( i, LUA_TTABLE )) {
			
			int itr = state.PushTableItr ( i );
			while ( state.TableItrNext ( itr )) {
				cc8* identifier = state.GetValue < cc8* >( -1, "" );
				[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];

			}
		}
	}
	
	SKProductsRequest* request = [[ SKProductsRequest alloc ] initWithProductIdentifiers:productSet ];
	request.delegate = MOAIApp::Get ().mStoreKitListener;
	[ request start ];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: test me
int MOAIApp::_scheduleLocalNotification ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* userInfo				= state.GetValue < cc8* >( 1, "" );
	cc8* fireDate				= state.GetValue < cc8* >( 2, "" ); // ISO8601
	cc8* timeZone				= state.GetValue < cc8* >( 3, 0 );
	
	cc8* alertAction			= state.GetValue < cc8* >( 5, 0 );
	cc8* alertBody				= state.GetValue < cc8* >( 6, 0 );
	bool hasAction				= state.GetValue < bool >( 4, true );
	cc8* alertLaunchImage		= state.GetValue < cc8* >( 7, 0 );
	
	int appIconBadgeNumber		= state.GetValue < int >( 8, 0 );
	cc8* soundName				= state.GetValue < cc8* >( 9, 0 );
	
	UILocalNotification* notification = [[[ NSNotification alloc ] init ] autorelease ];
	
	notification.fireDate			= [ NSDate dateFromISO8601String:[ NSString stringWithUTF8String:fireDate ]];
	notification.timeZone			= [ NSString stringWithUTF8String:timeZone ];
	
	notification.alertBody			= [ NSString stringWithUTF8String:alertAction ];
	notification.alertAction		= [ NSString stringWithUTF8String:alertBody ];
	notification.hasAction			= hasAction;
	notification.alertLaunchImage	= [ NSString stringWithUTF8String:alertLaunchImage ];
	
	notification.applicationIconBadgeNumber	= appIconBadgeNumber;
	notification.soundName					= [ NSString stringWithUTF8String:soundName ];
	
	NSMutableDictionary* userInfoDictionary = [[[ NSMutableDictionary alloc ] init ] autorelease ];
	[ userInfoDictionary setObject:[ NSString stringWithUTF8String:userInfo ] forKey:UILOCALNOTIFICATION_USER_INFO_KEY ];
	notification.userInfo = userInfoDictionary;
	
	UIApplication* application = [ UIApplication sharedApplication ];
	[ application scheduleLocalNotification:notification ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAppIconBadgeNumber
	@text	Set (or clears) the value of the counter shown on the app icon badge.
	
	@opt	number badgeNumber		Default value is 0.
	@out	nil
*/
int MOAIApp::_setAppIconBadgeNumber ( lua_State* L ) {
	USLuaState state ( L );
	
	UIApplication* application = [ UIApplication sharedApplication ];
	application.applicationIconBadgeNumber = state.GetValue < int >( 1, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setListener
	@text	Set a callback to handle events of a type.
	
	@in		number event		One of MOAIApp.ERROR, MOAIApp.DID_REGISTER, MOAIApp.REMOTE_NOTIFICATION,
								MOAIApp.PAYMENT_QUEUE_TRANSACTION, MOAIApp.PRODUCT_REQUEST_RESPONSE.
	@opt	function handler
	@out	nil
*/
int MOAIApp::_setListener ( lua_State* L ) {
	USLuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetRef ( state, 2, false );
	}
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
void MOAIApp::DidFailToRegisterForRemoteNotificationsWithError ( NSError* error ) {
	
	USLuaRef& callback = this->mListeners [ ERROR ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		[ error toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidReceiveLocalNotification ( UILocalNotification* notification ) {

	USLuaRef& callback = this->mListeners [ LOCAL_NOTIFICATION ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		NSDictionary* userInfo = notification.userInfo;
		if ( userInfo ) {
		
			NSString* userInfoString = [ userInfo objectForKey:UILOCALNOTIFICATION_USER_INFO_KEY ];
			if ( userInfoString ) {
				[ userInfoString toLua:state ];
			}
		}
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidReceiveRemoteNotification ( NSDictionary* userInfo ) {

	USLuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION ];
	
	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		
		[ userInfo toLua:state ];
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::DidRegisterForRemoteNotificationsWithDeviceToken	( NSData* deviceToken ) {
	
	USLuaRef& callback = this->mListeners [ DID_REGISTER ];
	
	if ( callback ) {
		
		STLString tokenStr;
		tokenStr.hex_encode ([ deviceToken bytes ], [ deviceToken length ]);
			
		USLuaStateHandle state = callback.GetSelf ();
		lua_pushstring ( state, tokenStr );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( USLuaObject )
	
	this->mStoreKitListener = [[ MOAIStoreKitListener alloc ] init ];
	[[ SKPaymentQueue defaultQueue ] addTransactionObserver:this->mStoreKitListener ];
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {

	[ this->mStoreKitListener release ];
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {
}

//----------------------------------------------------------------//
void MOAIApp::PaymentQueueUpdatedTransactions ( SKPaymentQueue* queue, NSArray* transactions ) {
	UNUSED ( queue );

	USLuaRef& callback = this->mListeners [ PAYMENT_QUEUE_TRANSACTION ];

	for ( SKPaymentTransaction* transaction in transactions ) {
	
		if ( callback ) {
		
			USLuaStateHandle state = callback.GetSelf ();
			this->PushPaymentTransaction ( state, transaction );
			state.DebugCall ( 1, 0 );
		}
		
		if ( transaction.transactionState != SKPaymentTransactionStatePurchasing ) {
			[[ SKPaymentQueue defaultQueue ] finishTransaction:transaction ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIApp::ProductsRequestDidReceiveResponse ( SKProductsRequest* request, SKProductsResponse* response ) {

	USLuaRef& callback = this->mListeners [ PRODUCT_REQUEST_RESPONSE ];
	if ( callback ) {
		
		USLuaStateHandle state = callback.GetSelf ();
		lua_newtable ( state );
		
		int count = 1;
		for ( SKProduct* product in response.products ) {
		
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
			
			state.SetField ( -1, "localizedTitle", [ product.localizedTitle UTF8String ]);
			state.SetField ( -1, "localizedDescription", [ product.localizedDescription UTF8String ]);
			state.SetField ( -1, "price", [ product.price floatValue ]);
			state.SetField ( -1, "priceLocale", [ product.priceLocale.localeIdentifier UTF8String ]);
			state.SetField ( -1, "productIdentifier", [ product.productIdentifier UTF8String ]);
			
			lua_settable ( state, -3 );
		}
		
		state.DebugCall ( 1, 0 );
	}
	[ request autorelease ];
}

//----------------------------------------------------------------//
void MOAIApp::PushPaymentTransaction ( lua_State* L, SKPaymentTransaction* transaction ) {

	lua_newtable ( L );
	
	lua_pushstring ( L, "transactionState" );
	
	switch ( transaction.transactionState ) {
		
		case SKPaymentTransactionStatePurchasing: {
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASING );
			break;
		}
		case SKPaymentTransactionStatePurchased: {
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASED );
			break;
		}
		case SKPaymentTransactionStateFailed: {
			lua_pushnumber ( L, TRANSACTION_STATE_FAILED );
			break;
		}
		case SKPaymentTransactionStateRestored: {
			lua_pushnumber ( L, TRANSACTION_STATE_RESTORED );
			break;
		}
		default: {
			lua_pushnil ( L );
			break;
		}
	}
	
	lua_settable ( L, -3 );
	
	if ( transaction.payment ) {
		
		lua_pushstring ( L, "payment" );
		lua_newtable ( L );
		
		lua_pushstring ( L, "productIdentifier" );
		[ transaction.payment.productIdentifier toLua:L ];
		lua_settable ( L, -3 );
		
		lua_pushstring ( L, "quantity" );
		lua_pushnumber ( L, transaction.payment.quantity );
		lua_settable ( L, -3 );
		
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateFailed ) {
		lua_pushstring ( L, "error" );
		[ transaction.error toLua:L ];
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateRestored ) {
		lua_pushstring ( L, "originalTransaction" );
		this->PushPaymentTransaction ( L, transaction.originalTransaction );
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStatePurchased ) {
		lua_pushstring ( L, "transactionReceipt" );
		[ transaction.transactionReceipt toLua:L ];
		lua_settable ( L, -3 );
	}
	
	if (( transaction.transactionState == SKPaymentTransactionStatePurchased ) || ( transaction.transactionState == SKPaymentTransactionStateRestored )) {
		
		lua_pushstring ( L, "transactionDate" );
		[ transaction.transactionDate toLua:L ];
		lua_settable ( L, -3 );

		lua_pushstring ( L, "transactionIdentifier" );
		[ transaction.transactionIdentifier toLua:L ];
		lua_settable ( L, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_NONE",	( u32 )UIRemoteNotificationTypeNone );
	state.SetField ( -1, "REMOTE_NOTIFICATION_BADGE",	( u32 )UIRemoteNotificationTypeBadge );
	state.SetField ( -1, "REMOTE_NOTIFICATION_SOUND",	( u32 )UIRemoteNotificationTypeSound );
	state.SetField ( -1, "REMOTE_NOTIFICATION_ALERT",	( u32 )UIRemoteNotificationTypeAlert );
	
	state.SetField ( -1, "ERROR",						( u32 )ERROR );
	state.SetField ( -1, "DID_REGISTER",				( u32 )DID_REGISTER );
	//state.SetField ( -1, "LOCAL_NOTIFICATION",		( u32 )LOCAL_NOTIFICATION );
	state.SetField ( -1, "PAYMENT_QUEUE_TRANSACTION",	( u32 )PAYMENT_QUEUE_TRANSACTION );
	state.SetField ( -1, "PRODUCT_REQUEST_RESPONSE",	( u32 )PRODUCT_REQUEST_RESPONSE );
	state.SetField ( -1, "REMOTE_NOTIFICATION",			( u32 )REMOTE_NOTIFICATION );

	luaL_Reg regTable[] = {
		{ "canMakePayments",					_canMakePayments },
		{ "getAppIconBadgeNumber",				_getAppIconBadgeNumber },
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "requestPaymentForProduct",			_requestPaymentForProduct },
		{ "requestProductIdentifiers",			_requestProductIdentifiers },
		//{ "scheduleLocalNotification",			_scheduleLocalNotification },
		{ "setAppIconBadgeNumber",				_setAppIconBadgeNumber },
		{ "setListener",						_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

