// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_IOS_H
#define	AKU_MODULES_IOS_H

#include <host-modules/aku_modules_ios_config.h>
#include <host-modules/aku_modules.h>

#if AKU_WITH_IOS
	#include <moai-ios/host.h>
#endif

#if AKU_WITH_IOS_BILLING
	#include <moai-ios-billing/host.h>
#endif

#if AKU_WITH_IOS_CHARTBOOST
	#include <moai-ios-chartboost/host.h>
#endif

#if AKU_WITH_IOS_CRITTERCISM
	#include <moai-ios-crittercism/host.h>
#endif

#if AKU_WITH_IOS_FACEBOOK
	#include <moai-ios-facebook/host.h>
#endif

#if AKU_WITH_IOS_GAMECENTER
	#include <moai-ios-gamecenter/host.h>
#endif

#if AKU_WITH_IOS_MOBILE_APP_TRACKER
	#include <moai-ios-mobile-app-tracker/host.h>
#endif

#if AKU_WITH_IOS_TAPJOY
	#include <moai-ios-tapjoy/host.h>
#endif

#if AKU_WITH_IOS_VUNGLE
	#include <moai-ios-vungle/host.h>
#endif

//----------------------------------------------------------------//
void		AKUModulesIosAppFinalize				();
void		AKUModulesIosAppInitialize				();
void		AKUModulesIosContextInitialize			();
void		AKUModulesIosUpdate						();

#endif
