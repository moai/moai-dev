// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-billing/host.h>
#include <moai-ios-billing/MOAIBillingIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosBillingAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosBillingAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosBillingContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIBillingIOS );
}
