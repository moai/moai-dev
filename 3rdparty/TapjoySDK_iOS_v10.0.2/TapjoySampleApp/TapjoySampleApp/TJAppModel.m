// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJAppModel.h"

@implementation TJAppModel

- (id)initWithAppID:(NSString *)appID SecretKey:(NSString *)secretKey CurrencyIDs:(NSArray *)currencies
{
    if (self = [super init])
    {
        _appID = appID;
        _secretKey = secretKey;
        _currencyIDs = currencies;
        
        if (_currencyIDs && _currencyIDs.count > 1)
            _managedCurrency = NO;
        else
            _managedCurrency = YES;
    }
    
    return self;
}


- (id)initWithDictionary:(NSDictionary*)dict
{
	self = [super init];
	
	if (self)
	{
		_appID = dict[@"AppID"];
		_secretKey = dict[@"SecretKey"];
		_currencyIDs = dict[@"Currencies"];
		
		if (_currencyIDs && _currencyIDs.count > 1)
            _managedCurrency = NO;
        else
            _managedCurrency = YES;
	}
	
	return self;
}


- (void)encodeWithCoder:(NSCoder*)encoder
{
	[encoder encodeObject:_appID forKey:@"appID"];
	[encoder encodeObject:_secretKey forKey:@"secretKey"];
	[encoder encodeObject:_currencyIDs forKey:@"currencyIDs"];
}


- (id)initWithCoder:(NSCoder*)decoder
{
	self = [super init];
	
	if (self)
	{
		_appID = [decoder decodeObjectForKey:@"appID"];
		_secretKey = [decoder decodeObjectForKey:@"secretKey"];
		_currencyIDs = [decoder decodeObjectForKey:@"currencyIDs"];
	}
	
	return self;
}




@end
