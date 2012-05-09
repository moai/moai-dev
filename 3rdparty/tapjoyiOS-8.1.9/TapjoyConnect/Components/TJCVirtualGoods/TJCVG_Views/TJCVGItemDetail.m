// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGItemDetail.h"
#import "TJCVGConstants.h"
#import "TJCVGStoreModel.h"
#import "TJCVGStoreManager.h"
#import "TJCVGItemButton.h"
#import "TJCVGItemButtonView.h"
#import "TJCViewCommons.h"
#import "TJCVGUtil.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCFileDownloadHandler.h"
#import "TJCVGViewHandler.h"
#import "TJCVGView.h"
#import "TJCVGItemDetailSectionView.h"
#import "TJCConstants.h"
#import "TJCUserAccountModel.h"


@implementation TJCVGItemDetail

@synthesize mainView, itemHeaderSection, itemDescriptionSection, navBar, backButton, iconImage, buyButton, 
costLabel, nameLabel, typeLabel, descItemAttributes, descItemDescription, descCostLabel, descNameLabel, descTypeLabel;



-(void) addObserversForPurchasedItem
{
	// Right after a purchase has been successfully verified, fire the selector.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(purchaseItemComplete:) 
																name:TJC_VG_ITEM_PURCHASE_RETURN 
															 object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(purchaseItemError) 
																name:TJC_VG_ITEM_PURCHASE_NET_ERROR
															 object:nil];
	
	// Right after the purchase has been completed, fire the download selector method.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(initiateItemDownload:) 
																name:TJC_VG_PURCHASE_SUCCESS_NOTIFICATION 
															 object:nil];
	
//	[[NSNotificationCenter defaultCenter] addObserver:self 
//														  selector:@selector(itemDownloadComplete) 
//																name:	ITEM_DOWNLOAD_SUCCESS_NOTIFICATION
//															 object:nil];
}


-(void) removeObserverForPurchasedItem
{
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_VG_ITEM_PURCHASE_RETURN object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_VG_ITEM_PURCHASE_NET_ERROR object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_VG_PURCHASE_SUCCESS_NOTIFICATION object:nil];
}


