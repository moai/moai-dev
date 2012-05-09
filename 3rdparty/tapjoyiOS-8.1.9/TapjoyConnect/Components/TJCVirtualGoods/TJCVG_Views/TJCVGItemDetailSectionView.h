// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>


/*!	\interface TJCVGItemDetailSectionView
 *	\brief The Tapjoy Item Detail View Section class.
 *
 * This is simply a UIView that has layoutSubView overridden so that sublayers will resize properly with the frame itself.
 */
@interface TJCVGItemDetailSectionView : UIView 
{
	CAGradientLayer *gradient;
}

@property (nonatomic, retain) CAGradientLayer *gradient;

@end
