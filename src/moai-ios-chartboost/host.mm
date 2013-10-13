// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-chartboost/host.h>
#include <moai-ios-chartboost/MOAIChartBoostIOS.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosChartBoostAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosChartBoostAppInitialize () {
}

//----------------------------------------------------------------//
void AKUIosChartBoostContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIChartBoostIOS );
}
