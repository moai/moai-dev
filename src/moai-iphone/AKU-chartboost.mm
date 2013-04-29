// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-chartboost.h>
#include <moaiext-iphone/MOAIChartBoostIOS.h>

//================================================================//
// AKU-chartboost
//================================================================//

//----------------------------------------------------------------//
void AKUChartBoostInit () {
	
	MOAIChartBoostIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIChartBoostIOS );
}