- (id)initWithItem:(TJCVGItemButton*)vgItem withFrame:(CGRect)frame withButtonEnabled:(BOOL)buttonEnabled
{
	self = [super init];
	
	if (self)
	{
		mainView = [[[NSBundle mainBundle] loadNibNamed:@"TJCVGItemDetail" owner:self options:nil] objectAtIndex:0];
		itemButtonRef = [vgItem retain];
		
		//mainView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		mainView.frame = frame;
		
		// If this detail view is loading from the purchased view, then disable the buy button.
		[buyButton setUserInteractionEnabled:buttonEnabled];
		if (!buttonEnabled)
		{
			[buyButton setAlpha:0.0f];
		}
		
		// Set rounded corners for the separate views.
		itemHeaderSection.layer.cornerRadius = TJCVG_ITEMDETAIL_CORNER_RADIUS;
		itemDescriptionSection.layer.cornerRadius = TJCVG_ITEMDETAIL_CORNER_RADIUS;
		itemDescriptionSectionBG.layer.cornerRadius = TJCVG_ITEMDETAIL_CORNER_RADIUS;
		
		currentItemRef = [[vgItem storeItemRef] retain];
		
		// Populate all the values in the upper portion of the view.
		if (!itemButtonRef.itemButton.buttonImage.image)
		{
			// If the image is not yet downloaded, defer loading.
			NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[self methodSignatureForSelector:@selector(updateImage)]];
			[invocation setTarget:self];
			[invocation setSelector:@selector(updateImage)];
			
			[[itemButtonRef itemButton] setImageReadyInvocation:invocation];
		}
		else 
		{
			iconImage.image = [TJCVGUtil makeRoundCornerImage:itemButtonRef.itemButton.buttonImage.image 
																			 :TJCVG_BUTTON_CORNER_RADIUS 
																			 :TJCVG_BUTTON_CORNER_RADIUS];
			
			// Create a reflected image of the icon.
			UIImage *reflectedImage = [TJCVGUtil reflectedImage:iconImage withHeight:(iconImage.frame.size.height / 2)];
			UIImageView *reflectedImageView = [[UIImageView alloc] initWithImage:reflectedImage];
			
			// Add the reflected image to the view right below the actual image.
			[reflectedImageView setFrame:CGRectMake(
																 iconImage.frame.origin.x, 
																 iconImage.frame.origin.y + iconImage.frame.size.height + 2,
																 reflectedImageView.frame.size.width, 
																 reflectedImageView.frame.size.height)];
			[itemHeaderSection addSubview:reflectedImageView];
			
			// reflectedImage will autorelease but reflectedImageView needs to be released manually.
			[reflectedImageView release];
		}
		
		nameLabel.text = [currentItemRef itemName];
		typeLabel.text = [currentItemRef itemTypeName];
		
		// Only display cost if we're viewing this from the available items view.
		if ([[TJCVGView sharedTJCVGView] getCurrentActiveView] == TJCVG_ALL_ITEMS_VIEW)
		{
			costLabel.text = [NSString stringWithFormat:@"%d %@", [currentItemRef itemPrice], [currentItemRef itemCurrencyName]];
		}
		else if ([[TJCVGView sharedTJCVGView] getCurrentActiveView] == TJCVG_PURCHASED_ITEMS_VIEW)
		{
			costLabel.text = @"Downloaded";
		}
		
		// Populate the bottom portion of the view.
		descNameLabel.text = [NSString stringWithFormat:@"Name: %@", [currentItemRef itemName]];
		descTypeLabel.text = [NSString stringWithFormat:@"Type: %@", [currentItemRef itemTypeName]];
		descCostLabel.text = [NSString stringWithFormat:@"Cost: %d %@", [currentItemRef itemPrice], [currentItemRef itemCurrencyName]];
		
		NSString *attributeString = nil;
		// Used to calculate where to place the description.
		int lineCounter = 0;
		for (TJCVGStoreItemAttribute *attribute in [currentItemRef itemAttributes])
		{
			NSString *currentAttrib = [NSString stringWithFormat:@"%@: %@", [attribute attributeType], [attribute attributeValue]];
			
			// Special handling for first attribute since we don't want unneeded newlines.
			if (lineCounter == 0)
			{
				attributeString = [NSString stringWithString:currentAttrib];
			}
			else
			{
				// Append the new attribute with a newline.
				attributeString = [attributeString stringByAppendingFormat:@"\n%@", currentAttrib];
				// An extra increment here to account for the two line breaks.
				//lineCounter++;
			}
			
			lineCounter++;
		}
		descItemAttributes.frame = CGRectMake(
														  descItemAttributes.frame.origin.x, 
														  descItemAttributes.frame.origin.y,
														  descItemAttributes.frame.size.width, 
														  descItemAttributes.frame.size.height * lineCounter);
		descItemAttributes.text = [NSString stringWithString:attributeString];
		
		descItemDescription.frame = CGRectMake(
															descItemDescription.frame.origin.x, 
															descItemAttributes.frame.origin.y + descItemAttributes.frame.size.height, 
															descItemDescription.frame.size.width, 
															descItemDescription.frame.size.height);
		descItemDescription.text = [NSString stringWithFormat:@"Description:\n%@", [currentItemRef itemDescription]];
		
		itemDescriptionSection.scrollEnabled = YES;
		itemDescriptionSection.contentSize = CGSizeMake(itemDescriptionSection.contentSize.width, 
																		descItemDescription.frame.origin.y + descItemDescription.frame.size.height);
		
		shouldRefreshOnDismiss = NO;
		
		[self setViewColor:[[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor]];
	}
	
	return self;
}


