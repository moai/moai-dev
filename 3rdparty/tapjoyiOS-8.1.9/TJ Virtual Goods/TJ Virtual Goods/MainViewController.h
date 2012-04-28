// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import "TapjoyConnect.h"

@interface MainViewController : UIViewController <TJCAdDelegate>
{
	
	IBOutlet UIButton *showOffersBtn;
	IBOutlet UIButton *refreshAdBtn;
	IBOutlet UIButton *showFeaturedBtn;
	IBOutlet UIButton *getTapPointsBtn;
	IBOutlet UIButton *spendTapPointsBtn;
	IBOutlet UIButton *awardTapPointsBtn;
	IBOutlet UIButton *showVGBtn;
	IBOutlet UILabel *tapPointsLabel;
}

@property (nonatomic, retain) IBOutlet UIButton *showFeaturedBtn;
@property (nonatomic, retain) IBOutlet UILabel *tapPointsLabel;

- (IBAction)showOffersBtnWithNavBarAction:(id)sender;
- (IBAction)showFeaturedAdAction:(id)sender;
- (IBAction)refreshAdAction:(id)sender;
- (IBAction)getTapPointsAction:(id)sender;
- (IBAction)spendTapPointsAction:(id)sender;
- (IBAction)showVirtualGoodsBtnAction:(id)sender;
- (IBAction)awardTapPointsAction:(id)sender;

@end
