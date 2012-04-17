// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVGView.h"
#import "TJCVGPageView.h"
#import "TJCViewCommons.h"
#import "TJCVGViewHandler.h"
#import "TJCVGStoreManager.h"
#import "TJCVGStoreModel.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCUserAccountModel.h"
#import "TJCConstants.h"
#import "TJCVGConstants.h"
#import "TJCOffersViewHandler.h"
#import "TJCUserAccountManager.h"
#import "TJCLog.h"
#import "TJCVGUtil.h"
#import "TJCVGTableViewCell.h"
#import "TJCVGItemButton.h"
#import "TJCVGItemButtonView.h"
#import "TJCVGItemDetail.h"
#import "SynthesizeSingleton.h"


@implementation TJCVGView

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCVGView)

static NSString *TJCVG_NAV_BAR_TITLE_AVAILABLE = @"Available Items";
static NSString *TJCVG_NAV_BAR_TITLE_PURCHASED = @"Purchased Items";
static NSString *TJCVG_VIEWING_STR = @"Viewing";
static NSString *TJCVG_ALL_ITEMS_STR = @"Available";
static NSString *TJCVG_PURCHASED_ITEMS_STR = @"Purchased";
static NSString *TJCVG_CURRENCY_STR = @"Currency";

static NSString *TJCVG_NO_ITEMS_MSG = @"No Items Available";
static const int TJCVG_ALL_ITEMS_ALERT_ID = 100;
static const int TJCVG_PURCHASED_ITEMS_ALERT_ID = 101;


@synthesize mainView, pageCtrl, navBar, navBarButton, scrollView, tableView, toolBarLeft, toolBarRight,
itemSelectorButton, viewModeSelectorButton, earnMoreButton, loadingTextLabel, genericLabel, loadingView, 
genericIndicator, itemTypeLabel, currencyLabel, shouldReloadAvailableItemsPages, shouldReloadPurchasedItemsPages;



- (void)addObserversForAllVGStoreItems 
{
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(recievedAllVGItems) 
																name:SUBMIT_ALL_VG_STORE_ITEMS_RECIEVE_DATA 
															 object:nil]; 
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(VGAllItemsNetError) 
																name:SUBMIT_ALL_VG_STORE_ITEMS_ERROR_RECIEVE_DATA 
															 object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(VGAllItemsNetError) 
																name:SUBMIT_ALL_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA 
															 object:nil];
}


- (void)removeObserversForAllVGStoreItems 
{
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_ALL_VG_STORE_ITEMS_RECIEVE_DATA object:nil ];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_ALL_VG_STORE_ITEMS_ERROR_RECIEVE_DATA object:nil ];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_ALL_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil ];
	
}


- (void)addObserversForPurchasedVGStoreItems 
{
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(recievedPurchasedVGItems) 
																name:SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA 
															 object:nil]; 
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(VGPurchasedItemsNetError) 
																name:SUBMIT_PURCHASED_VG_STORE_ITEMS_ERROR_RECIEVE_DATA 
															 object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(VGPurchasedItemsNetError) 
																name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA 
															 object:nil];
}


- (void)removeObserversForPurchasedVGStoreItems
{
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA object:nil ];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_ERROR_RECIEVE_DATA object:nil ];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil ];	
}


- (void)addObserversForLoadingItems
{
	// This will effectively fade in the scroll view with all the available items and fade out the loading views.
	[[NSNotificationCenter defaultCenter] addObserver:self
														  selector:@selector(currentViewModeTransitionInAnimate)
																name:TJC_VG_DISMISS_LOADING_VIEW
															 object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
														  selector:@selector(refreshScrollView)
																name:TJC_VG_DISMISS_LOADING_VIEW
															 object:nil];
}


- (void)removeObserversForLoadingItems
{
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_VG_DISMISS_LOADING_VIEW object:nil ];
}