- (void)updateImage
{
	iconImage.image = [TJCVGUtil makeRoundCornerImage:itemButtonRef.itemButton.buttonImage.image 
																	 :TJCVG_BUTTON_CORNER_RADIUS 
																	 :TJCVG_BUTTON_CORNER_RADIUS];
	
	// Create a reflected image of the icon.
	UIImage *reflectedImage = [TJCVGUtil reflectedImage:iconImage withHeight:(iconImage.frame.size.height / 2)];
	UIImageView *reflectedImageView = [[UIImageView alloc] initWithImage:reflectedImage];
	
	// Add the reflected image to the view right below the actual image.
	[reflectedImageView setFrame:CGRectMake(
														 iconImage.frame.origin.x, 
														 iconImage.frame.origin.y + iconImage.frame.size.height + 2,
														 reflectedImageView.frame.size.width, 
														 reflectedImageView.frame.size.height)];
	[itemHeaderSection addSubview:reflectedImageView];
	
	[iconImage setAlpha:0.0f];
	[reflectedImageView setAlpha:0.0f];
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ALPHA];
	[iconImage setAlpha:1.0f];
	[reflectedImageView setAlpha:1.0f];
	[UIView commitAnimations];
	
	// reflectedImage will autorelease but reflectedImageView needs to be released manually.
	[reflectedImageView release];
}


- (void)setViewColor:(UIColor*)color
{
	// Create a gradient for the different sections to make it look prettier.
	//UIColor *lighterColor = [TJCVGUtil getUserDefinedColorWithWhiteValue:TJCVG_COLOR_LIGHTEN_VALUE];
	UIColor *lighterColor1 = [TJCVGUtil getUserDefinedColorWithWhiteValue:TJCVG_COLOR_LIGHTEN_VALUE1];	
	
	[mainView setBackgroundColor:color];
	[navBar setTintColor:color];
	
	// The sections are to be a lighter tint.
	[itemHeaderSection setBackgroundColor:color];	// Set the background view since sometimes the layer gets out of sync during rotation.
	
	// Remove any previous gradient to prevent artifacts.
	for (int i = 0; i < [itemHeaderSection.layer sublayers].count; ++i)
	{
		if ([[[itemHeaderSection.layer sublayers] objectAtIndex:i] isKindOfClass:[CAGradientLayer class]])
		{
			[[[itemHeaderSection.layer sublayers] objectAtIndex:i] removeFromSuperlayer];
		}
	}
	
	// Add a gradient to the background.
	itemHeaderSection.gradient = [CAGradientLayer layer];
	itemHeaderSection.gradient.frame = [itemHeaderSection bounds];
	itemHeaderSection.gradient.cornerRadius = TJCVG_BUTTON_CORNER_RADIUS;
	itemHeaderSection.gradient.colors = [NSArray arrayWithObjects:
													 (id)[lighterColor1 CGColor], 
													 (id)[[UIColor whiteColor] CGColor], 
													 nil];
	[itemHeaderSection.layer insertSublayer:itemHeaderSection.gradient atIndex:0];
	
	// Remove any previous gradient to prevent artifacts.
	for (int i = 0; i < [itemDescriptionSectionBG.layer sublayers].count; ++i)
	{
		if ([[[itemDescriptionSectionBG.layer sublayers] objectAtIndex:i] isKindOfClass:[CAGradientLayer class]])
		{
			[[[itemDescriptionSectionBG.layer sublayers] objectAtIndex:i] removeFromSuperlayer];
		}
	}
	
	// Add a gradient to the background.
	// Notice that we don't actually change the bg color of the actual scroll view. 
	// This is because we want this bg view to show through the scroll view.
	// This avoids complications since the layer being added to the scrollview would be scrolled as well.
	itemDescriptionSectionBG.gradient = [CAGradientLayer layer];
	itemDescriptionSectionBG.gradient.frame = [itemDescriptionSectionBG bounds];
	itemDescriptionSectionBG.gradient.cornerRadius = TJCVG_BUTTON_CORNER_RADIUS;
	itemDescriptionSectionBG.gradient.colors = [NSArray arrayWithObjects:
															  (id)[lighterColor1 CGColor], 
															  (id)[[UIColor whiteColor] CGColor], 
															  nil];
	[itemDescriptionSectionBG.layer insertSublayer:itemDescriptionSectionBG.gradient atIndex:0];
}


