// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>

#define USER_DEFAULT_KEY_SELECTED_INDEX     @"selectedIndex"
#define USER_DEFAULT_KEY_SELECTED_CURRENCY_INDEX    @"selectedCurrencyIndex"

static const CGFloat TJCornerRadius = 5;

@interface TJUtils : NSObject

+ (UIView*)applyRoundedMaskToView:(UIView*)view
						radius_tl:(CGFloat)radius_tl
						radius_tr:(CGFloat)radius_tr
						radius_bl:(CGFloat)radius_bl
						radius_br:(CGFloat)radius_br;

+ (UIView*)applyTopRoundedCorners:(UIView*)view;
+ (UIView*)applyBottomRoundedCorners:(UIView*)view;
+ (UIView*)applyRoundedCorners:(UIView*)view;

+ (BOOL)isValidAppID:(NSString*)appID;

+ (BOOL)isValidCurrencyID:(NSString*)currencyID;

+ (BOOL)isValidSecretKey:(NSString*)secretKey;

@end