- (void)refreshWithFrame:(CGRect)frame
{
	// This is a bit ambiguous. The index 1 represents the 2nd vg view.
	mainView = [[[NSBundle mainBundle] loadNibNamed:@"TJCVGView" owner:self options:nil] objectAtIndex:0];
	mainView.frame = frame;
	
	[self initiateVGItemsDownload];
	
	// Start on the available items view.
	currentActiveView = TJCVG_ALL_ITEMS_VIEW;
	
	// Start on the table view.
	currentViewStyle = TJCVG_TABLEVIEW_MODE;
	
	// The reason these views are not visible is because if there is no internet connection, we don't want to display old data.
	// One of these views will be displayed when data is retrieved in the loadItemsFromArray method.
	[tableView setAlpha:0.0f];
	[scrollView setAlpha:0.0f];
	[pageCtrl setAlpha:0.0f];
	
	mainView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	
	pageCtrl.numberOfPages = 1;
	pageCtrl.currentPage = 0;
	
	// This needs to be set immediately so we don't see the placeholder text.
	[itemTypeLabel setText:TJCVG_VIEWING_STR];
	[currencyLabel setText:TJCVG_CURRENCY_STR];
	
	scrollView.pagingEnabled = YES;
	// A page is the width of the scroll view
	scrollView.contentSize = CGSizeMake(scrollView.frame.size.width * numPages, scrollView.frame.size.height);
	scrollView.showsHorizontalScrollIndicator = NO;
	scrollView.showsVerticalScrollIndicator = NO;
	scrollView.scrollsToTop = NO;
	//scrollView.delegate = self;
	
	// Set up the loading views.
	loadingView.backgroundColor = [UIColor blackColor];
	loadingView.alpha = 0;
	genericIndicator.alpha = 0;
	loadingTextLabel.alpha = 0;
	
	[self showLoadingViews];
	
	[self addObserversForLoadingItems];
	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updatePointsView) name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];
	
	// Set color to user defined color.
	[self setViewColor:[[TJCViewCommons sharedTJCViewCommons] getUserDefinedColor]];
	
	// Disable certain buttons until loading is finished.
	[self disableButtons];
	
	areAvailableVGItemsAvailable = NO;
	arePurchasedVGItemsAvailable = NO;
	shouldReloadAvailableItemsPages = YES;
	shouldReloadPurchasedItemsPages = YES;
}


- (void)updatePointsView
{
	// The top right label shows how much currency we have.
	NSString *cachedCurrencyName = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME];
	NSNumber *cachedPoints = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINTS_KEY_NAME];
	[currencyLabel setText:[NSString stringWithFormat:@"%@: %d", cachedCurrencyName, [cachedPoints intValue]]];
}


-(void)VGAllItemsNetError
{
	if([genericIndicator isAnimating])
	{
		[genericIndicator stopAnimating];
	}
	
	UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
																		  message:@"Service is unreachable.\nDo you want to try again?" 
																		 delegate:self 
															 cancelButtonTitle:@"Cancel" 
															 otherButtonTitles:@"Retry", nil];
	alertview.tag = TJCVG_ALL_ITEMS_ALERT_ID;
	[alertview show];
	[alertview release];
}


-(void)VGPurchasedItemsNetError
{
	if([genericIndicator isAnimating])
	{
		[genericIndicator stopAnimating];
	}
	
	UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:@"" 
																		  message:@"Service is unreachable.\nDo you want to try again?" 
																		 delegate:self 
															 cancelButtonTitle:@"Cancel" 
															 otherButtonTitles:@"Retry", nil];
	alertview.tag = TJCVG_PURCHASED_ITEMS_ALERT_ID;
	[alertview show];
	[alertview release];
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{	
	// Handle the all items error alert.
	if(alertView.tag == TJCVG_ALL_ITEMS_ALERT_ID)
	{
		if(buttonIndex == 1)
		{
			// The retry button initiates another load attempt.
			[genericIndicator startAnimating];
			[self initiateVGItemsDownload];
			[self hideGenericLabel];
		}
		else
		{
			// The cancel button has been pressed...
			[genericIndicator stopAnimating];
			// Show the no available items message.
			[self showGenericLabel:TJCVG_NO_ITEMS_MSG];
			[self hideLoadingViews];
			
			// Get rid of all cached items so that toggling between item views will not show old data.
			[[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems] removeAllObjects];
			[[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems] removeAllObjects];
		}
	}
	
	// Handle the purchased items error alert.
	if(alertView.tag == TJCVG_PURCHASED_ITEMS_ALERT_ID)
	{
		if(buttonIndex == 1)
		{
			// The retry button initiates another load attempt.
			[genericIndicator startAnimating];
			[self initiatePurchasedItemsVGDownload];
			[self hideGenericLabel];
		}
		else
		{
			// The cancel button has been pressed...
			[genericIndicator stopAnimating];
			// Show the no available items message.
			[self showGenericLabel:TJCVG_NO_ITEMS_MSG];
			[self hideLoadingViews];
			
			// Get rid of all cached items so that toggling between item views will not show old data.
			[[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems] removeAllObjects];
			[[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems] removeAllObjects];
			
		}
	}
}


-(void)dealloc 
{
	[mainView release];
	[pageCtrl release];
	[navBar release];
	[navBarButton release];
	[scrollView release];
	[tableView release];
	[toolBarLeft release];
	[toolBarRight release];
	[itemSelectorButton release];
	[viewModeSelectorButton release];
	[earnMoreButton release];
	[loadingView release];
	[genericIndicator release];
	[genericLabel release];
	[loadingTextLabel release];
	[itemTypeLabel release];
	[currencyLabel release];
	[availableItemsPages release];
	[purchasedItemsPages release];
	[detailView release];
	
	[super dealloc];
}


#pragma mark Data Related Methods


