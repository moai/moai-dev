// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import "host.h"
#import "headers.h"

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUAndroidAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAndroidAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAndroidContextInitialize () {

	MOAIAppAndroid::Affirm ();
	MOAIBrowserAndroid::Affirm ();
	MOAIDialogAndroid::Affirm ();
	MOAIMoviePlayerAndroid::Affirm ();
	MOAIKeyboardAndroid::Affirm ();
	MOAIBillingAndroid::Affirm ();
	MOAINotificationsAndroid::Affirm ();

	REGISTER_LUA_CLASS ( MOAIAppAndroid );
	REGISTER_LUA_CLASS ( MOAIBrowserAndroid );
	REGISTER_LUA_CLASS ( MOAIDialogAndroid );
	REGISTER_LUA_CLASS ( MOAIMoviePlayerAndroid );
	REGISTER_LUA_CLASS ( MOAIKeyboardAndroid );
	REGISTER_LUA_CLASS ( MOAIBillingAndroid );
	REGISTER_LUA_CLASS ( MOAINotificationsAndroid );
}
