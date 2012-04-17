// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGTableViewCell.h"


@implementation TJCVGTableViewCell

@synthesize typeLabel, nameLabel, costLabel, iconImage, bgView, gradient, downloadedLabel, reuseIdentifierStr;


- (NSString *) reuseIdentifier 
{
	return reuseIdentifierStr;
}

- (void)dealloc
{
	[typeLabel release];
	[nameLabel release];
	[costLabel release];
	[iconImage release];
	[bgView release];
	[gradient release];
	
	[super dealloc];
}

@end