- (void)initiateVGItemsDownload
{
	// Notice we only add observers for all VG store item requests. This is because the notifications for purhcased items are the same.
	[self removeObserversForAllVGStoreItems];
	
	// Add observers for receiving item notifications.
	[self addObserversForAllVGStoreItems];
	
	// Initiate web request for virtual goods.
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForAllVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForAllVGStoreItems];
	
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForPurchasedVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForPurchasedVGStoreItems];
}


-(void)initiateAllItemsVGDownload
{
	[self removeObserversForAllVGStoreItems];
	
	// Add observers for receiving item notifications.
	[self addObserversForAllVGStoreItems];
	
	// Initiate web request for virtual goods.
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForAllVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForAllVGStoreItems];
}


-(void)initiatePurchasedItemsVGDownload
{
	[self removeObserversForPurchasedVGStoreItems];
	
	// Add observers for receiving item notifications.
	[self addObserversForPurchasedVGStoreItems];
	
	// Initiate web request for virtual goods.
	[[TJCVGStoreManager sharedTJCVGStoreManager] reloadStartForPurchasedVGStoreItems];
	[[TJCVGStoreManager sharedTJCVGStoreManager] initiateRequestForPurchasedVGStoreItems];
}


-(void)recievedAllVGItems
{
	// Net request has finished, remove the observers.
	[self removeObserversForAllVGStoreItems];
	
	// This is a bit of a strange check. This is basically only used to initialization to make sure that the view set is the view we want to see.
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		[self loadItemsFromArray:[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems]];
	}
}


-(void)recievedPurchasedVGItems
{
	// Net request has finished, remove the observers.
	[self removeObserversForPurchasedVGStoreItems];
	
	// This is a bit of a strange check. This is basically only used to initialization to make sure that the view set is the view we want to see.
	if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		[self loadItemsFromArray:[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems]];
	}
}


- (IBAction)toggleItemViewAction
{	
	BOOL shouldShowViews = NO;
	
	if (itemSelectorButton.selectedSegmentIndex == 0)
	{
		// All button selected.
		[self loadAllItemsAction];
	}
	else
	{
		// Purchased button selected.
		[self loadPurchasedItemsAction];
	}
	
	// This portion of the code depends on the methods above. Don't move it.
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		if (areAvailableVGItemsAvailable)
		{
			shouldShowViews = YES;
		}
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		if (arePurchasedVGItemsAvailable)
		{
			shouldShowViews = YES;
		}
	}
	
	if (shouldShowViews)
	{
		if (currentViewStyle == TJCVG_TABLEVIEW_MODE)		
		{
			// Show the table view.
			[tableView setAlpha:1.0f];
			[scrollView setAlpha:0.0f];
			[pageCtrl setAlpha:0.0f];
		}
		else if (currentViewStyle == TJCVG_SCROLLVIEW_MODE)
		{
			// Show the table view.
			[tableView setAlpha:0.0f];
			[scrollView setAlpha:1.0f];
			[pageCtrl setAlpha:1.0f];
		}
		
		// Just in case any views didn't recieve this message (such as if the alpha was zero when orientation changed).
		[self updateViewWithOrientation:lastOrientation];
	}
	else
	{
		// Get rid of views if there are no items available.
		[tableView setAlpha:0.0f];
		[scrollView setAlpha:0.0f];
		[pageCtrl setAlpha:0.0f];
	}	
}


- (void)loadPurchasedItemsAction
{
	// Don't reload views if we're already on the desired one.
	if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		return;
	}
	
	// Re-add the observers for loading items.
	[self addObserversForLoadingItems];
	
	currentActiveView = TJCVG_PURCHASED_ITEMS_VIEW;
	
	[self showLoadingViews];
	
	// Change nav bar title.
	[[navBar topItem] setTitle:TJCVG_NAV_BAR_TITLE_PURCHASED];
	
	[self loadItemsFromArray:[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems]];
}


- (void)loadAllItemsAction
{
	// Don't reload views if we're already on the desired one.
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		return;
	}
	
	// Re-add the observers for loading items.
	[self addObserversForLoadingItems];
	
	currentActiveView = TJCVG_ALL_ITEMS_VIEW;
	
	[self showLoadingViews];
	
	// Change nav bar title.
	[[navBar topItem] setTitle:TJCVG_NAV_BAR_TITLE_AVAILABLE];
	
	[self loadItemsFromArray:[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems]];
}


- (void)refreshItems
{
	// Both purchased and available items are refreshed. The view is resolved but currentActiveView which is done in
	[self initiateVGItemsDownload];
	
	[self showLoadingViews];
	
	[self addObserversForLoadingItems];
}


