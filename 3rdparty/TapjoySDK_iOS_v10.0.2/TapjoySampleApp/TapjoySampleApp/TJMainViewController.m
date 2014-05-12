// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJMainViewController.h"
#import "TJAddNewAppViewController.h"
#import "TJEventsViewController.h"
#import "TJDeviceInfoTableViewController.h"
#import "TJEventDataStore.h"
#import "TJAppModel.h"
#import "TJUtils.h"
#import "TJFeaturesViewController.h"
#import <Tapjoy/Tapjoy.h>


typedef enum {
    AppID,
    CurrencyID
} MainPageSelectorType;

@interface TJMainViewController ()

@property (nonatomic, strong) NSArray *apps;
@property (nonatomic, assign) int selectedAppIndex;
@property (nonatomic, assign) int selectedCurrencyIndex;
@property (nonatomic, strong) UIActionSheet *actionSheet;
@property (nonatomic, assign) BOOL connected;
@property (nonatomic, assign) MainPageSelectorType mpst;
@property (nonatomic, strong) UIPopoverController *popover;
@property (weak, nonatomic) IBOutlet UILabel *secretKeyLabel;
@property (weak, nonatomic) IBOutlet UISwitch *switchCurrencyType;
@property (weak, nonatomic) IBOutlet UIButton *buttonAppID;
@property (weak, nonatomic) IBOutlet UIButton *buttonCurrencyID;
@property (weak, nonatomic) IBOutlet TJDebugTextView *debugLog;
@property (weak, nonatomic) IBOutlet UILabel *appIDLabel;
@property (weak, nonatomic) IBOutlet UILabel *currencyIDLabel;
@property (weak, nonatomic) IBOutlet UIView *deviceInfoView;
@property (weak, nonatomic) IBOutlet UIView *appIDView;
@property (weak, nonatomic) IBOutlet UIView *currencyIDView;
@property (weak, nonatomic) IBOutlet UIButton *appIDButton;
@property (weak, nonatomic) IBOutlet UIButton *currencyIDButton;
@property (weak, nonatomic) IBOutlet UIView *bannerAdView;
@property (weak, nonatomic) IBOutlet UIButton *connectButton;


@end

@implementation TJMainViewController


