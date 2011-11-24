// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaiext-android/MOAIApp.h>
#include <android/log.h>

//================================================================//
// Utility macros
//================================================================//

	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIApp::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

int MOAIApp::_checkBillingSupported ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool result = MOAIApp::Get ().checkBillingSupportedFunc ();
	lua_pushboolean ( state, result );
	
	return 1;
}

int MOAIApp::_requestPurchase ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	bool result = MOAIApp::Get ().requestPurchaseFunc ( identifier );
	lua_pushboolean ( state, result );
	
	return 1;
}

int MOAIApp::_confirmNotification ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* notification = state.GetValue < cc8* >( 1, "" );
	bool result = MOAIApp::Get ().confirmNotificationFunc ( notification );
	lua_pushboolean ( state, result );
	
	return 1;
}

int MOAIApp::_restoreTransactions ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool result = MOAIApp::Get ().restoreTransactionsFunc ();
	lua_pushboolean ( state, result );
	
	return 1;
}

int MOAIApp::_setMarketPublicKey ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* key = state.GetValue < cc8* >( 1, "" );
	MOAIApp::Get ().setMarketPublicKeyFunc ( key );
	
	return 0;
}

//================================================================//
// MOAIApp
//================================================================//

//----------------------------------------------------------------//
void MOAIApp::DidStartSession( ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_START ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SESSION_START",		    			( u32 )SESSION_START );
	state.SetField ( -1, "SESSION_END",			    			( u32 )SESSION_END );
	state.SetField ( -1, "CHECK_BILLING_SUPPORTED",				( u32 )CHECK_BILLING_SUPPORTED );
	state.SetField ( -1, "PURCHASE_RESPONSE_RECEIVED",			( u32 )PURCHASE_RESPONSE_RECEIVED );
	state.SetField ( -1, "PURCHASE_STATE_CHANGED",				( u32 )PURCHASE_STATE_CHANGED );
	state.SetField ( -1, "RESTORE_RESPONSE_RECEIVED",			( u32 )RESTORE_RESPONSE_RECEIVED );

	state.SetField ( -1, "BILLING_RESULT_OK",					( u32 )BILLING_RESULT_OK );
	state.SetField ( -1, "BILLING_RESULT_USER_CANCELED",		( u32 )BILLING_RESULT_USER_CANCELED );
	state.SetField ( -1, "BILLING_RESULT_SERVICE_UNAVAILABLE",	( u32 )BILLING_RESULT_SERVICE_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_BILLING_UNAVAILABLE",	( u32 )BILLING_RESULT_BILLING_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ITEM_UNAVAILABLE",		( u32 )BILLING_RESULT_ITEM_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_DEVELOPER_ERROR",		( u32 )BILLING_RESULT_DEVELOPER_ERROR );
	state.SetField ( -1, "BILLING_RESULT_ERROR",				( u32 )BILLING_RESULT_ERROR );

	state.SetField ( -1, "BILLING_STATE_ITEM_PURCHASED",		( u32 )BILLING_STATE_ITEM_PURCHASED );
	state.SetField ( -1, "BILLING_STATE_PURCHASE_CANCELED",		( u32 )BILLING_STATE_PURCHASE_CANCELED );
	state.SetField ( -1, "BILLING_STATE_ITEM_REFUNDED",			( u32 )BILLING_STATE_ITEM_REFUNDED );

	luaL_Reg regTable[] = {
		{ "setListener",						_setListener },
		{ "checkBillingSupported",				_checkBillingSupported },
		{ "requestPurchase",					_requestPurchase },
		{ "confirmNotification",				_confirmNotification },
		{ "restoreTransactions",				_restoreTransactions },
		{ "setMarketPublicKey",					_setMarketPublicKey },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIApp::Reset () {
	for ( int i = 0 ; i < TOTAL; i++ ) {
		mListeners [ i ].Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIApp::WillEndSession( ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_END ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::SetCheckBillingSupportedFunc ( bool ( *billingSupportedFunc ) ( void )) {
	checkBillingSupportedFunc = billingSupportedFunc;
}

//----------------------------------------------------------------//
void MOAIApp::SetRequestPurchaseFunc ( bool ( *purchaseFunc ) ( cc8* )) {
	requestPurchaseFunc = purchaseFunc;
}

//----------------------------------------------------------------//
void MOAIApp::SetConfirmNotificationFunc ( bool ( *confirmFunc ) ( cc8* )) {
	confirmNotificationFunc = confirmFunc;
}

//----------------------------------------------------------------//
void MOAIApp::SetRestoreTransactionsFunc ( bool ( *restoreFunc ) ( void )) {
	restoreTransactionsFunc = restoreFunc;
}

//----------------------------------------------------------------//
void MOAIApp::SetMarketPublicKeyFunc ( void ( *setKeyFunc ) ( cc8* )) {
	setMarketPublicKeyFunc = setKeyFunc;
}

//----------------------------------------------------------------//
void MOAIApp::NotifyBillingSupported (bool supported) {	
	MOAILuaRef& callback = this->mListeners [ CHECK_BILLING_SUPPORTED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushboolean ( state, supported );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyPurchaseResponseReceived ( cc8* identifier, int code ) {
	MOAILuaRef& callback = this->mListeners [ PURCHASE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, identifier );
		lua_pushinteger ( state, code );	
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyPurchaseStateChanged ( cc8* identifier, int code, cc8* order, cc8* notification, cc8* payload ) {
	MOAILuaRef& callback = this->mListeners [ PURCHASE_STATE_CHANGED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, identifier );
		lua_pushinteger ( state, code );	
		lua_pushstring ( state, order );
		lua_pushstring ( state, notification );
		lua_pushstring ( state, payload );
		
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::NotifyRestoreResponseReceived ( int code ) {
	MOAILuaRef& callback = this->mListeners [ RESTORE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushinteger ( state, code );	
		
		state.DebugCall ( 1, 0 );
	}
}