- (void)loadItemsFromArray:(NSArray*)itemsArray
{
	// Remove all previous visible items.
	[self currentViewModeTransitionOutAnimate];
	
	NSMutableArray *itemsPageArrayRef = nil;
	
	int vgCount = [itemsArray count];
	
	// Save number of pages, with a certain number of buttons per page, each button representing a virtual good.
	numPages = (int)ceil((float)vgCount / TJCVG_NUM_BUTTONS);
	
	// Clear all sub views in the scroll view.
	for (UIView *subView in scrollView.subviews)
	{
		[subView removeFromSuperview];
	}
	
	// Update the content size of the scroll view.
	scrollView.contentSize = CGSizeMake(scrollView.frame.size.width * numPages, scrollView.frame.size.height);
	
	// Update the current page and the number of pages.
	pageCtrl.currentPage = 0;
	
	// Setting page control to zero produces undefined behavior. Make sure it's at least 1.
	if (numPages <= 0)
	{
		pageCtrl.numberOfPages = 1;
	}
	else
	{
		pageCtrl.numberOfPages = numPages;	
	}
	
	// See if the data for the item pages needs to be reloaded.
	BOOL shouldReloadData = NO;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		if (shouldReloadAvailableItemsPages)
		{
			shouldReloadData = YES;
		}
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		if (shouldReloadPurchasedItemsPages)
		{
			shouldReloadData = YES;
		}
	}
	
	if (shouldReloadData)
	{		
		if (vgCount > 0)
		{
			// Initialize all the pages with the item data.
			itemsPageArrayRef = [[NSMutableArray alloc] init];
			for (int i = 0; i < numPages; ++i)
			{
				TJCVGPageView *page = [[TJCVGPageView alloc] initWithPageNumber:i 
																						withFrame:[self getScrollViewFrame]
																						 withData:itemsArray];
				
				[itemsPageArrayRef addObject:page];
				
				[page release];
			}
		}
		
		if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
		{
			// Clear any previous page data.
			if (availableItemsPages)
			{
				[availableItemsPages removeAllObjects];
				[availableItemsPages release];
				availableItemsPages = nil;
			}
			
			availableItemsPages = [[NSMutableArray alloc] initWithArray:itemsPageArrayRef];
		}
		else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
		{
			// Clear any previous page data.
			if (purchasedItemsPages)
			{
				[purchasedItemsPages removeAllObjects];
				[purchasedItemsPages release];
				purchasedItemsPages = nil;
			}
			
			purchasedItemsPages = [[NSMutableArray alloc] initWithArray:itemsPageArrayRef];
		}
	}
	
	// Update the labels near the top of the view.
	// Left label describes what view is currently active.
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		[itemTypeLabel setText:[NSString stringWithFormat:@"%@: %@", TJCVG_VIEWING_STR, TJCVG_ALL_ITEMS_STR]];
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		[itemTypeLabel setText:[NSString stringWithFormat:@"%@: %@", TJCVG_VIEWING_STR, TJCVG_PURCHASED_ITEMS_STR]];
	}
	// Second label shows how much currency we have.
	NSString *cachedCurrencyName = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_CURRENCY_KEY_NAME];
	NSNumber *cachedPoints = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_POINTS_KEY_NAME];
	[currencyLabel setText:[NSString stringWithFormat:@"%@: %d", cachedCurrencyName, [cachedPoints intValue]]];
	
	// Set the currency name for the offers button.
	[earnMoreButton setTitle:[NSString stringWithFormat:@"Earn %@", cachedCurrencyName]];
	
	if (vgCount <= 0)
	{
		if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
		{
			areAvailableVGItemsAvailable = NO;
		}
		else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
		{
			arePurchasedVGItemsAvailable = NO;
		}		
		
		// Display a message if there are no purchased items.
		[self showGenericLabel:TJCVG_NO_ITEMS_MSG];
		[self hideLoadingViews];
		
		// Loading has finished, re-enable buttons.
		[self enableButtons];
		
		// Disable the view style button.
		[self disableViewStyleButton];
		
		// Disable item views.
		[self hideTableView];
		[self hideScrollView];
	}
	else 
	{
		if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
		{
			areAvailableVGItemsAvailable = YES;
		}
		else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
		{
			arePurchasedVGItemsAvailable = YES;
		}		
		
		// There are items to load, hide the no items label and certain buttons.
		[self hideGenericLabel];
		
		//[self disableButtons];
		[self enableButtons];
		
		[tableView reloadData];
	}
	
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		shouldReloadAvailableItemsPages = NO;
		
		if (areAvailableVGItemsAvailable)
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_DISMISS_LOADING_VIEW object:nil];
		}
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		shouldReloadPurchasedItemsPages = NO;
		
		if (arePurchasedVGItemsAvailable)
		{
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_DISMISS_LOADING_VIEW object:nil];
		}
	}
	
	[itemsPageArrayRef release];
	
	[self removeObserversForLoadingItems];
}




#pragma mark View Related Methods


- (void)disableButtons
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[itemSelectorButton setAlpha:0.0f];
	[viewModeSelectorButton setAlpha:0.0f];
	
	[UIView commitAnimations];
}


- (void)disableViewStyleButton
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[viewModeSelectorButton setAlpha:0.0f];
	
	[UIView commitAnimations];
}


