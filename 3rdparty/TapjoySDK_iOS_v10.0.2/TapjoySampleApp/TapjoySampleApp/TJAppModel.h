// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>

@interface TJAppModel : NSObject

@property (nonatomic, strong) NSString* appID;
@property (nonatomic, strong) NSString* secretKey;
@property (nonatomic, assign, getter = isManagedCurrency) BOOL managedCurrency;
@property (nonatomic, strong) NSArray *currencyIDs;

- (id)initWithAppID:(NSString *)appID SecretKey:(NSString *)secretKey CurrencyIDs:(NSArray *)currencies;
- (id)initWithDictionary:(NSDictionary*)dict;

@end
