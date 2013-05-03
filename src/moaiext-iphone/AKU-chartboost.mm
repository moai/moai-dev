// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CHARTBOOST

#include <moaiext-iphone/AKU-chartboost.h>
#include <moaiext-iphone/MOAIChartBoostIOS.h>

//================================================================//
// AKU-chartboost
//================================================================//

//----------------------------------------------------------------//
void AKUChartBoostInit () {
	
	MOAIChartBoostIOS::Affirm();

#ifndef DISABLE_TWITTER
	REGISTER_LUA_CLASS ( MOAIChartBoostIOS );
#endif
}

#endif