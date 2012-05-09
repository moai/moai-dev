// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

typedef enum eTJCVGActiveItemsView
{
	TJCVG_ALL_ITEMS_VIEW = 0,
	TJCVG_PURCHASED_ITEMS_VIEW,
	TJCVG_ITEMS_VIEW_MAX,
} eTJCVGActiveItemsView;

typedef enum eTJCVGViewStyle
{
	TJCVG_TABLEVIEW_MODE = 0,
	TJCVG_SCROLLVIEW_MODE,
	TJCVG_VIEWSTYLE_MAX,
} eTJCVGViewStyle;

@class TJCVGItemDetail;

@interface TJCVGView : NSObject <UIScrollViewDelegate, UIAlertViewDelegate, UITableViewDelegate, UITableViewDataSource>
{
	IBOutlet UIView *mainView;					/*<! The main virtual goods view that all other objects are placed in. */
	IBOutlet UIPageControl *pageCtrl;			/*<! The page control tells us what page we are currently on in the scroll view. */
	IBOutlet UINavigationBar *navBar;			/*!< The bar at the top of the view. */
	IBOutlet UIBarButtonItem *navBarButton;		/*!< The button on the bar at the top of the view. */
	
	IBOutlet UIScrollView *scrollView;			/*!< The scroll view holds all the item buttons that populate scroll view. */
	IBOutlet UITableView *tableView;			/*!< The a standard table view that displays the virtual good items. */
	
	IBOutlet UINavigationBar *toolBarLeft;		/*!< The bar at the bottom left of the view. */
	IBOutlet UINavigationBar *toolBarRight;		/*!< The bar at the bottom right of the view. */
	IBOutlet UISegmentedControl *itemSelectorButton;	/*!< The toggle button that switches between viewing all or purchased items. */
	IBOutlet UISegmentedControl *viewModeSelectorButton;/*!< The toggle button that switches between viewing the items in a table or a scrollview.. */
	IBOutlet UIBarButtonItem *earnMoreButton;	/*!< This button will show the offer wall. */
	
	IBOutlet UIView *loadingView;				/*!< The loading view covers everything while the activity indicator is active. */
	IBOutlet UIActivityIndicatorView *genericIndicator;	/*!< Used for loading screens. */
	IBOutlet UILabel *loadingTextLabel;			/*!< The loading text. */
	IBOutlet UILabel *genericLabel;				/*!< A text label that is displayed when no items are available. */
	IBOutlet UILabel *itemTypeLabel;			/*!< The text label that indicates which view is currently showing. */
	IBOutlet UILabel *currencyLabel;			/*!< The text label that indicates the amount of currency available. */
	IBOutlet UILabel *bgLabel;					/*!< Covers the area behind the itemTypeLabel and currencyLabel. */
	NSMutableArray *availableItemsPages;		/*!< The TJCVGPageView array for available vg items. */
	NSMutableArray *purchasedItemsPages;		/*!< The TJCVGPageView array for purchased vg items. */
	BOOL shouldReloadAvailableItemsPages;		/*!< Flag indicating whether the item pages needs to be reloaded. This should only happen on init and after an item has been purchased. */
	BOOL shouldReloadPurchasedItemsPages;		/*!< Flag indicating whether the item pages needs to be reloaded. This should only happen on init and after an item has been purchased. */
	int numPages;								/*!< The total number of pages, determined by the number of virtual goods available. */
    BOOL pageControlUsed;						/*!< To be used when scrolls originate from the UIPageControl. */
	eTJCVGActiveItemsView currentActiveView;	/*!< Indicates whether all items or purchased items are visible. */
	eTJCVGViewStyle currentViewStyle;			/*!< Indicates whether the table view or the scroll view is visible. */
	TJCVGItemDetail *detailView;				/*!< The detail view is used when loading item details from the table view (scroll view handles it differently). */
	BOOL areAvailableVGItemsAvailable;			/*!< Used to prevent the table view from showing up when toggling the view mode when there are no virtual goods available. */
	BOOL arePurchasedVGItemsAvailable;			/*!< Used to prevent the table view from showing up when toggling the view mode when there are no virtual goods available. */
	UIInterfaceOrientation lastOrientation;		/*!< Just in case any views didn't recieve this message (such as if the alpha was zero when orientation changed), we need to know this value. */
}

+ (TJCVGView*) sharedTJCVGView;