- (IBAction)dismissView
{
	[TJCViewCommons animateTJCView:mainView 
					 withTJCTransition:TJCTransitionNormalToBottom
								withDelay:TJC_DEFAULT_TRANSITION_DELAY];
	
	// After the animation, remove the view from the window.
	[self performSelector:@selector(removeView) withObject:nil afterDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
	
	// Determine if a refresh is required.
	if (shouldRefreshOnDismiss)
	{
		[[TJCVGView sharedTJCVGView] refreshItems];
	}
}


- (void)removeView
{
	[mainView removeFromSuperview];
}


- (IBAction)purchaseItemAction
{
	// Disable the buy button.
	[buyButton setUserInteractionEnabled:NO];
	[buyButton setAlpha:0.5f];
	
	UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
																		  message:@"Are you sure you want to purchase this item?" 
																		 delegate:self 
															 cancelButtonTitle:@"Cancel"
															 otherButtonTitles:@"OK", nil];	
	
	alertview.tag = TJC_DETAIL_ALERT_PURCHASE_ID;
	
	[alertview show];
	[alertview release];
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (alertView.tag == TJC_DETAIL_ALERT_PURCHASE_ID)
	{
		// The cancel button
		if (buttonIndex == 0)
		{
			// Re-enable the buy button.
			[buyButton setUserInteractionEnabled:YES];
			[buyButton setAlpha:1.0f];
			
			[self removeObserverForPurchasedItem];
		}
		// The OK button. Initiate a purchase.
		else if (buttonIndex == 1)
		{
			[self initiateItemPurchase];
		}
	}
	else if (alertView.tag == TJC_DETAIL_ALERT_INSUFFICIENT_FUNDS_ID)
	{
		// The cancel button
		if (buttonIndex == 0)
		{
			// Re-enable the buy button.
			[buyButton setUserInteractionEnabled:YES];
			[buyButton setAlpha:1.0f];
		}
		// The 'get more' button. Open the offer wall here.
		else if (buttonIndex == 1)
		{
			UIView *offersWebView = [TJCOffersViewHandler showOffersWithFrame:[mainView bounds]];
			
			[mainView addSubview:offersWebView];
			
			// Re-enable the buy button.
			[buyButton setUserInteractionEnabled:YES];
			[buyButton setAlpha:1.0f];
		}
		
		// Re-enable back button no matter what at this point. We don't want to lock the user in this screen for any reason.
		[backButton setEnabled:TRUE];
	}
	else if (alertView.tag == TJC_DETAIL_ALERT_PURCHASE_ERROR)
	{
		// The cancel button
		if (buttonIndex == 0)
		{
			// Re-enable the buy button.
			[buyButton setUserInteractionEnabled:YES];
			[buyButton setAlpha:1.0f];
			
			[self removeObserverForPurchasedItem];
		}
		else if (buttonIndex == 1)
		{
			// Retry a purchase.
			[self initiateItemPurchase];
		}
		
		// Re-enable back button no matter what at this point. We don't want to lock the user in this screen for any reason.
		[backButton setEnabled:TRUE];
	}
}


