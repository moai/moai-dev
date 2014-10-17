//
//  TJFeaturesViewController.m
//  TapjoySampleApp
//
//  Created by Melinda Tang on 5/17/13.
//  Copyright (c) 2013 Tapjoy. All rights reserved.
//

#import "TJFeaturesViewController.h"
#import "TJAddNewAppViewController.h"
#import "TJEventsViewController.h"
#import "TJDeviceInfoTableViewController.h"
#import "TJEventDataStore.h"
#import "TJAppModel.h"
#import "TJUtils.h"
#import <Tapjoy/Tapjoy.h>

@interface TJFeaturesViewController ()<TJEventDelegate, TJCAdDelegate>

@property (nonatomic, assign) int selectedAppIndex;
@property (nonatomic, assign) int selectedCurrencyIndex;
@property (nonatomic, strong) NSMutableArray *categories;
@property (nonatomic, strong) NSArray *apps;
@property (nonatomic, strong) UIActionSheet *actionSheet;
@property (nonatomic, strong) UIPopoverController *popover;
@property (weak, nonatomic) IBOutlet UIView *bannerAdView;
@end

@implementation TJFeaturesViewController

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if(self = [super initWithCoder:aDecoder])
	{
        _categories = [[NSMutableArray alloc] initWithObjects:
                       NSLocalizedString(@"Events-Based APIs", @"Sends an event name to the server"),
                       NSLocalizedString(@"Offerwall", @"Displays the offerwall"),
					   NSLocalizedString(@"Get Direct Play Video Ad", @"Displays a direct play video"),
                       NSLocalizedString(@"Banner Ad", @"Displays the banner ad"),
                       nil];
		
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(showDirectPlayVideoAd:)
													 name:TJC_FULL_SCREEN_AD_RESPONSE_NOTIFICATION
												   object:nil];
		
		// Fired for any Tapjoy view being closed.
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(tapjoyViewClosed:)
													 name:TJC_VIEW_CLOSED_NOTIFICATION
												   object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(offerwallLoadError:)
													 name:TJC_OFFERS_RESPONSE_NOTIFICATION_ERROR
												   object:nil];
    }
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
    _selectedAppIndex = [[NSUserDefaults standardUserDefaults] integerForKey:USER_DEFAULT_KEY_SELECTED_INDEX];
    _selectedCurrencyIndex = [[NSUserDefaults standardUserDefaults] integerForKey:USER_DEFAULT_KEY_SELECTED_CURRENCY_INDEX];
	
	TJAppModel *myApp = nil;
	if (_selectedAppIndex < [[[TJEventDataStore defaultEventDataStore] apps] count])
		myApp = [[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:_selectedAppIndex];
	
	if ([myApp isManagedCurrency])
	{
		[_categories addObject:NSLocalizedString(@"Virtual Currency", @"Methods for accessing the user account model")];
	}
	
	NSString *navBarFileName = nil;
	UIImage *navBarImage = nil;
	
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
		navBarFileName = @"navbar~ipad.png";
	else
		navBarFileName = @"navbar.png";
	
	navBarImage = [UIImage imageNamed:navBarFileName];
    
	[[[self navigationController] navigationBar] setBackgroundImage:navBarImage forBarMetrics:UIBarMetricsDefault];
}

- (void)viewDidUnload
{
	[self setBannerAdView:nil];
    [super viewDidUnload];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Other Tapjoy notifications

- (void)showDirectPlayVideoAd:(NSNotification*)notification
{
	[Tapjoy showFullScreenAdWithViewController:self.navigationController];
}

- (void)tapjoyViewClosed:(NSNotification*)notification
{
	[Tapjoy getTapPoints];
}

- (void)offerwallLoadError:(NSNotification*)notification
{
	NSLog(@"There was an error loading the offerwall");
}

#pragma mark - Tapjoy Display Ad Delegate methods

- (void) didReceiveAd:(TJCAdView*)adView
{
	[adView setFrame:CGRectMake(0, 0, 320, 50)];
	[_bannerAdView addSubview:adView];
}

- (void) didFailWithMessage:(NSString*)msg
{
	NSLog(@"Banner ad error:%@", msg);
}

- (NSString*) adContentSize
{
	return TJC_DISPLAY_AD_SIZE_320X50;
}

- (BOOL) shouldRefreshAd
{
	return NO;
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [_categories count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return NSLocalizedString(@"API Calls", @"The API Calls available to the developer");
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *NoSegueCellIdentifier = @"NoSegueCell";
    static NSString *SegueToVirtualCurrencyCellIdentifier = @"SegueToVirtualCurrencyCell";
    
    NSString *cellIdentifier = NoSegueCellIdentifier;
		
	if ([_categories[indexPath.row] isEqualToString:@"Virtual Currency"])
	{
		cellIdentifier = SegueToVirtualCurrencyCellIdentifier;
	}
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
	{
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
    }
    
    [[cell textLabel] setText:[_categories objectAtIndex:[indexPath row]]];
    
    return cell;
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    TJAppModel *myApp = nil;
    if (_selectedAppIndex < [[[TJEventDataStore defaultEventDataStore] apps] count])
        myApp = [[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:_selectedAppIndex];
    
	if ([_categories[indexPath.row] isEqualToString:@"Events-Based APIs"])
	{
		[self performSegueWithIdentifier:@"FeaturesToEventsSegue" sender:self];
	}
	else if ([_categories[indexPath.row] isEqualToString:@"Offerwall"])
	{
		if (myApp.managedCurrency)
			[Tapjoy showOffersWithViewController:self.navigationController];
		else
		{
			if (_selectedCurrencyIndex < [[myApp currencyIDs] count])
			{
				[Tapjoy showOffersWithCurrencyID:[[myApp currencyIDs] objectAtIndex:_selectedCurrencyIndex]
							  withViewController:self.navigationController
							withCurrencySelector:NO];
			}
		}
	}
	else if ([_categories[indexPath.row] isEqualToString:@"Get Direct Play Video Ad"])
	{
		// Shows a direct play video
		// The getFullScreenAd call has been deprecated and should only be used for getting direct play videos.
		// To show a full screen ad please use the Events framework -- see TJEventsViewController for more details
		if (myApp.managedCurrency)
			[Tapjoy getFullScreenAd];
		else
		{
			if (_selectedCurrencyIndex < [[myApp currencyIDs] count])
				[Tapjoy getFullScreenAdWithCurrencyID:[[myApp currencyIDs] objectAtIndex:_selectedCurrencyIndex]];
		}
	}
	else if ([_categories[indexPath.row] isEqualToString:@"Banner Ad"])
	{
		if (myApp.managedCurrency)
			[Tapjoy getDisplayAdWithDelegate:self];
		else
		{
			if (_selectedCurrencyIndex < [[myApp currencyIDs] count])
				[Tapjoy getDisplayAdWithDelegate:self currencyID:[[myApp currencyIDs] objectAtIndex:_selectedCurrencyIndex]];
		}
        
	}
    
    // Always deselect the row
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

#pragma mark - Storyboard

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"FeaturesToEventsSegue"])
        [segue.destinationViewController setEvents:[[TJEventDataStore defaultEventDataStore] events]];
}

@end