- (void)enableButtons
{	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[itemSelectorButton setAlpha:1.0f];
	[viewModeSelectorButton setAlpha:1.0f];
	
	[UIView commitAnimations];
}


- (void)hideTableView
{
	[tableView setAlpha:0.0f];
}


- (void)hideScrollView
{
	[scrollView setAlpha:0.0f];
	[pageCtrl setAlpha:0.0f];
}


- (IBAction)toggleViewMode
{
	BOOL shouldShowView = NO;
	
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		if (areAvailableVGItemsAvailable)
		{
			shouldShowView = YES;
		}
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		if (arePurchasedVGItemsAvailable)
		{
			shouldShowView = YES;
		}
	}
	
	if (viewModeSelectorButton.selectedSegmentIndex == 0)
	{
		currentViewStyle = TJCVG_TABLEVIEW_MODE;
		
		if (shouldShowView)
		{
			// Show the table view.
			[tableView setAlpha:0.0f];
			[scrollView setAlpha:1.0f];
			[pageCtrl setAlpha:1.0f];
			
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ANIM_DURATION];
			
			[tableView setAlpha:1.0f];
			[scrollView setAlpha:0.0f];
			[pageCtrl setAlpha:0.0f];
			
			[UIView commitAnimations];
		}
	}
	else
	{
		currentViewStyle = TJCVG_SCROLLVIEW_MODE;
		
		if (shouldShowView)
		{
			// Show the scroll view.
			[tableView setAlpha:1.0f];
			[scrollView setAlpha:0.0f];
			[pageCtrl setAlpha:0.0f];
			
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ANIM_DURATION];
			
			[tableView setAlpha:0.0f];
			[scrollView setAlpha:1.0f];
			[pageCtrl setAlpha:1.0f];
			
			[UIView commitAnimations];
		}
	}
}


- (void)setViewColor:(UIColor*)color
{
	//[mainView setBackgroundColor:color];
	
	[navBar setTintColor:color];
	
	//[navBarButton setColor:color];
	
	[scrollView setBackgroundColor:[TJCVGUtil getUserDefinedColorWithWhiteValue:TJCVG_COLOR_LIGHTEN_VALUE]];
	
	[toolBarLeft setTintColor:color];
	[toolBarRight setTintColor:color];
	
	[itemSelectorButton setBackgroundColor:color];
	
	//[earnMoreButton setColor:color];
	
	[itemTypeLabel setBackgroundColor:color];
	[currencyLabel setBackgroundColor:color];
	[bgLabel setBackgroundColor:color];
}


- (eTJCVGActiveItemsView)getCurrentActiveView
{
	return currentActiveView;
}


-(void)showGenericLabel:(NSString*)msg
{	
	// Generic labels should never show up with items, so make sure to hide all items here.
	[self hideTableView];
	[self hideScrollView];
	[self disableViewStyleButton];
	
	[genericLabel setText:msg];
	genericLabel.alpha = 1;
}


-(void)hideGenericLabel
{
	genericLabel.alpha = 0;
}


- (IBAction)showOffersView
{
	UIView *offersWebView = [TJCOffersViewHandler showOffersWithFrame:[mainView bounds]];
	[mainView addSubview:offersWebView];
}


-(void)showLoadingViews
{	
	loadingView.alpha = 0.0f;
	genericIndicator.alpha = 0.0f;
	[genericIndicator startAnimating];
	loadingTextLabel.alpha = 0.0f;
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ANIM_DURATION];
	
	// Let the loading view be transparent during the loading animation, it looks better.
	loadingView.alpha = TJCVG_LOADING_VIEW_ALPHA;
	genericIndicator.alpha = 1.0f;
	loadingTextLabel.alpha = 1.0f;
	
	[UIView commitAnimations];	
}


-(void)hideLoadingViews
{	
	loadingView.alpha = TJCVG_LOADING_VIEW_ALPHA;
	genericIndicator.alpha = 1.0f;
	loadingTextLabel.alpha = 1.0f;
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ANIM_DURATION];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(hideLoadingViewsEnded:finished:context:)];
	
	loadingView.alpha = 0.0f;
	genericIndicator.alpha = 0.0f;
	loadingTextLabel.alpha = 0.0f;
	
	[UIView commitAnimations];	
}


- (void)hideLoadingViewsEnded:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	[genericIndicator stopAnimating];
}


- (void)updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	lastOrientation = fromInterfaceOrientation;

	scrollView.contentSize = CGSizeMake(scrollView.frame.size.width * numPages, [self getScrollViewFrame].size.height);
	
	// Manually execute a page change to make sure that we aren't halfway on a page after an orientation change.
	[self changePage:self];
	
	NSMutableArray *itemsPageArrayRef = nil;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsPageArrayRef = availableItemsPages;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsPageArrayRef = purchasedItemsPages;
	}
	
	for (int i = 0; i < numPages; ++i)
	{
		TJCVGPageView *pageView = [itemsPageArrayRef objectAtIndex:i];
		[pageView reformatButtonsWithFrame:[self getScrollViewFrame]];
	}
}