- (id)initWithCoder:(NSCoder *)aDecoder
{
    if(self = [super initWithCoder:aDecoder]) {
        
        _connected = NO;
        
        // Tapjoy Connect Notifications
        [[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(tjcConnectSuccess:)
													 name:TJC_CONNECT_SUCCESS
												   object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(tjcConnectFail:)
													 name:TJC_CONNECT_FAILED
												   object:nil];
    }
    
    return self;
}


- (void)viewDidLoad
{
  [super viewDidLoad];
  
  [_debugLog setEditable:NO];
  [_debugLog setText:@""];
	
	[TJUtils applyRoundedCorners:_deviceInfoView];
	[TJUtils applyTopRoundedCorners:_appIDView];
	[TJUtils applyBottomRoundedCorners:_currencyIDView];
	[TJUtils applyRoundedCorners:_appIDButton];
	[TJUtils applyRoundedCorners:_currencyIDButton];
	[TJUtils applyRoundedCorners:_debugLog];
	
	NSString *navBarFileName = nil;
	UIImage *navBarImage = nil;
	
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
		navBarFileName = @"navbar~ipad.png";
	else
		navBarFileName = @"navbar.png";
	
	navBarImage = [UIImage imageNamed:navBarFileName];
		
	[[[self navigationController] navigationBar] setBackgroundImage:navBarImage forBarMetrics:UIBarMetricsDefault];
  
  if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
    [[self navigationController] navigationBar].tintColor = [UIColor whiteColor];
}

- (void)viewDidUnload
{
  [self setButtonAppID:nil];
  [self setButtonCurrencyID:nil];
	
	[self setSecretKeyLabel:nil];
	[self setAppIDLabel:nil];
	[self setCurrencyIDLabel:nil];
	[self setDeviceInfoView:nil];
	[self setAppIDView:nil];
	[self setCurrencyIDView:nil];
	[self setAppIDButton:nil];
	[self setCurrencyIDButton:nil];
	[self setBannerAdView:nil];
	[self setConnectButton:nil];
  [super viewDidUnload];
}

- (void)didReceiveMemoryWarning
{
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    if ((toInterfaceOrientation == UIInterfaceOrientationPortrait) ||
        (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft))
        return YES;
    
    return NO;
}

- (void)viewWillAppear:(BOOL)animated
{
	[_connectButton setEnabled:YES];
    _selectedAppIndex = [[NSUserDefaults standardUserDefaults] integerForKey:USER_DEFAULT_KEY_SELECTED_INDEX];
    _selectedCurrencyIndex = [[NSUserDefaults standardUserDefaults] integerForKey:USER_DEFAULT_KEY_SELECTED_CURRENCY_INDEX];
    [self updateViewDisplay];
}

#pragma mark - Main View Controller
- (IBAction)connect:(id)sender
{
  // NOTE: This must be replaced by your App ID. It is Retrieved from the Tapjoy website, in your account.
  [Tapjoy requestTapjoyConnect:_appIDLabel.text
                       secretKey:_secretKeyLabel.text
                         options:@{
   TJC_OPTION_TRANSITION_EFFECT : @(TJCTransitionExpand),
      TJC_OPTION_ENABLE_LOGGING : @(YES),
 TJC_OPTION_COLLECT_MAC_ADDRESS : @(TJCMacAddressOptionOffWithVersionCheck),
    // If you are not using Tapjoy Managed currency, you would set your own user ID here.
    // TJC_OPTION_USER_ID : @"A_UNIQUE_USER_ID"
   
    // You can also set your event segmentation parameters here.
    // Example segmentationParams object -- NSDictionary *segmentationParams = @{@"iap" : @(YES)};
    // TJC_OPTION_SEGMENTATION_PARAMS : segmentationParams
     }];
  
    [_debugLog log:@"Connecting..."];
	[_connectButton setEnabled:NO];
}

- (IBAction)switchCurrencyType:(id)sender
{
    [self setCurrencyType:_switchCurrencyType.on];
}

- (IBAction)selectAppID:(id)sender
{
    [self displayMainPageSelectorWithType:AppID];
}

- (IBAction)selectCurrencyID:(id)sender
{
    [self displayMainPageSelectorWithType:CurrencyID];
}

- (IBAction)createNewAppAction:(id)sender
{
	[self performSegueWithIdentifier:@"ToNewApp" sender:self];
}

- (void)displayMainPageSelectorWithType:(MainPageSelectorType)mpst
{
    _mpst = mpst;
	
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        UIToolbar *toolbar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, 320, 44)];
        toolbar.barStyle = UIBarStyleDefault;
        
        UIBarButtonItem *selectButton = [[UIBarButtonItem alloc] initWithTitle:@"Select"
																		 style:UIBarButtonItemStyleBordered
																		target:self
																		action:@selector(dismissSelector:)];
        
        [toolbar setItems:[NSArray arrayWithObjects:selectButton, nil]];
        
        CGRect pickerFrame = CGRectMake(0, 44, 320, 216);
        UIPickerView *pickerView = [[UIPickerView alloc] initWithFrame:pickerFrame];
        pickerView.showsSelectionIndicator = YES;
        pickerView.dataSource = self;
        pickerView.delegate = self;
        
        pickerFrame.origin.y = toolbar.frame.size.height;
        [pickerView setFrame:pickerFrame];
		
        UIView *view = [[UIView alloc] init];
        [view addSubview:pickerView];
        [view addSubview:toolbar];
        
        UIViewController *vc = [[UIViewController alloc] init];
        [vc setView:view];
        [vc setContentSizeForViewInPopover:CGSizeMake(320, 260)];
        
        _popover = [[UIPopoverController alloc] initWithContentViewController:vc];
      
      UIButton* selectedButtonID;
      if (_mpst == AppID)
        selectedButtonID = _buttonAppID;
      else
        selectedButtonID = _currencyIDButton;

      [_popover presentPopoverFromRect:selectedButtonID.bounds
                inView:selectedButtonID
      permittedArrowDirections:UIPopoverArrowDirectionAny
              animated:YES];
        
      [self pickerView:pickerView didSelectRow:0 inComponent:0];
    }
    else
    {
        _actionSheet = [[UIActionSheet alloc] initWithTitle:nil
                                                   delegate:nil
                                          cancelButtonTitle:nil
                                     destructiveButtonTitle:nil
                                          otherButtonTitles:nil];
        
        [_actionSheet setActionSheetStyle:UIActionSheetStyleBlackTranslucent];
        
        CGRect pickerFrame = CGRectMake(0, 40, 0, 0);
        UIPickerView *pickerView = [[UIPickerView alloc] initWithFrame:pickerFrame];
        pickerView.showsSelectionIndicator = YES;
        pickerView.dataSource = self;
        pickerView.delegate = self;
        [pickerView selectRow:0 inComponent:0 animated:NO];
        [_actionSheet addSubview:pickerView];
        
        UISegmentedControl *selectButton = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObject:@"Select"]];
        selectButton.momentary = YES;
        selectButton.frame = CGRectMake(260, 7.0f, 50.0f, 30.0f);
        selectButton.segmentedControlStyle = UISegmentedControlStyleBar;
        selectButton.tintColor = [UIColor blackColor];
        [selectButton addTarget:self action:@selector(dismissSelector:) forControlEvents:UIControlEventValueChanged];
        [_actionSheet addSubview:selectButton];
        
        [_actionSheet showInView:[[UIApplication sharedApplication] keyWindow]];
        
        [_actionSheet setBounds:CGRectMake(0, 0, 320, 485)];
        
        [self pickerView:pickerView didSelectRow:0 inComponent:0];
    }
    
}

