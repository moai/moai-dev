// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCLog.h"

//#warning change to LOG_NONFATAL_ERROR for production builds vs. LOG_ALL...
static int _logThreshold =  LOG_DEBUG;

@implementation TJCLog
#pragma mark -
#pragma mark Class Methods

+ (void) setLogThreshold: (int) myThreshhold;
{
    _logThreshold = myThreshhold;
}

+ (void) logWithLevel: (int) myLevel format: (NSString *) myFormat, ...;
{
    va_list s;
    NSString * localFormat = nil;
	
    if (myLevel > _logThreshold) {
        return;
    }
	
    localFormat = [NSString stringWithFormat: @"[Log Level: %d]; %@\n", myLevel, myFormat];
    
    va_start(s, myFormat);
    NSLogv(localFormat, s);
    va_end(s);
}

@end