- (void)currentViewModeTransitionOutAnimate
{
	if (currentViewStyle == TJCVG_TABLEVIEW_MODE)
	{
		[self tableViewTransitionOutAnimate];
	}
	else if (currentViewStyle == TJCVG_SCROLLVIEW_MODE)
	{
		[self scrollViewTransitionOutAnimate];
	}
}


- (void)currentViewModeTransitionInAnimate
{
	if (currentViewStyle == TJCVG_TABLEVIEW_MODE)
	{
		[self tableViewTransitionInAnimate];
	}
	else if (currentViewStyle == TJCVG_SCROLLVIEW_MODE)
	{
		[self scrollViewTransitionInAnimate];
	}
	
	[self hideLoadingViews];
}


-(IBAction)dismissView
{
	[TJCViewCommons animateTJCView:mainView 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getReverseTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
	// This ensures that the tap points notification doesn't get fired repeatedly when opening and closing the view.
	[[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];
	
	// After the animation, remove the view from the window.
	[self performSelector:@selector(removeView) withObject:nil afterDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
}


-(void)removeView
{
	[mainView removeFromSuperview];
	
	// This will execute a method call that (effectively) removes this view from the super view.
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VIEW_CLOSED_NOTIFICATION object:nil];
}





#pragma mark UIScrollView implementation


-(void)scrollViewDidScroll:(UIScrollView *)sender 
{
	// We don't want a "feedback loop" between the UIPageControl and the scroll delegate in
	// which a scroll event generated from the user hitting the page control triggers updates from
	// the delegate method. We use a boolean to disable the delegate logic when the page control is used.
	if (pageControlUsed) 
	{
		// do nothing - the scroll was initiated from the page control, not the user dragging
		return;
	}
	
	// Switch the indicator when more than 50% of the previous/next page is visible
	CGFloat pageWidth = scrollView.frame.size.width;
	int page = (int)floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
	pageCtrl.currentPage = page;
	
	// A possible optimization would be to unload the views+controllers which are no longer visible
}


// At the begin of scroll dragging, reset the boolean used when scrolls originate from the UIPageControl
-(void)scrollViewWillBeginDragging:(UIScrollView *)sender
{
	pageControlUsed = NO;
	
	// Switch the indicator when more than 50% of the previous/next page is visible
	CGFloat pageWidth = scrollView.frame.size.width;
	int page = (int)floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
	
	// load the visible page and the page on either side of it (to avoid flashes when the user starts scrolling)
	[self loadScrollViewWithPage:page - 1];
	[self loadScrollViewWithPage:page];
	[self loadScrollViewWithPage:page + 1];
}


// At the end of scroll animation, reset the boolean used when scrolls originate from the UIPageControl
-(void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView 
{
	pageControlUsed = NO;
}


-(IBAction)changePage:(id)sender 
{
	int page = pageCtrl.currentPage;
	
	// load the visible page and the page on either side of it (to avoid flashes when the user starts scrolling)
	[self loadScrollViewWithPage:page - 1];
	[self loadScrollViewWithPage:page];
	[self loadScrollViewWithPage:page + 1];
	
	// update the scroll view to the appropriate page
	CGRect frame = scrollView.frame;
	frame.origin.x = frame.size.width * page;
	frame.origin.y = 0;
	[scrollView scrollRectToVisible:frame animated:YES];
	
	// Set the boolean used when scrolls originate from the UIPageControl. See scrollViewDidScroll: above.
	pageControlUsed = YES;
}


-(void)loadScrollViewWithPage:(int)page 
{
	if (page < 0) 
	{
		return;
	}
	
	if (page >= numPages) 
	{
		return;
	}
	
	NSMutableArray *itemsPageArrayRef;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsPageArrayRef = availableItemsPages;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsPageArrayRef = purchasedItemsPages;
	}
	
	if ([itemsPageArrayRef count] <= 0)
	{
		return;
	}
	
	TJCVGPageView *pageView = [itemsPageArrayRef objectAtIndex:page];
	
	if (pageView)
	{
		CGRect frame = scrollView.frame;
		frame.origin.x = frame.size.width * page;
		frame.origin.y = 0;
		pageView.pageView.frame = frame;
		[scrollView addSubview:pageView.pageView];
	}
	else
	{
		[TJCLog logWithLevel: LOG_DEBUG
						  format: @"No page available: %s: %d; %s;", __FILE__, __LINE__, __PRETTY_FUNCTION__];
	}
	
}


- (void)scrollViewTransitionOutAnimate
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[scrollView setAlpha:0.0f];
	
	[UIView commitAnimations];
}


- (void)scrollViewTransitionInAnimate
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[scrollView setAlpha:1.0f];
	
	[UIView commitAnimations];
}


