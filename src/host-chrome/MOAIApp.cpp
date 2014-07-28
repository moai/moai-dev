#include "moai-core/pch.h"
#include "geturl_handler.h"
#include "MOAIApp.h"
#include "moai_nacl.h"

std::string g_openID;
std::string g_productID;

#define PRODUCTION_URLS 1

void MOAIApp::HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size ) {

	NACL_LOG ( "MOAIApp_HttpLoaded code %d, data %s\n", handler->GetStatusCode (), buffer );

	if ( handler->GetStatusCode () == 200 ) {
		NACL_LOG ( "payment success\n" );
		g_MessageQueue->Push ( "STORE:PaymentSuccess:" );
		RedeemProduct ( );
	}
	else if ( handler->GetStatusCode () == 404 ) {
		NACL_LOG ( "User Did not purchse\n" );
		g_instance->PostMessage ( "ALERT:STORE" );
		g_MessageQueue->Push ( "STORE:PaymentFailed:" );
	}
	else {
		NACL_LOG ( "???\n" );
		g_instance->PostMessage ( "ALERT:STORE" );
		g_MessageQueue->Push ( "STORE:PaymentFailed:" );
	}
}

void MOAIApp::DummyHttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size ) {
}

void MOAIApp::RedeemProduct ( ) {
#if PRODUCTION_URLS
	std::string url = "http://services.moaicloud.com//zipline/ziplinereceiptverification?action=redeemPurchase&userId=";
#else
	std::string url = "http://services.moaicloud.com//zipline/sandbox_ziplinereceiptverification?action=redeemPurchase&userId=";
#endif
	url.append ( g_openID );
	url.append ( "&productId=" );
	url.append ( g_productID );
	url.append ( "&applicationId=WolfToss" );

	GetURLHandler* handler = GetURLHandler::Create( g_instance, url );
	
	if (handler != NULL) {

		handler->SetMethod ( GetURLHandler::GET );
		//handler->SetUserData ( NULL );

		handler->Start( DummyHttpLoaded );
	}
}

void MOAIApp::VerifyPaymentMainThread ( void * userData, int32_t result )  {
#if PRODUCTION_URLS
	std::string url = "http://services.moaicloud.com//zipline/ziplinereceiptverification?action=getPurchaseStatus&userId=";
#else
	std::string url = "http://services.moaicloud.com//zipline/sandbox_ziplinereceiptverification?action=getPurchaseStatus&userId=";
#endif
	url.append ( g_openID );
	url.append ( "&productId=" );
	url.append ( g_productID );
	url.append ( "&applicationId=WolfToss" );

	GetURLHandler* handler = GetURLHandler::Create( g_instance, url );
	
	if (handler != NULL) {

		handler->SetMethod ( GetURLHandler::GET );
		//handler->SetUserData ( NULL );

		handler->Start( HttpLoaded );
	}
}
//----------------------------------------------------------------//
void MOAIApp::HandleStoreMessage ( std::string & message ) {

	if ( message.find( "STORE:PaymentSuccess:" ) != std::string::npos ) {

		//change to queue for bckgrd thread
		NACL_LOG ( "STORE:OnLoginSuccess!\n" );
		//g_SocialConnect->OnLoginSuccess ();

		int responseStartIndex =  strlen ( "STORE:PaymentSuccess:" );
		MOAIApp::Get ().PaymentQueueUpdatedTransactions ( TRANSACTION_STATE_PURCHASED, message.c_str () + responseStartIndex );
	}
	else if ( message.find( "STORE:PaymentFailed:" ) != std::string::npos ) {

		//change to queue for bckgrd thread
		/*int responseStartIndex =  strlen ( "STORE:OnRequestSuccess:" );
		int responseEndIndex =  message.find( "&" );

		int responseSize = responseEndIndex - responseStartIndex + 1;
		char *responseText = new char [ responseSize ];

		memset ( responseText, 0, responseSize );
		memcpy ( responseText, message.c_str () + responseStartIndex, responseSize - 1 );

		int responseId = atoi ( message.c_str () + responseEndIndex + 1 ); 

		NACL_LOG ( "MOAIApp->PaymentFailed ( %d, %s )\n", responseId, responseText );*/
		//g_SocialConnect->OnRequestSuccess ( responseId, responseText );

		int responseStartIndex =  strlen ( "STORE:PaymentFailed:" );
		MOAIApp::Get ().PaymentQueueUpdatedTransactions ( TRANSACTION_STATE_CANCELLED, message.c_str () + responseStartIndex );

		//delete [] responseText;
	}
	else if ( message.find( "STORE:PaymentClientDone:" ) != std::string::npos ) {

		int responseStartIndex =  strlen ( "STORE:PaymentClientDone:" );
		//const char *openId = message.c_str () + responseStartIndex;
		std::string params = message.substr ( responseStartIndex, message.length () - responseStartIndex );

		int midIndex = params.find ( ":" );

		g_openID = params.substr ( 0, midIndex ) ;
		g_productID = params.substr ( midIndex + 1, params.length () - midIndex );

		NACL_LOG ( "Client payment done with Id %s, product %s\n", g_openID.c_str (), g_productID.c_str ());

		pp::CompletionCallback cc ( VerifyPaymentMainThread, NULL );

		g_core->CallOnMainThread ( 0, cc , 0 );
	}
}

