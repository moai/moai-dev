// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-iphone/AKU-chartboost.h>
#include <moai-iphone/MOAIChartBoostIOS.h>

//================================================================//
// AKU-chartboost
//================================================================//

//----------------------------------------------------------------//
void AKUChartBoostInit () {
	
	MOAIChartBoostIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIChartBoostIOS );
}