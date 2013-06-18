// Copyright (C) 2012 by Grow Mobile, Inc.
//
// This file is part of the Grow Mobile SDK.
//
// By using the Grow Mobile SDK in your software, you agree to the terms of the Grow Mobile SDK License Agreement.
//
// The Grow Mobile SDK is bound by the Grow Mobile SDK License Agreement which can be found here:
// http://www.growmobile.com/sdk/license

//
// SDK Version: 04/15/2013
//

#import <Foundation/Foundation.h>

// Your app key and secret can be found on the dashboard at this link:
// https://dashboard.growmobile.com
#define GROW_MOBILE_APP_KEY     @"tY1X85APWOPrBFJ"
#define GROW_MOBILE_APP_SECRET  @"UD63ARvnanzBU5I"

// For examples of how to use the APIs, check out AppDelegate.m and ViewController.m in this sample app
@interface GrowMobileSDK : NSObject

// This is a required method to report an app open to Grow Mobile
// Call this method in the AppDelegate's applicationDidBecomeActive method
// The applicationDidBecomeActive is preferred than didFinishLaunchingWithOptions,
// because to track retention, we need to report every open including the case where the app
// wakes up from the background
+ (void) reportOpen;

// This is an optional method to report a user action to Grow Mobile
// The data can be used to segment users during reporting
// Call this method anywhere in your code but don't call it repeatedly with the same data
// since it will unnecessarily increase our server load and slow down your app
// The key and value parameters can be any strings and can contain spaces
+ (void) reportActionWithKey:(NSString *)key
                    andValue:(NSString *)value;

// This is an optional method to report an in-app purchase to Grow Mobile
// The data will be part of the monetization reports
// Call this method in the StoreKit's purchase completed delegate
// The currency parameter is the ISO currency codes of the NSLocale class, e.g. "USD"
// The amount parameter is e.g. "1.99"
// Grow Mobile will convert all currencies to USD and will only report in USD
+ (void) reportInAppPurchaseWithCurrency:(NSString *)currency
                               andAmount:(NSString *)amount;

// This is an optional method to report an ad view to Grow Mobile if ads are served in app
// The data will be part of the monetization reports
// Call this method in the publisher SDK's ad viewed delegate
// On the Grow Mobile dashboard you can enter the eCPM in USD for each ad network, so that we can
// calculate the earnings from ads. We don't track clicks separately since the
// eCPM should factor in clicks as well
+ (void) reportAdViewWithAdNetwork:(NSString *)adNetwork;

// This is an optional method to report an offer wall redemption to Grow Mobile if offers are served in app
// The data will be part of the monetization reports
// Call this method in the publisher SDK's offer redeemed delegate
// The usdAmount parameter is what you have earned. You should convert offer pts such as Tapjoy pts
// to USD amounts for this method
+ (void) reportOfferWallWithUsdAmount:(NSString *)usdAmount
                         andAdNetwork:(NSString *)adNetwork;

@end