//----------------------------------------------------------------//
/** @lua	alert
	@text	Display a modal style dialog box with one or more buttons, including a
			cancel button. This will not halt execution (this function returns immediately),
			so if you need to respond to the user's selection, pass a callback.

	@in		string title		The title of the dialog box.
	@in		string message		The message to display.
	@in		function callback	The function that will receive an integer index as which button was pressed.
	@in		string cancelTitle	The title of the cancel button.
	@in		string... buttons	Other buttons to add to the alert box.
	@out nil
 
*/
int MOAIApp::_alert( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	//cc8* title = state.GetValue< cc8* >(1, "Alert");
	cc8* message = state.GetValue< cc8* >(2, "");
	//cc8* cancelTitle = state.GetValue< cc8*>(4, NULL);
	
	NACL_LOG ( "ALERT: %s\n", message );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	canMakePayments
	@text	Verify that the app has permission to request payments.
	
	@out	boolean canMakePayments
*/
int MOAIApp::_canMakePayments ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool result = true;
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	requestPaymentForProduct
	@text	Request payment for a product.
	
	@in		string productIdentifier
	@opt	number quantity				Default value is 1.
	@out	nil
*/
int MOAIApp::_requestPaymentForProduct ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	int quantity = state.GetValue < int >( 2, 1 );

	//AJV Google IAP doesn't care about quantity?
	char message[256];
	memset ( message, 0, 256 );
	sprintf ( message, "STORE:Buy:%s", identifier );

	NaClPostMessage ( message );

	/*if ( quantity ) {
		SKMutablePayment* payment = [ SKMutablePayment paymentWithProductIdentifier:[ NSString stringWithUTF8String:identifier ]];
		payment.quantity = quantity;
		[[ SKPaymentQueue defaultQueue ] addPayment:payment ];
	}*/

	return 0;
}


//----------------------------------------------------------------//
/**	@lua	requestProductIdentifiers
	@text	Verify the validity of a set of products.
	
	@in		table productIdentifiers			A table of product identifiers.
	@out	nil
*/
int MOAIApp::_requestProductIdentifiers ( lua_State* L ) {
	MOAILuaState state ( L );
	
	NACL_LOG ( "_requestProductIdentifiers Not implemented\n" );

	/*NSMutableSet* productSet = [[[ NSMutableSet alloc ] init ] autorelease ];
	
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
	}*/
	
	return 0;
}

//----------------------------------------------------------------//
/** @lua	restoreCompletedTransactions
	@text	Request a restore of all purchased non-consumables from the App Store.
			Use this to retrieve a list of all previously purchased items (for example
			after reinstalling the app on a different device).
	@out	nil
 
*/
int MOAIApp::_restoreCompletedTransactions( lua_State* L ) {
	
	NACL_LOG ( "_restoreCompletedTransactions Not implemented\n" );
	//[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];

	return 0;
}


//----------------------------------------------------------------//
/**	@lua	setListener
	@text	Set a callback to handle events of a type.
	
	@in		number event		One of MOAIApp.ERROR, MOAIApp.DID_REGISTER, MOAIApp.REMOTE_NOTIFICATION,
								MOAIApp.PAYMENT_QUEUE_TRANSACTION, MOAIApp.PRODUCT_REQUEST_RESPONSE.
	@opt	function handler
	@out	nil
*/
int MOAIApp::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIApp::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getDirectoryInDomain
	@text	Search the platform's internal directory structure for a special directory
			as defined by the platform.
 
	@in		number domain		One of MOAIApp.DOMAIN_DOCUMENTS, MOAIApp.DOMAIN_APP_SUPPORT
	@out	string directory	The directory associated with the given domain.
