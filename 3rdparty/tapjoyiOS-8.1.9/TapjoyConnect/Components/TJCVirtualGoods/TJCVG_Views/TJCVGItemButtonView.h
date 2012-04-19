// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>


@interface TJCVGItemButtonView : UIButton 
{
	IBOutlet UILabel *itemNameLabel;
	IBOutlet UILabel *itemTypeLabel;
	IBOutlet UILabel *itemCostLabel;
	IBOutlet UIImageView *buttonImage;
	NSURLConnection *connection;
	NSMutableData *data;
	NSInvocation *invoker;
}

@property (nonatomic, retain) IBOutlet UILabel *itemNameLabel;
@property (nonatomic, retain) IBOutlet UILabel *itemTypeLabel;
@property (nonatomic, retain) IBOutlet UILabel *itemCostLabel;
@property (nonatomic, retain) IBOutlet UIImageView *buttonImage;

// Adjusts the button view to look better by applying gradients, borders, shadows, and rounded corners.
- (void)reformatViewWithFrame:(CGRect)frame;

- (void)requestThumbImageWithURL:(NSString*)thumbImageURL;

- (void)setImageReadyInvocation:(NSInvocation*)invocation;

@end