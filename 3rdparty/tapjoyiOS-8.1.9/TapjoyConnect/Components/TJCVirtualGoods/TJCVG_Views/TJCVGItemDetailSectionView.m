// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGItemDetailSectionView.h"


@implementation TJCVGItemDetailSectionView

@synthesize gradient;


- (id)initWithFrame:(CGRect)frame 
{
    
    self = [super initWithFrame:frame];
    if (self) 
	{
        // Initialization code.
    }
    return self;
}


- (void)dealloc 
{
	[gradient release];
	
    [super dealloc];
}


- (void)layoutSubviews
{
	gradient.frame = self.bounds;
}


@end
