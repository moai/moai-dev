// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#define TJC_DETAIL_ALERT_PURCHASE_ID			0
#define TJC_DETAIL_ALERT_INSUFFICIENT_FUNDS_ID	1
#define TJC_DETAIL_ALERT_PURCHASE_ERROR			2


#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

@class TJCVGItemButton;
@class TJCVGStoreItem;
@class TJCVGItemDetailSectionView;

@interface TJCVGItemDetail : NSObject <UIAlertViewDelegate>
{
	IBOutlet UINavigationBar *navBar;		/*<! The navigation bar. */
	IBOutlet UIBarButtonItem *backButton;	/*!< The back button within the navigation bar. */
	IBOutlet UIView *mainView;				/*!< Everything is placed within this view. */

	IBOutlet TJCVGItemDetailSectionView *itemHeaderSection;		/*<! The top portion of the view. Contains the icon, cost, and buy button. */
	IBOutlet UIImageView *iconImage;		/*<! The icon image view of the item. */
	IBOutlet UIButton *buyButton;			/*<! The button that initiates a purchase of the item. */
	IBOutlet UILabel *costLabel;			/*<! This is label that contains the cost of the item in the itemHeaderSection. */
	IBOutlet UILabel *nameLabel;			/*<! This is label that contains the name of the item in the itemHeaderSection. */
	IBOutlet UILabel *typeLabel;			/*<! This is label that contains the type of the item in the itemHeaderSection. */	
	
	IBOutlet UIScrollView *itemDescriptionSection;	/*<! The bottom portion of the view. This contains the description of the item. */
	IBOutlet TJCVGItemDetailSectionView *itemDescriptionSectionBG;	/*<! The bottom portion of the view that serves at the background so we can more easily show a gradient. */
	IBOutlet UILabel *descItemAttributes;			/*<! Holds the text item attributes. */
	IBOutlet UILabel *descItemDescription;			/*<! Holds the text for the description of the item. */
	IBOutlet UILabel *descCostLabel;				/*<! This is label that contains the cost of the item in the itemDescriptionSection. */
	IBOutlet UILabel *descNameLabel;				/*<! This is label that contains the name of the item in the itemDescriptionSection. */
	IBOutlet UILabel *descTypeLabel;				/*<! This is label that contains the type of the item in the itemDescriptionSection. */	
	
	TJCVGStoreItem *currentItemRef;			/*<! A reference to the current item being displayed. */
	BOOL shouldRefreshOnDismiss;			/*<! Inidicates that a refresh should be initiated after the detail view has been dismissed. This is needed when an item has just been purchased. */
	TJCVGItemButton *itemButtonRef;			/*<! A reference to the current button being processed. */
}

@property (nonatomic, retain) IBOutlet UINavigationBar *navBar;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *backButton;
@property (nonatomic, retain) IBOutlet UIView *mainView;

@property (nonatomic, retain) IBOutlet UIView *itemHeaderSection;
@property (nonatomic, retain) IBOutlet UIImageView *iconImage;
@property (nonatomic, retain) IBOutlet UIButton *buyButton;
@property (nonatomic, retain) IBOutlet UILabel *costLabel;
@property (nonatomic, retain) IBOutlet UILabel *nameLabel;
@property (nonatomic, retain) IBOutlet UILabel *typeLabel;

@property (nonatomic, retain) IBOutlet UIScrollView *itemDescriptionSection;
@property (nonatomic, retain) IBOutlet UILabel *descItemAttributes;
@property (nonatomic, retain) IBOutlet UILabel *descItemDescription;
@property (nonatomic, retain) IBOutlet UILabel *descCostLabel;
@property (nonatomic, retain) IBOutlet UILabel *descNameLabel;
@property (nonatomic, retain) IBOutlet UILabel *descTypeLabel;


// The reason we want the TJCVGItemButton and not just the TJCVGStoreItem, is because we need access to the downloaded icon.
- (id)initWithItem:(TJCVGItemButton*)vgItem withFrame:(CGRect)frame withButtonEnabled:(BOOL)buttonEnabled;

- (void)updateImage;

- (IBAction)dismissView;

- (IBAction)purchaseItemAction;

// Sets the detail view color tint to the given UIColor.
- (void)setViewColor:(UIColor*)color;

- (void)initiateItemPurchase;

- (void)purchaseItemError;

@end
