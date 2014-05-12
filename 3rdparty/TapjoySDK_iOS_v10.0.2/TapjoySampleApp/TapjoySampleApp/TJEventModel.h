// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>

@interface TJEventModel : NSObject<UITableViewDelegate, UITableViewDataSource>

@property NSString *name;
@property NSString *value;

- (id) initWithName:(NSString*)name value:(NSString*)value;

@end
