// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGPageView.h"
#import "TJCVGStoreModel.h"
#import "TJCVGUtil.h"
#import "TJCVGItemButton.h"
#import "TJCVGItemButtonView.h"
#import "TJCVGConstants.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCVGView.h"
#import "TJCLog.h"


static float TJCVG_BUTTON_WIDTH = 0;
static float TJCVG_BUTTON_HEIGHT = 0;

@implementation TJCVGPageView

@synthesize pageView, buttons;


// Load the view nib and initialize the pageNumber ivar.
- (id)initWithPageNumber:(int)page withFrame:(CGRect)frame withData:(NSArray*)data
{
	if ((self = [super init]))
	{
		// No longer loaded from a nib, alloc manually here.
		pageView = [[UIView alloc] init];
		
		// The page number will also help determine button placement.
        pageNumber = page;

		TJCVG_BUTTON_WIDTH = frame.size.width * TJCVG_BUTTON_W_SIZE_PERCENTAGE;
		TJCVG_BUTTON_HEIGHT = frame.size.height * TJCVG_BUTTON_H_SIZE_PERCENTAGE;
		CGRect buttonFrame = CGRectMake(0, 
										0, 
										frame.size.width * TJCVG_BUTTON_W_SIZE_PERCENTAGE, 
										frame.size.height * TJCVG_BUTTON_H_SIZE_PERCENTAGE);
		
		// Init the array of buttons.
		buttons = [[NSMutableArray alloc] init];
		int curX = 0;
		int curY = 0;		
		for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
		{
			// The way the buttons are split up and positioned, we need to calculate the center location for every other section.
			// Hence, the multiplied by 2 plus 1 math.
			curX = ((i % TJCVG_NUM_BUTTONS_ROW) * 2) + 1;
			curY = ((i / TJCVG_NUM_BUTTONS_ROW) * 2) + 1;

			// Load the button from the nib. Note that it isn't actually a button but a custom uiview.
			TJCVGItemButton *tmpButton = [[TJCVGItemButton alloc] init];
			
			// Calculate the button location.
			//tmpButton.view.frame = buttonFrame;
			[[tmpButton itemButton] reformatViewWithFrame:buttonFrame];

			tmpButton.itemButton.center = CGPointMake((frame.size.width / 4) * curX,
										(frame.size.height / 4) * curY);
			
			tmpButton.itemButton.alpha = 0;
			
			[buttons addObject: tmpButton];
			
			[pageView addSubview:tmpButton.itemButton];
			
			[tmpButton release];
		}

		//pageView.backgroundColor = [UIColor lightGrayColor];
		
		// Load the virtual goods.
		int vgCount = [data count];
		int vg_counter = page * TJCVG_NUM_BUTTONS;	// Each page contains TJCVG_NUM_BUTTONS, so set the button counter to the current page.
		for (int j = 0; j < TJCVG_NUM_BUTTONS; ++j)
		{
			// If we go over the virtual goods count, we need to exit the loop.
			if (vg_counter >= vgCount)
			{
				break;
			}
			
			// Initialize the button, which will initiate a fade-in animation for that button.
			// Important to note that we're pulling the proper object out of the items array.
			TJCVGStoreItem *VGStoreItem = [data objectAtIndex:j + (page * TJCVG_NUM_BUTTONS)];
			
			//NSMutableArray *itemData = [[[NSMutableArray alloc] init] autorelease];
			//[itemData addObject:[NSNumber numberWithInt:j]];
			//[itemData addObject:VGStoreItem];
			// Invoke the init method in a background thread to prevent interruption with current loading.
			//[self performSelectorInBackground:@selector(initButtonWithData:) withObject:itemData];
			[self initButtonWithID:j withDetail:VGStoreItem];
			[self fadeInButtonWithIndex:j];
			
			vg_counter++;
		}
    }
	
    return self;
}


- (void)reformatButtonsWithFrame:(CGRect)frame
{
	TJCVG_BUTTON_WIDTH = frame.size.width * TJCVG_BUTTON_W_SIZE_PERCENTAGE;
	TJCVG_BUTTON_HEIGHT = frame.size.height * TJCVG_BUTTON_H_SIZE_PERCENTAGE;
	CGRect buttonFrame = CGRectMake(0, 
									0, 
									frame.size.width * TJCVG_BUTTON_W_SIZE_PERCENTAGE, 
									frame.size.height * TJCVG_BUTTON_H_SIZE_PERCENTAGE);
	
	int curX = 0;
	int curY = 0;		
	for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
	{
		// The way the buttons are split up and positioned, we need to calculate the center location for every other section.
		// Hence, the multiplied by 2 plus 1 math.
		curX = ((i % TJCVG_NUM_BUTTONS_ROW) * 2) + 1;
		curY = ((i / TJCVG_NUM_BUTTONS_ROW) * 2) + 1;
		
		// Load the button from the nib. Note that it isn't actually a button but a custom uiview.
		TJCVGItemButton *tmpButton = [buttons objectAtIndex:i];
		
		// Calculate the button location.
		//tmpButton.view.frame = buttonFrame;
		[[tmpButton itemButton] reformatViewWithFrame:buttonFrame];
		
		tmpButton.itemButton.center = CGPointMake((frame.size.width / 4) * curX,
											(frame.size.height / 4) * curY);
	}
}


