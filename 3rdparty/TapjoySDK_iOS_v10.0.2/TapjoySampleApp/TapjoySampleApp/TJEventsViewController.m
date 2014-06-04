// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJEventsViewController.h"
#import "TJEventDataStore.h"
#import "TJEventModel.h"
#import "TJUtils.h"
#import <Tapjoy/Tapjoy.h>
#import <Tapjoy/TJEvent.h>


@interface TJEventsViewController ()

@property (weak, nonatomic) IBOutlet UITableView *paramTblView;
@property (weak, nonatomic) IBOutlet TJDebugTextView *debugLog;
@property (weak, nonatomic) IBOutlet UIButton *selectedEventBtn;
@property (weak, nonatomic) IBOutlet UIButton *sendBtn;

@property (nonatomic, strong) NSArray *events;
@property (nonatomic, strong) UIActionSheet *actionSheet;
@property (nonatomic, strong) TJEvent *tapjoyEvent;

@end

@implementation TJEventsViewController

- (id)initWithEvents:(NSArray *)events
{
    _events = events;
    
    return [self initWithNibName:nil bundle:nil];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  
  [_debugLog setEditable:NO];
  [_debugLog setText:@""];
    
  _actionSheet = [[UIActionSheet alloc] initWithTitle:nil
											   delegate:self
									  cancelButtonTitle:nil
								 destructiveButtonTitle:nil
									  otherButtonTitles: nil];
    
	for(TJEventModel *event in _events)
		[_actionSheet addButtonWithTitle: [event name]];
    
    // TODO: If there is only one event, don't display the option to select
//    if (_events.count == 1) {
//        [_sendBtn setUserInteractionEnabled: YES];
//        [_presentBtn setUserInteractionEnabled: NO];
//        [_selectedEventBtn setTitle:@"Select Event" forState:UIControlStateNormal];
//    }
//    else
    {
        [_sendBtn setUserInteractionEnabled: NO];
        [_selectedEventBtn setTitle:@"Select Event" forState:UIControlStateNormal];
    }
	
	[TJUtils applyRoundedCorners:_selectedEventBtn];
	[TJUtils applyRoundedCorners:_sendBtn];
	[TJUtils applyRoundedCorners:_paramTblView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    if ((toInterfaceOrientation == UIInterfaceOrientationPortrait) ||
        (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft))
        return YES;
    
    return NO;
}

- (IBAction)selectEvent:(id)sender
{
	[_actionSheet showInView:self.view];
}

- (IBAction)sendEvent:(id)sender
{
	[_tapjoyEvent setPresentationViewController:self.navigationController];
    [_tapjoyEvent send];
}

#pragma mark - Debug Log
- (void)log:(NSString *)format, ...
{
    // Make string from the format and vars provided
    va_list args;
    va_start(args, format);
    NSString *s = [[NSString alloc] initWithFormat:format arguments:args];
    va_end(args);
    
    // Append string onto our debug log
    [_debugLog setText:[NSString stringWithFormat: @"%@\n%@", [_debugLog text], s]];
    
    // Adjust size of debugLog to fit content
    CGRect frame = _debugLog.frame;
    frame.size.height = _debugLog.contentSize.height;
    _debugLog.frame = frame;
}

#pragma mark - UIActionSheetDelegate methods

- (void)actionSheet:(UIActionSheet*)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex >= 0) {
		TJEventModel *evt = [_events objectAtIndex:buttonIndex];
		
		// clear out any existing delegate before sending a new event
		if(_tapjoyEvent)
		{
			_tapjoyEvent.delegate = nil;
		}
		
		_tapjoyEvent = [TJEvent eventWithName:[evt name] value:[evt value] delegate:self];

		[_selectedEventBtn setTitle:[evt name] forState:UIControlStateNormal];
		[_sendBtn setUserInteractionEnabled:YES];
		[_sendBtn setAlpha:1.0];
	
		[_paramTblView setDataSource:evt];
		[_paramTblView setDelegate:evt];
	
		[_paramTblView reloadData];
	}
}

#pragma mark - TJEventDelegate

- (void)sendEventComplete:(TJEvent*)event withContent:(BOOL)contentIsAvailable
{
	[_debugLog log:@"Send event complete, contentIsAvailable: %d", contentIsAvailable];
}

- (void)sendEventFail:(TJEvent*)event error:(NSError*)error
{
	[_debugLog log:@"Tapjoy Ad Unit error: %@", [error localizedDescription]];
}

- (void)contentWillAppear:(TJEvent*)event
{
	[_debugLog log:@"Tapjoy event content will appear"];
}

- (void)contentDidAppear:(TJEvent*)event
{
	[_debugLog log:@"Tapjoy event content did appear"];
}

- (void)contentWillDisappear:(TJEvent*)event
{
	[_debugLog log:@"Tapjoy event content will disappear"];
}

- (void)contentDidDisappear:(TJEvent*)event
{
	[_debugLog log:@"Tapjoy event content did disappear"];
}

- (void)event:(TJEvent*)event didRequestAction:(TJEventRequest*)request
{
	NSString *message = [NSString stringWithFormat: @"Type: %d, Identifier: %@, Quantity: %d", request.type, request.identifier, request.quantity];
	[[[UIAlertView alloc] initWithTitle: @"Got Action Callback" message: message delegate: nil cancelButtonTitle: @"OK" otherButtonTitles: nil] show];
	
	/*
	// Your app should perform an action based on the value of the request.type property
	switch(request.type) {
		case TJEventRequestInAppPurchase:
			// Your app should initiate an in-app purchase of the product identified by request.identifier
			break;
		case TJEventRequestVirtualGood:
			// Your app should award the user the item specified by request.identifier with the amount specified by request.quantity
			break;
		case TJEventRequestCurrency:
			// The user has been awarded the currency specified with request.identifier, with the amount specified by request.quantity
			break;
		case TJEventRequestNavigation:
			// Your app should attempt to navigate to the location specified by request.identifier
			break;
	}
	*/
	
	// Your app must call either completed or cancelled to complete the lifecycle of the request
	[request completed];
}


#pragma mark - TJAdUnitDelegate methods

- (void) didReceiveAd:(TJCAdView*)adView
{
	[adView setFrame:CGRectMake(0, 0, 320, 50)];
	//[self.view addSubview:adView];
}

- (void) didFailWithMessage:(NSString*)msg
{
	[_debugLog log:@"Banner ad error:%@", msg];
}

- (NSString*) adContentSize
{
	return TJC_DISPLAY_AD_SIZE_320X50;
}

- (BOOL) shouldRefreshAd
{
	return NO;
}

- (void)viewDidAppearWithType:(int)viewType
{
	[_debugLog log:@"view did appear %d", viewType];
}

- (void)viewDidDisappearWithType:(int)viewType
{
	[_debugLog log:@"view did disappear %d", viewType];
}

- (void)dealloc
{
	_tapjoyEvent.delegate = nil;
}

@end