*/
int MOAIApp::_getDirectoryInDomain ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 dirCode = state.GetValue<u32>( 1, 0 ); 
	
	lua_pushstring(L, "data");

	return 1;
}

int MOAIApp::_openURL ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* url	= state.GetValue < cc8* >( 1, "" );
	
	NACL_LOG ("MOAIApp should open url: %s\n", url );

	char message[2048];
	memset ( message, 0, 2048 );
	sprintf ( message, "URL:Open:%s", url );

	NaClPostMessage ( message );
	
	return 0;
}

int MOAIApp::_openURLPOP ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* url	= state.GetValue < cc8* >( 1, "" );
	
	NACL_LOG ("MOAIApp should open popup url: %s\n", url );

	char message[2048];
	memset ( message, 0, 2048 );
	sprintf ( message, "URL:OpenPop:%s", url );

	NaClPostMessage ( message );
	
	return 0;
}
 

//----------------------------------------------------------------//
MOAIApp::MOAIApp () {

	RTTI_SINGLE ( MOAILuaObject )
	
	/*this->mStoreKitListener = [[ MOAIStoreKitListener alloc ] init ];
	[[ SKPaymentQueue defaultQueue ] addTransactionObserver:this->mStoreKitListener ];
	
	MOAISim& device = MOAISim::Get ();
	device.SetUniqueIdentifier([[[UIDevice currentDevice] uniqueIdentifier] UTF8String]);

	this->mReachabilityListener = [ ReachabilityListener alloc ];
	[ this->mReachabilityListener startListener ];*/
}

//----------------------------------------------------------------//
MOAIApp::~MOAIApp () {

	//[[ SKPaymentQueue defaultQueue ] removeTransactionObserver:this->mStoreKitListener];
	//[ this->mStoreKitListener release ];
}

//----------------------------------------------------------------//
void MOAIApp::OnInit () {
}

//----------------------------------------------------------------//
void MOAIApp::Reset () {
	for( int i = 0 ; i < TOTAL; i++ )
		mListeners[i].Clear();
}

//----------------------------------------------------------------//
void MOAIApp::DidReceivePaymentQueueError ( cc8* extraInfo ) {
	
	MOAILuaRef& callback = this->mListeners [ PAYMENT_QUEUE_ERROR ];
	
	if ( callback ) {
		/*MOAIScopedLuaState state = callback.GetSelf ();
		
		[ error toLua:state ];
		lua_pushstring(state, extraInfo);
		
		state.DebugCall ( 2, 0 );*/
	}
}

//----------------------------------------------------------------//
void MOAIApp::PaymentQueueUpdatedTransactions ( int transactionResult, const char *extraInfo )  {
	//UNUSED ( queue );

	MOAILuaRef& callback = this->mListeners [ PAYMENT_QUEUE_TRANSACTION ];

	//for ( SKPaymentTransaction* transaction in transactions ) {
	
		if ( callback ) {
		
			MOAIScopedLuaState state = callback.GetSelf ();
			this->PushPaymentTransaction ( transactionResult, state );
			state.DebugCall ( 1, 0 );
		}
		
		//if ( transaction.transactionState != SKPaymentTransactionStatePurchasing ) {
		//	[[ SKPaymentQueue defaultQueue ] finishTransaction:transaction ];
		//}
	//}
}

