// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCFileDownloadConnection.h"


@implementation TJCFileDownloadConnection


@synthesize dataReceived; // Objects
@synthesize totalData, fh,fileNumber; // primitive variables

- (id)initWithRequest:(NSURLRequest *)request delegate:(id)delegate downloadFileHandle:(NSFileHandle *)dlFH
{
    if ((self = [super initWithRequest:request delegate:delegate]))
	{
        dataReceived = 0;
        totalData = 0;
		fh = [dlFH retain];
	}
    return self;
}

- (void)dealloc
{
	[fh closeFile];
    [fh release];
    [super dealloc];
}

@end
