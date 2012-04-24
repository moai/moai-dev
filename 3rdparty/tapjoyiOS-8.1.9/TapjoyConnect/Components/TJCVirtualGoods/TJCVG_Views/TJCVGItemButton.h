// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

@class TJCVGItemButtonView;
@class TJCVGStoreItem;
@class TJCVGItemDetail;

@interface TJCVGItemButton : NSObject <UIGestureRecognizerDelegate>
{
	IBOutlet TJCVGItemButtonView *itemButton;
	TJCVGStoreItem *storeItemRef;

	TJCVGItemDetail *itemDetail;
}

@property (nonatomic, retain) IBOutlet TJCVGItemButtonView *itemButton;
@property (nonatomic, retain) TJCVGStoreItem *storeItemRef;

- (IBAction)itemSelected;

@end