- (void)refreshScrollView
{		
	NSMutableArray *itemsPageArrayRef;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsPageArrayRef = availableItemsPages;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsPageArrayRef = purchasedItemsPages;
	}
	
	if ([itemsPageArrayRef count] <= 0)
	{
		return;
	}
	
	// Now that the scrollview is not visible, swap in the other view, by refreshing the first couple of pages.
	[self loadScrollViewWithPage:0];
	[self loadScrollViewWithPage:1];	
	
	// This will fade out all buttons on all controllers.
	for (int i = 0; i < [itemsPageArrayRef count]; ++i)
	{
		[[itemsPageArrayRef objectAtIndex:0] fadeInViewWithDuration:TJCVG_VIEW_ANIM_DURATION];
	}
}


- (CGRect)getScrollViewFrame
{
//	CGRect scrollFrame = CGRectMake(scrollView.frame.origin.x, 
//											  scrollView.frame.origin.y, 
//											  scrollView.frame.size.width, 
//											  scrollView.frame.size.height - (pageCtrl.frame.size.height / 2));

	CGRect scrollFrame = CGRectMake(scrollView.bounds.origin.x, 
											  scrollView.bounds.origin.y, 
											  scrollView.bounds.size.width, 
											  scrollView.bounds.size.height - (pageCtrl.bounds.size.height / 2));

	
	return scrollFrame;
}


#pragma mark UITableView implementation

- (UITableViewCell *)tableView:(UITableView *)aTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSArray *itemsArray;
	
	static NSString *vgViewCellIdentifier;
	
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		vgViewCellIdentifier = TJC_VG_TABLEVIEWCELL_ID_AVAILABLE;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		vgViewCellIdentifier = TJC_VG_TABLEVIEWCELL_ID_PURCAHSED;
	}
	
	TJCVGTableViewCell *cell = (TJCVGTableViewCell*)[aTableView dequeueReusableCellWithIdentifier:vgViewCellIdentifier];
	
	if (cell == nil) 
	{
		NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"TJCVGTableViewCell" owner:self options:nil];
		cell = (TJCVGTableViewCell*)[nib objectAtIndex:0];
		[cell setReuseIdentifierStr:vgViewCellIdentifier];
		//[cell setSelectionStyle:UITableViewCellSelectionStyleGray];
		
		UIView *backGroundView = [self customCellBackgroundWithFrame:cell.frame];
		[cell.bgView addSubview:backGroundView];
	}
	
	// Clear all labels.
	cell.typeLabel.text = @"";
	cell.nameLabel.text = @"";
	cell.costLabel.text = @"";
	
	// Determine which array to pull the data from, depending on the currently visible items view.
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsArray = [[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems];
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsArray = [[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems];
	}
	
	if ([itemsArray count] == 0)
	{
		return cell;
	}
	
	// Sanity check to make sure we don't over step array bounds.
	if ([itemsArray count] <= [indexPath row])
	{
		return cell;
	}
	
	// Pull the individual store item data.
	TJCVGStoreItem *currentItem = [itemsArray objectAtIndex:[indexPath row]];
	
	if (currentItem == nil)
	{
		return cell;
	}
	
	cell.typeLabel.text = currentItem.itemTypeName;
	cell.nameLabel.text = currentItem.itemName;
	cell.costLabel.text = [NSString stringWithFormat:@"Cost: %d", currentItem.itemPrice];
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We also set up the scroll view here since it's convenient and we also want to be able to switch views on the fly right after data is loaded.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Each page has a certain amount of buttons. Calculate where the desired button is located. This will contain the loaded icon image.
	int itemPageNum = [indexPath row] / TJCVG_NUM_BUTTONS;
	int buttonNum = [indexPath row] % TJCVG_NUM_BUTTONS;
	
	NSMutableArray *itemsPageArrayRef;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsPageArrayRef = availableItemsPages;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsPageArrayRef = purchasedItemsPages;
		// Make the download label visible since we're viewing purchased items.
		[cell.downloadedLabel setHidden: NO];
	}
	
	// Grab the appropriate page.
	TJCVGPageView *currPage = [itemsPageArrayRef objectAtIndex:itemPageNum];
	
	// Grab the appropriate button item.
	TJCVGItemButton *currButton = [currPage.buttons objectAtIndex:buttonNum];
	
	if (![currButton itemButton].buttonImage.image)
	{
		// Clear out any previous image.
		if (cell.iconImage.image)
		{
			cell.iconImage.image = nil;
		}
		
		// If the image is not yet downloaded, defer loading.
		NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:
											 [self methodSignatureForSelector:@selector(updateImageWithButtonID:withButtonItem:)]];
		[invocation setTarget:self];
		[invocation setSelector:@selector(updateImageWithButtonID:withButtonItem:)];
		[invocation setArgument:&cell atIndex:2];
		[invocation setArgument:&currButton atIndex:3];
		[invocation retainArguments];
		
		[[currButton itemButton] setImageReadyInvocation:invocation];
	}
	else
	{
		// Now set the icon image from the button image.	
		cell.iconImage.image = [currButton itemButton].buttonImage.image;
	}
	
	return cell;
}