@property (nonatomic, retain) IBOutlet UIView *mainView;
@property (nonatomic, retain) IBOutlet UIPageControl *pageCtrl;
@property (nonatomic, retain) IBOutlet UINavigationBar *navBar;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *navBarButton;
@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;
@property (nonatomic, retain) IBOutlet UITableView *tableView;
@property (nonatomic, retain) IBOutlet UINavigationBar *toolBarLeft;
@property (nonatomic, retain) IBOutlet UINavigationBar *toolBarRight;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *earnMoreButton;
@property (nonatomic, retain) IBOutlet UISegmentedControl *itemSelectorButton;
@property (nonatomic, retain) IBOutlet UISegmentedControl *viewModeSelectorButton;
@property (nonatomic, retain) IBOutlet UILabel *genericLabel;
@property (nonatomic, retain) IBOutlet UIView *loadingView;
@property (nonatomic, retain) IBOutlet UIActivityIndicatorView *genericIndicator;
@property (nonatomic, retain) IBOutlet UILabel *loadingTextLabel;
@property (nonatomic, retain) IBOutlet UILabel *itemTypeLabel;
@property (nonatomic, retain) IBOutlet UILabel *currencyLabel;
@property BOOL shouldReloadAvailableItemsPages;
@property BOOL shouldReloadPurchasedItemsPages;


// Notifications for when VG items are recieved.
- (void)addObserversForAllVGStoreItems;
- (void)addObserversForPurchasedVGStoreItems;
- (void)removeObserversForAllVGStoreItems;
- (void)removeObserversForPurchasedVGStoreItems;
- (void)addObserversForLoadingItems;
- (void)removeObserversForLoadingItems;

// Initiates the request for both purchased and available virtual good items.
- (void)initiateVGItemsDownload;

// Initiates the request for all virtual good items.
- (void)initiateAllItemsVGDownload;

// Initiates the request for purchased virtual good items.
- (void)initiatePurchasedItemsVGDownload;

// Called by the notification center when virtual good items are recieved from the server.
- (void)recievedAllVGItems;

// Called by the notification center when purchased virtual good items are recieved from the server.
- (void)recievedPurchasedVGItems;

// Called by the notification center when an error occurs during reception of item data from the server.
- (void)VGAllItemsNetError;

// Called by the notification center when an error occurs during reception of item data from the server for purchased goods.
- (void)VGPurchasedItemsNetError;

- (void)refreshWithFrame:(CGRect)frame;

// Loaded whenever the user scrolls the page either through touch-dragging, or the page control.
- (void)loadScrollViewWithPage:(int)page;

// Linked to the page control, is called upon the 'value changed' action.
- (IBAction)changePage:(id)sender;

// Dismisses the entire view.
- (IBAction)dismissView;

// Called after the view animates out, removes the current view from the superview.
- (void)removeView;

// Sets a generic label in the center of the view and animates it in.
- (void)showGenericLabel:(NSString*)msg;

// Animates the generic label out of visibility.
- (void)hideGenericLabel;

// The button action for the item view toggle button between viewing all items and purchased items.
- (IBAction)toggleItemViewAction;

// The button action for loading purchased items. This assumes that the data has already been downloaded. This must not be called before recievedPurchasedVGItems is invoked.
- (void)loadPurchasedItemsAction;

// The button action for loading all virtual good items. This assumes that the data has already been downloaded. This must not be called before recievedAllVGItems is invoked.
- (void)loadAllItemsAction;

// Loads the given vg items into the page views.
- (void)loadItemsFromArray:(NSArray*)itemsArray;

// Refreshes the items in the current active view. This pings the server for a hard refresh of the items.
- (void)refreshItems;

// Shows the offers view from within this virtual goods view.
- (IBAction)showOffersView;

// Does a smooth transition when switching between virtual good views.
- (void)scrollViewTransitionOutAnimate;
- (void)scrollViewTransitionInAnimate;
- (void)tableViewTransitionOutAnimate;
- (void)tableViewTransitionInAnimate;
- (void)currentViewModeTransitionOutAnimate;
- (void)currentViewModeTransitionInAnimate;

// Refreshes the pages in the scroll view. This is to be called after items have been loaded.
- (void)refreshScrollView;

// The scroll view takes into account the page control below it.
- (CGRect)getScrollViewFrame;

// Fades in/out the loading views.
- (void)showLoadingViews;
- (void)hideLoadingViews;
- (void)hideLoadingViewsEnded:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;

- (void)updateViewWithOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;

// Returns the enum value that tells whether the available items or the purchased items is currently visible.
- (eTJCVGActiveItemsView)getCurrentActiveView;

// Sets the tint color of the view and all elements within it with the given color.
- (void)setViewColor:(UIColor*)color;

// Returns a customized table cell view background view.
- (UIView*)customCellBackgroundWithFrame:(CGRect)frame;

// Toggles between the view mode. Table view or scroll view.
- (IBAction)toggleViewMode;

// Toggles whether certain buttons can be interacted with. This prevents monkey button pressing.
- (void)disableButtons;
- (void)enableButtons;

// Convenience methods for hiding the item views.
- (void)hideTableView;
- (void)hideScrollView;

// The toggle button between table and scroll views is disabled when there are no available items.
- (void)disableViewStyleButton;

@end