//- (void)fadeInButtonWithData:(NSArray*)data
//{
//	int buttonID = [[data objectAtIndex:0] integerValue];
//	
//	// The positions for the buttons are already set in the init method.
//	// We only need to fill in the description and thumbnail image here.
//	TJCVGItemButton *tmpButton = [buttons objectAtIndex:buttonID];
//	
//	// Show it with a transition effect
//	[UIView beginAnimations:nil context:nil];
//	// animation duration in seconds
//	[UIView setAnimationDuration: (float)0.5];
//	
//	// Make it visible.
//	tmpButton.itemButton.alpha = 1.0f;
//	
//	// Start the animation.
//	[UIView commitAnimations];
//	
//	// Fire notification that downloading has finished. This will increment the item counter in TJCVGView so it knows when to remove the loading screen.
//	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_DOWNLOAD_FINISHED_FOR_LOADING_NOTIFICATION
//														object:nil];
//}


- (void)fadeInButtonWithIndex:(int)index
{
	// The positions for the buttons are already set in the init method.
	// We only need to fill in the description and thumbnail image here.
	TJCVGItemButton *tmpButton = [buttons objectAtIndex:index];
	
	// Show it with a transition effect
	[UIView beginAnimations:nil context:nil];
	// animation duration in seconds
	[UIView setAnimationDuration: (float)0.5];
	
	// Make it visible.
	tmpButton.itemButton.alpha = 1.0f;
	
	// Start the animation.
	[UIView commitAnimations];
}


- (void)initButtonWithID:(int)buttonID withDetail:(TJCVGStoreItem*)details
{
	// The positions for the buttons are already set in the init method.
	// We only need to fill in the description and thumbnail image here.
	TJCVGItemButton *tmpButton = [buttons objectAtIndex:buttonID];
	
	// Store the item data itself in case we need access to it's contents later.
	tmpButton.storeItemRef = details;

	// Initiate request to download the thumbnail image.
	[[tmpButton itemButton] requestThumbImageWithURL:details.itemThumbImageURL];
	
	// Populate the labels of the item.
	if (details.itemName)
	{
		[tmpButton itemButton].itemNameLabel.text = [NSString stringWithFormat:@"Name: %@", details.itemName];
	}
	
	if (details.itemTypeName)
	{
		[tmpButton itemButton].itemTypeLabel.text = [NSString stringWithFormat:@"Type: %@", details.itemTypeName];
	}
	
	// Accessing global vgView class, kinda yucky but very convenient!
	// Depending on which view is currently active, we will either display the cost or the download status.
	if ([[TJCVGView sharedTJCVGView] getCurrentActiveView] == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		[tmpButton itemButton].itemCostLabel.text = @"Downloaded";
	}
	else
	{
		if (details.itemCurrencyName)
		{
			[tmpButton itemButton].itemCostLabel.text = [NSString stringWithFormat:@"Cost: %d %@", details.itemPrice, details.itemCurrencyName];
		}
	}	
}


- (void)fadeOutViewWithDuration:(float)duration
{
	pageView.alpha = 1.0f;
		
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:duration];
	
	pageView.alpha = 0.0f;
	
	[UIView commitAnimations];	
}


- (void)fadeInViewWithDuration:(float)duration
{
	pageView.alpha = 0.0f;
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:duration];
	
	pageView.alpha = 1.0f;
	
	[UIView commitAnimations];	
}


- (void)fadeOutButtonsWithDuration:(float)duration
{
	for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
	{
		TJCVGItemButton *tmpButton = [buttons objectAtIndex:i];
		tmpButton.itemButton.alpha = 1.0f;
	}
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:duration];
	
	for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
	{
		TJCVGItemButton *tmpButton = [buttons objectAtIndex:i];
		tmpButton.itemButton.alpha = 0.0f;
	}
	
	[UIView commitAnimations];	
}


- (void)fadeInButtonsWithDuration:(float)duration
{
	for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
	{
		TJCVGItemButton *tmpButton = [buttons objectAtIndex:i];
		tmpButton.itemButton.alpha = 0.0f;
	}
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:duration];
	
	for (int i = 0; i < TJCVG_NUM_BUTTONS; ++i)
	{
		TJCVGItemButton *tmpButton = [buttons objectAtIndex:i];
		tmpButton.itemButton.alpha = 1.0f;
	}
	
	[UIView commitAnimations];	
}


- (void)dealloc 
{	
	[buttons release];
	
    [super dealloc];
}

@end
