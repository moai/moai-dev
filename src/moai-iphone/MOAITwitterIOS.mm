// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <StoreKit/StoreKit.h>
#import <Twitter/Twitter.h>
#import <moaiext-iphone/MOAITwitterIOS.h>
#import <moaiext-iphone/MOAIStoreKitListener.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canTweet
 	@text	Determines whether or not Twitter interaction is enabled.
 
 	@out	bool canTweet
*/
int MOAITwitterIOS::_canTweet ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if ( NSClassFromString ( @"TWTweetComposeViewController" ) == nil ) {
		
		lua_pushboolean ( state, false );
		return 1;
	}
	
	BOOL result = [ TWTweetComposeViewController canSendTweet ];
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	composeTweet
	@text	Opens a view to compose a tweet.
 
	@opt	string text				The default text of the tweet. Default is nil.
	@opt	string url				A URL to add to the tweet. Default is nil.
	@out	nil
 */
int MOAITwitterIOS::_composeTweet ( lua_State* L ) {	
	
	MOAILuaState state ( L );
	
	cc8* text	= state.GetValue < cc8* >( 1, "" );
	cc8* url	= state.GetValue < cc8* >( 2, "" );
	
	if ( NSClassFromString ( @"TWTweetComposeViewController" ) == nil ) {
		
		return 0;
	}
	
	if ( ![ TWTweetComposeViewController canSendTweet ]) {
		
		return 0;
	}
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];
	
	// Set up the built-in twitter composition view controller.
    TWTweetComposeViewController *tweetViewController = [[ TWTweetComposeViewController alloc ] init ];
    
    // Set the initial tweet text. See the framework for additional properties that can be set.
	if ( text ) {
		
		[ tweetViewController setInitialText: [ NSString stringWithUTF8String:text ]];
	}
	
	if ( url ) {
		
		[ tweetViewController addURL: [ NSURL URLWithString:[ NSString stringWithUTF8String:url ]]]; 
	}
    
    // Create the completion handler block.
    [ tweetViewController setCompletionHandler:^( TWTweetComposeViewControllerResult result ) {
				
		if ( result == TWTweetComposeViewControllerResultCancelled ) {
			MOAITwitterIOS::Get ().DidSendTweet ( false );
		}
		else if ( result == TWTweetComposeViewControllerResultDone ) {
			MOAITwitterIOS::Get ().DidSendTweet ( true );
		}
		
        // Dismiss the tweet composition view controller.
		if  ( rootVC != nil ) {
			[ rootVC dismissModalViewControllerAnimated:YES ];
		}
    }];
    
	if  ( rootVC != nil ) {
		
		[ rootVC presentModalViewController:tweetViewController animated:YES ];
	}
	
	[ tweetViewController release ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAITwitterIOS::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAITwitterIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAITwitterIOS
//================================================================//

//----------------------------------------------------------------//
void MOAITwitterIOS::DidSendTweet ( bool success ) {

	if ( success ) {
		
		MOAILuaRef& callback = this->mListeners [ TWEET_SUCCESSFUL ];
		
		if ( callback ) {
			
			MOAILuaStateHandle state = callback.GetSelf ();
			
			state.DebugCall ( 0, 0 );
		}
	}
	else {
		
		MOAILuaRef& callback = this->mListeners [ TWEET_CANCELLED ];
		
		if ( callback ) {
			
			MOAILuaStateHandle state = callback.GetSelf ();
			
			state.DebugCall ( 0, 0 );
		}
	}	
}

//----------------------------------------------------------------//
MOAITwitterIOS::MOAITwitterIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITwitterIOS::~MOAITwitterIOS () {
	
}

//----------------------------------------------------------------//
void MOAITwitterIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TWEET_SUCCESSFUL",	( u32 )TWEET_SUCCESSFUL );
	state.SetField ( -1, "TWEET_CANCELLED",		( u32 )TWEET_CANCELLED );
	
	luaL_Reg regTable[] = {
		{ "canTweet",		_canTweet },
		{ "composeTweet",	_composeTweet },
		{ "setListener",	_setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}