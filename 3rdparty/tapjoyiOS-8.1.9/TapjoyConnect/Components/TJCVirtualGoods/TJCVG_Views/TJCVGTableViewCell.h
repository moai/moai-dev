// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

@interface TJCVGTableViewCell : UITableViewCell 
{
	IBOutlet UILabel *typeLabel;
	IBOutlet UILabel *nameLabel;
	IBOutlet UILabel *costLabel;
	IBOutlet UIImageView *iconImage;
	IBOutlet UIView *bgView;
	CAGradientLayer *gradient;			/*<! The gradient layer is used as the background for the table cell. */
	IBOutlet UILabel *downloadedLabel;
	NSString *reuseIdentifierStr;		/*<! Used to differentiate between purchased and available items. */
}

@property (nonatomic, retain) IBOutlet UILabel *typeLabel;
@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
@property (nonatomic, retain) IBOutlet UILabel *costLabel;
@property (nonatomic, retain) IBOutlet UIImageView *iconImage;
@property (nonatomic, retain) IBOutlet UIView *bgView;
@property (nonatomic, retain) CAGradientLayer *gradient;
@property (nonatomic, retain) IBOutlet UILabel *downloadedLabel;
@property (nonatomic, copy) NSString *reuseIdentifierStr;


@end