//----------------------------------------------------------------//
void MOAIApp::ProductsRequestDidReceiveResponse () {

	MOAILuaRef& callback = this->mListeners [ PRODUCT_REQUEST_RESPONSE ];
	if ( callback ) {
		
		/*MOAIScopedLuaState state = callback.GetSelf ();
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
		
		// Note: If you're testing in-app purchases, chances are your product Id
		// will take time to propagate into Apple's sandbox/test environment and
		// thus the id's will be invalid for hours(?) (at the time of writing this).
		for ( NSString *invalidProductId in response.invalidProductIdentifiers )
		{
			NSLog(@"StoreKit: Invalid product id: %@" , invalidProductId);
		}*/
		
		//state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIApp::PushPaymentTransaction ( int transactionResult, lua_State *L ) {

	//MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	//lua_State *L = state;
	lua_newtable ( L );
	
	lua_pushstring ( L, "transactionState" );
	
	switch ( transactionResult ) {
		
		case TRANSACTION_STATE_PURCHASED: {
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASED );
			break;
		}
		case TRANSACTION_STATE_FAILED: 
		case TRANSACTION_STATE_CANCELLED: {
			lua_pushnumber ( L, TRANSACTION_STATE_CANCELLED );
			break;
		}
		case TRANSACTION_STATE_RESTORED: {
			lua_pushnumber ( L, TRANSACTION_STATE_RESTORED );
			break;
		}
		default: {
			lua_pushnil ( L );
			break;
		}
	}
	
	lua_settable ( L, -3 );
	
	if ( TRANSACTION_STATE_PURCHASED ) {
		
		lua_pushstring ( L, "payment" );
		lua_newtable ( L );
		
		lua_pushstring ( L, "productIdentifier" );
		//[ transaction.payment.productIdentifier toLua:L ];
		//AJV TODO, use product Id
		lua_pushstring ( L, g_productID.c_str () );
		lua_settable ( L, -3 );
		
		lua_pushstring ( L, "quantity" );
		//AJV warning hardcoded to 1
		lua_pushnumber ( L, 1 );
		lua_settable ( L, -3 );
		
		lua_settable ( L, -3 );
	}
	
	/*if ( transaction.transactionState == SKPaymentTransactionStateFailed ) {
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
		if( transaction.transactionReceipt != nil )
		{
			[ transaction.transactionReceipt toLua:L ];
		}
		else
		{
			lua_pushnil( L );
		}

		lua_settable ( L, -3 );
	}
	
	if (( transaction.transactionState == SKPaymentTransactionStatePurchased ) || ( transaction.transactionState == SKPaymentTransactionStateRestored )) {
		
		lua_pushstring ( L, "transactionDate" );
		[ transaction.transactionDate toLua:L ];
		lua_settable ( L, -3 );

		lua_pushstring ( L, "transactionIdentifier" );
		[ transaction.transactionIdentifier toLua:L ];
		lua_settable ( L, -3 );
	}*/
}

//----------------------------------------------------------------//
void MOAIApp::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "PAYMENT_QUEUE_TRANSACTION",	( u32 )PAYMENT_QUEUE_TRANSACTION );
	state.SetField ( -1, "PAYMENT_QUEUE_ERROR",			( u32 )PAYMENT_QUEUE_ERROR );
	state.SetField ( -1, "PRODUCT_REQUEST_RESPONSE",	( u32 )PRODUCT_REQUEST_RESPONSE );
	
	state.SetField ( -1, "DOMAIN_DOCUMENTS",			( u32 )DOMAIN_DOCUMENTS );
	state.SetField ( -1, "DOMAIN_APP_SUPPORT",			( u32 )DOMAIN_APP_SUPPORT );

	state.SetField ( -1, "TRANSACTION_STATE_PURCHASING",( u32 )TRANSACTION_STATE_PURCHASING );
	state.SetField ( -1, "TRANSACTION_STATE_PURCHASED", ( u32 )TRANSACTION_STATE_PURCHASED );
	state.SetField ( -1, "TRANSACTION_STATE_FAILED",    ( u32 )TRANSACTION_STATE_FAILED );
	state.SetField ( -1, "TRANSACTION_STATE_RESTORED",  ( u32 )TRANSACTION_STATE_RESTORED );
	state.SetField ( -1, "TRANSACTION_STATE_CANCELLED", ( u32 )TRANSACTION_STATE_CANCELLED );
	
	luaL_Reg regTable[] = {
		{ "alert",								_alert },

		{ "canMakePayments",					_canMakePayments },
		{ "restoreCompletedTransactions",		_restoreCompletedTransactions },
		{ "requestPaymentForProduct",			_requestPaymentForProduct },
		{ "requestProductIdentifiers",			_requestProductIdentifiers },

		{ "setListener",						_setListener },
		{ "getDirectoryInDomain",				_getDirectoryInDomain },
		{ "openURL",							_openURL },
		{ "openURLPOP",							_openURLPOP },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}