- (void)initiateItemPurchase
{
	[self removeObserverForPurchasedItem];
	
	// Check for sufficient funds.
	NSNumber *cachedPoints = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINTS_KEY_NAME];
	int currencyAmt = [cachedPoints intValue];
	
	// Throw an alert if there are insufficient funds.
	if (currencyAmt < [currentItemRef itemPrice])
	{
		NSString *fundsMsg = [NSString stringWithFormat:@"Insufficient %@", [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME]];
		
		UIAlertView *alertview = [[UIAlertView alloc] initWithTitle:@"" 
																			 message:fundsMsg
																			delegate:self 
																cancelButtonTitle:@"Cancel"
																otherButtonTitles:@"Get More", nil];	
		
		alertview.tag = TJC_DETAIL_ALERT_INSUFFICIENT_FUNDS_ID;
		
		[alertview show];
		[alertview release];
	}	
	else
	{
		// Initiate a purchase.
		[[TJCVGStoreManager sharedTJCVGStoreManager] buyVGStoreItemWithID:currentItemRef.itemID];
		
		// Add observers to monitor for purchase attempts.
		[self addObserversForPurchasedItem];
		
		[TJCVGView sharedTJCVGView].shouldReloadAvailableItemsPages = YES;
		[TJCVGView sharedTJCVGView].shouldReloadPurchasedItemsPages = YES;
		
		// Disable back button for now.
		[backButton setEnabled:FALSE];
	}
}


- (void)purchaseItemComplete:(id)sender
{
	// Check that a purchase was successful before downloading the item.
	TJCVGItemPurchaseResponse *vgRetObj = [[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] getBuyItemResponse];
	
	if(vgRetObj)
	{
		if(vgRetObj.success)
		{
			// Posting this notification will initiate an item download.
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_PURCHASE_SUCCESS_NOTIFICATION object:nil];
		}
		else
		{
			[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Item Purchase Failed: %@", vgRetObj.message];
			
			// Indicate that a refresh must occur.
			shouldRefreshOnDismiss = YES;
			
			// Re-enable back button, error occured.
			[backButton setEnabled:TRUE];
		}
	}
	
	// Remove observers since a purchase has been completed.
	[self removeObserverForPurchasedItem];
	
	// This will refresh all the items.
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForAllVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForAllVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForPurchasedVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForPurchasedVGStoreItems];
}


- (void)purchaseItemError
{
	UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
																		  message:@"Service is unreachable.\nDo you want to try again?"
																		 delegate:self 
															 cancelButtonTitle:@"Cancel"
															 otherButtonTitles:@"Retry", nil];
	
	alertview.tag = TJC_DETAIL_ALERT_PURCHASE_ERROR;
	
	[alertview show];
	[alertview release];
}


- (void)initiateItemDownload:(id)sender
{	
	TJCFileDownloadHandler *fileDownloadHandler = [TJCVGViewHandler sharedTJCVGViewHandler].fileDownloadHandler;
	
	[fileDownloadHandler downloadItem:currentItemRef];
	
	UIView *indic = [fileDownloadHandler getVGPurchasedItemDownloadProgressView:currentItemRef.itemID];
	
	//[indic retain];
	
	if (indic)
	{
		// Place the download progress view right below the type label.
		[indic setFrame:CGRectMake(
											typeLabel.frame.origin.x,
											typeLabel.frame.origin.y + typeLabel.frame.size.height,
											indic.frame.size.width, 
											indic.frame.size.height)];
		[itemHeaderSection addSubview:indic];
	}
	
	// Indicate that a refresh must occur.
	shouldRefreshOnDismiss = YES;
	
	// Re-enable back button after item download has completed.
	[backButton setEnabled:TRUE];
}


//- (void)itemDownloadComplete
//{
//
//}


- (void)dealloc
{
	[navBar release];
	[backButton release];
	[mainView release];
	[itemHeaderSection release];
	[iconImage release];
	[buyButton release];
	[costLabel release];
	[nameLabel release];
	[typeLabel release];
	[itemDescriptionSection release];
	[itemDescriptionSectionBG release];
	[descItemAttributes release];
	[descItemDescription release];
	[descCostLabel release];
	[descNameLabel release];
	[descTypeLabel release];
	[currentItemRef release];
	[itemButtonRef release];
	[super dealloc];
}

@end
