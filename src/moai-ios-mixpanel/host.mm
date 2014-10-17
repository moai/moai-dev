// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-mixpanel/host.h>
#include <moai-ios-mixpanel/MOAIMixpanelIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosMixpanelAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosMixpanelAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosMixpanelContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIMixpanelIOS );
}
