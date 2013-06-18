// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moaiext-iphone/MOAIGrowMobileIOS.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

#import "GrowMobileSDK.h"

//================================================================//
// lua
//================================================================//


//----------------------------------------------------------------//
/**	@name	reportOpen
 @text	Start the grow mobile session.
 
 @in	none
 @out	nil
 */
int MOAIGrowMobileIOS::_init( lua_State * L ) {
	
	MOAILuaState state ( L );
	
	cc8* key = state.GetValue < cc8* >( 1, "" );
    cc8* secret = state.GetValue < cc8* >( 1, "" );
	
    NSString * keyString = [ NSString stringWithUTF8String:key ];
	NSString* secretString = [ NSString stringWithUTF8String:secret ];
	
    [GrowMobileSDK initWithKey:keyString andSecret:secretString];
	
    NSLog(@"Init");
	
    return 0;
}

//----------------------------------------------------------------//
/**	@name	reportOpen
 @text	Start the grow mobile session.
 
 @in	none
 @out	nil
 */
int MOAIGrowMobileIOS::_reportOpen( lua_State * L ) {
	
	MOAILuaState state ( L );
	
    [GrowMobileSDK reportOpen];
	
    NSLog(@"Report Open Button clicked");
	
    return 0;
}

/**	@name	report
 @text	Log an event to Flurry.
 
 @in	string key	The key with the action
 @in	string value  The value of the action
 @out	nil
 */
int MOAIGrowMobileIOS::_reportAction( lua_State * L ) {
	
	MOAILuaState state ( L );
	
    NSLog(@"Report Action Button clicked");

    cc8* key = state.GetValue < cc8* >( 1, "" );
    cc8* value = state.GetValue < cc8* >( 1, "" );
	
    NSString * keyString = [ NSString stringWithUTF8String:key ];
	NSString* valueString = [ NSString stringWithUTF8String:value ];
	
    if (keyString == NULL || valueString == NULL) {
        
        return 0;
    }
    
    [GrowMobileSDK reportActionWithKey:keyString
                              andValue:valueString];
    
	return 0;
}

/**	@name	reportInAppPurchase
 @text	Start a timed event.
 
 @in	string eventName	The name of the event to send to Flurry
 @in	table params		(optional)The params to send to Flurry with the event or nil if not required.
 @out	nil
 */
int MOAIGrowMobileIOS::_reportInAppPurchase( lua_State * L ) {
	
	MOAILuaState state ( L );
	
    NSLog(@"Report In-app Purchase Button clicked");
    
    cc8* currency = state.GetValue < cc8* >( 1, "" );
    cc8* amount = state.GetValue < cc8* >( 1, "" );
	
    NSString * currencyString = [ NSString stringWithUTF8String:currency ];
	NSString* amountString = [ NSString stringWithUTF8String:amount ];
	
    if (currencyString == NULL || amountString == NULL) {
        
        return 0;
    }
    
    [GrowMobileSDK reportInAppPurchaseWithCurrency:currencyString
                                         andAmount:amountString];
    
	return 0;
}

/**	@name	reportAdView
 @text	End a timed event.
 
 @in	string eventName	The name of the event to send to Flurry
 @out	nil
 */
int MOAIGrowMobileIOS::_reportAdView( lua_State * L ) {
	
	MOAILuaState state ( L );
	
    NSLog(@"Report Ad View Button clicked");
    
    cc8* network = state.GetValue < cc8* >( 1, "" );
	
    NSString * networkString = [ NSString stringWithUTF8String:network ];
	
    if (networkString == NULL) {
        
        return 0;
    }
    
    [GrowMobileSDK reportAdViewWithAdNetwork:networkString];
    
	return 0;
}


/**	@name	reportOfferWall
 @text	End a timed event.
 
 @in	string eventName	The name of the event to send to Flurry
 @out	nil
 */
int MOAIGrowMobileIOS::_reportOfferWall( lua_State * L ) {
	
	MOAILuaState state ( L );
	
    NSLog(@"Report Offer Wall Button clicked");
    
    cc8* amount = state.GetValue < cc8* >( 1, "" );
    cc8* network = state.GetValue < cc8* >( 1, "" );
	
    NSString * amountString = [ NSString stringWithUTF8String:amount ];
	NSString* networkString = [ NSString stringWithUTF8String:network ];
	
    if (amountString == NULL || networkString == NULL) {
        
        return 0;
    }
    
    [GrowMobileSDK reportOfferWallWithUsdAmount:amountString
                                   andAdNetwork:networkString];
    
    
	return 0;
}


//================================================================//
// MOAIGrowMobileIOS
//================================================================//

//----------------------------------------------------------------//
MOAIGrowMobileIOS::MOAIGrowMobileIOS () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
}

//----------------------------------------------------------------//
MOAIGrowMobileIOS::~MOAIGrowMobileIOS () {
	
}

//----------------------------------------------------------------//
void MOAIGrowMobileIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	//printf("register Flurry '%s'\n", "test");
	
	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ "reportOpen",             _reportOpen },
		{ "reportAction",			_reportAction},
		{ "reportInAppPurchase",	_reportInAppPurchase },
		{ "reportAdView",           _reportAdView },
        { "reportOfferWall",		_reportOfferWall},
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}