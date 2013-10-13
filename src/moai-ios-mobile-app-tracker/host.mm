// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-mobile-app-tracker/host.h>
#include <moai-ios-mobile-app-tracker/MOAIMobileAppTrackerIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosMobileAppTrackerAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosMobileAppTrackerAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosMobileAppTrackerContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIMobileAppTrackerIOS );
}