- (void)dismissSelector:(id)sender
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        [_popover dismissPopoverAnimated:YES];
    else
        [_actionSheet dismissWithClickedButtonIndex:0 animated:YES];
    
    if (_selectedAppIndex < [[[TJEventDataStore defaultEventDataStore] apps] count])
        [self updateViewDisplay];	
}

- (void)setCurrencyType:(BOOL)isManaged
{
    if (_selectedAppIndex <= 1)
    {
		if (isManaged)
			_selectedAppIndex = 0;
		else
			_selectedAppIndex = 1;
        
        [self updateViewDisplay];
    }
}

- (void)updateViewDisplay
{
    if (_selectedAppIndex >= [[[TJEventDataStore defaultEventDataStore] apps] count])
        _selectedAppIndex = 0;
    
    TJAppModel *app = [[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:_selectedAppIndex];
    
    if (_selectedCurrencyIndex >= [[app currencyIDs] count])
        _selectedCurrencyIndex = 0;
    
	[_appIDLabel setText:app.appID];
    [_secretKeyLabel setText:app.secretKey];
    
	NSString *currID = @"default";
	
    if ([app.currencyIDs count] > 0)
        currID = [app.currencyIDs objectAtIndex:_selectedCurrencyIndex];
	else
		[app setManagedCurrency:YES];
	
	[_currencyIDLabel setText:currID];
    [_buttonCurrencyID setEnabled:!app.isManagedCurrency];
	[_buttonCurrencyID setAlpha:app.isManagedCurrency ? 0.5 : 1];
    
    [_switchCurrencyType setOn:app.isManagedCurrency animated:YES];
    
    if (_selectedAppIndex > 1)
        [_switchCurrencyType setUserInteractionEnabled:NO];
}

#pragma mark - Tapjoy Connect Notifications

-(void)tjcConnectSuccess:(NSNotification*)notifyObj
{
	[_debugLog log:@"Tapjoy connect Succeeded"];
    
    _connected = YES;
   // [[self tableView] reloadData];
    
    [self performSegueWithIdentifier:@"ConnectSuccessSegue" sender:self];
}

- (void)tjcConnectFail:(NSNotification*)notifyObj
{
	[_debugLog log:@"Tapjoy connect Failed"];
}


#pragma mark - Pickerview delegate

- (void)pickerView:(UIPickerView *)pickerView didSelectRow: (NSInteger)row inComponent:(NSInteger)component
{
    if (_mpst == AppID)
        [self setSelectedAppIndex:row];
    else
        [self setSelectedCurrencyIndex:row];
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	if (_mpst == AppID)
	{
		if (row < [[[TJEventDataStore defaultEventDataStore] apps] count])
		{
			TJAppModel *app = [[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:row];
			
			return app.appID;
		}
	}
	else
	{
		TJAppModel *app = [[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:_selectedAppIndex];
		if (row < [[app currencyIDs] count])
		{
			return [[app currencyIDs] objectAtIndex:row];
		}
	}
    return nil;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
    // Width in points
    return 300;
}

#pragma mark - PickerView Datasource

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    if (_mpst == AppID)
        return [[[TJEventDataStore defaultEventDataStore] apps] count];
    else if (_mpst == CurrencyID && _selectedAppIndex < [[[TJEventDataStore defaultEventDataStore] apps] count])
        return [[[[[TJEventDataStore defaultEventDataStore] apps] objectAtIndex:_selectedAppIndex] currencyIDs] count];
    
    return 0;
}

#pragma mark - Storyboard

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    [[NSUserDefaults standardUserDefaults] setValue:@(_selectedAppIndex) forKey:USER_DEFAULT_KEY_SELECTED_INDEX];
    [[NSUserDefaults standardUserDefaults] setValue:@(_selectedCurrencyIndex) forKey:USER_DEFAULT_KEY_SELECTED_CURRENCY_INDEX];
}

@end