- (void)updateImageWithButtonID:(TJCVGTableViewCell*)tableCell withButtonItem:(TJCVGItemButton*)buttonItem
{
	if (!tableCell || !buttonItem)
	{
		return;
	}
	
	tableCell.iconImage.image = [buttonItem itemButton].buttonImage.image;
	
	[tableCell.iconImage setAlpha:0.0f];
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_LOADING_VIEW_ALPHA];
	[tableCell.iconImage setAlpha:1.0f];
	[UIView commitAnimations];
	
	//[tableCell release];
	//[buttonItem release];
}


- (void)tableView:(UITableView *)aTableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
	BOOL shouldEnableBuyButton = YES;
	
	if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		shouldEnableBuyButton = NO;
	}
	
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
	
	// Each page has a certain amount of buttons. Calculate where the desired button is located.
	int itemPageNum = [indexPath row] / TJCVG_NUM_BUTTONS;
	int buttonNum = [indexPath row] % TJCVG_NUM_BUTTONS;
	
	NSMutableArray *itemsPageArrayRef;
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		itemsPageArrayRef = availableItemsPages;
	}
	else //if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		itemsPageArrayRef = purchasedItemsPages;
	}
	
	// Grab the appropriate page.
	TJCVGPageView *currPage = [itemsPageArrayRef objectAtIndex:itemPageNum];
	
	// Grab the appropriate button item.
	TJCVGItemButton *currButton = [currPage.buttons objectAtIndex:buttonNum];
	
	detailView = [[TJCVGItemDetail alloc] initWithItem:currButton 
														  withFrame:mainView.bounds
												withButtonEnabled:shouldEnableBuyButton];
	
	// Add the details view to the calls wrapper view.
	[mainView addSubview:detailView.mainView];
	
	[TJCViewCommons animateTJCView:detailView.mainView 
					 withTJCTransition:TJCTransitionBottomToTop
								withDelay:TJC_DEFAULT_TRANSITION_DELAY];
}


- (UIView*)customCellBackgroundWithFrame:(CGRect)frame
{
	UIView *cellView = [[UIView alloc] initWithFrame:frame];
	[cellView setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
	UIView *cellSeparatorWhiteLine = [[UIView alloc] initWithFrame:CGRectMake(0,0, frame.size.width, 1)];
	[cellSeparatorWhiteLine setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
	UIView *cellSeparatorGrayLine = [[UIView alloc] initWithFrame:CGRectMake(0,0, frame.size.width, 1)];
	[cellSeparatorGrayLine setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
	
	UIImageView *backGroundImageView = [[UIImageView alloc] initWithImage:
													[TJCVGUtil gradientCellBackgroundImageWithWidth:frame.size.width 
																										  height:frame.size.height
																										  white1:-5
																										  white2:0]];
	[backGroundImageView setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
	
	// Add the gradient view.
	[backGroundImageView setFrame:frame];
	[cellView addSubview:backGroundImageView];
	[backGroundImageView release];
	
	// Add the separator lines.
	[cellSeparatorWhiteLine setBackgroundColor:TJCVG_WHITE_ROW_WHITE_SEP];
	[cellView addSubview:cellSeparatorWhiteLine];
	
	CGRect myFrame = cellSeparatorGrayLine.frame;
	myFrame.origin.y = frame.size.height - 1;
	cellSeparatorGrayLine.frame = myFrame;
	
	[cellSeparatorGrayLine setBackgroundColor:TJCVG_WHITE_ROW_GRAY_SEP];
	[cellView addSubview:cellSeparatorGrayLine];
	
	[cellSeparatorWhiteLine release];
	[cellSeparatorGrayLine release];
	
	return [cellView autorelease];
}


- (CGFloat)tableView:(UITableView *)aTableView heightForRowAtIndexPath:(NSIndexPath *)indexPath 
{
	return 90;
}


- (NSInteger)tableView:(UITableView *)aTableView numberOfRowsInSection:(NSInteger)section
{
	NSInteger rows = 0;
	
	if (currentActiveView == TJCVG_ALL_ITEMS_VIEW)
	{
		rows = [[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] allItems] count];
	}
	else if (currentActiveView == TJCVG_PURCHASED_ITEMS_VIEW)
	{
		rows = [[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems] count];
	}
	
	return rows;
}


- (void)tableViewTransitionOutAnimate
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[tableView setAlpha:0.0f];
	
	[UIView commitAnimations];
}


- (void)tableViewTransitionInAnimate
{
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJCVG_VIEW_ANIM_DURATION];
	
	[tableView setAlpha:1.0f];
	
	[UIView commitAnimations];
}


@end
