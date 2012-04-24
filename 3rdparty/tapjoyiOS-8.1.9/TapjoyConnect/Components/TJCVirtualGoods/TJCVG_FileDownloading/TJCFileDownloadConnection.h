// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>


@interface TJCFileDownloadConnection : NSURLConnection
{
	NSFileHandle *fh; // Download file, handle
    long double dataReceived;
    long double totalData;
	int fileNumber;	//used to represent if its datafile or not
}

@property (assign) long double dataReceived;
@property (assign) long double totalData;
@property (nonatomic, retain) NSFileHandle *fh; 
@property (assign) int fileNumber;

- (id)initWithRequest:(NSURLRequest *)request delegate:(id)delegate downloadFileHandle:(NSFileHandle *)dlFH;

@end



