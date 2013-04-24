//
//  CurrencyAppViewController.m
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//
// This UIViewController subclass controls the main view of the virtual currency video sample application.
//
// This class controls three important user actions:
// 1.	The user may manually launch a video ad and be rewarded with virtual currency.
// 2.	The user may opt-in to interstitial video ads that reward virtual currency via a toggle control.
// 3.	The user may navigate to the GameContentViewController, which represents a game's content.
//		This navigation action can trigger an interstitial video ad if the user has opted in.
//

#import "AppDelegate.h"
#import "CurrencyAppViewController.h"
#import "GameContentViewController.h"

#define kAdjustAmount       100

@implementation CurrencyAppViewController

@synthesize cm;

#pragma mark general

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return YES;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    onIPadDetermined = NO;
    onIPad = NO;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardAppeared:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardGone:) name:UIKeyboardWillHideNotification object:nil];

	
	// Determine if the user has previously selected to opt in and set the UI accordingly
	userOptedIn = [[NSUserDefaults standardUserDefaults] boolForKey:@"AdColonyInterstitialVideoOptIn"];
	if (userOptedIn) {
		[videoOptInSwitch setOn:YES animated:NO];
	}
    
    ADCOLONY_ZONE_STATUS s = [AdColony zoneStatusForSlot:1];
    if(s != ADCOLONY_ZONE_STATUS_ACTIVE){
		videoButton.enabled = NO;
        [NSThread detachNewThreadSelector:@selector(videoAdsReadyCheck) toTarget:self withObject:nil];
    }
    
    [cm addDelegate:self];
        
    [virtualCurrencyLabel1 setCurrencyLabelToInt:[cm gold]];
    [virtualCurrencyLabel2 setCurrencyLabelToInt:[cm silver]];
}

-(void)videoAdsReadyCheck{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    const float checkEvery = 0.1f; //seconds
	
	BOOL zone1Ready = ([AdColony zoneStatusForSlot:1] == ADCOLONY_ZONE_STATUS_ACTIVE);
    while (!zone1Ready) {
        [NSThread sleepForTimeInterval:checkEvery];
        zone1Ready = ([AdColony zoneStatusForSlot:1] == ADCOLONY_ZONE_STATUS_ACTIVE);
    }
    
    [self performSelectorOnMainThread:@selector(enableVideoButton) withObject:nil waitUntilDone:NO];
    
    [pool release];
}

-(void)enableVideoButton{
    videoButton.enabled = YES;
}

#pragma mark labels

- (void)viewDidAppear:(BOOL)animated {    
    [virtualCurrencyLabel1 setCurrencyLabelToInt:[cm gold]];
    [virtualCurrencyLabel2 setCurrencyLabelToInt:[cm silver]];
}

- (void)viewWillAppear:(BOOL)animated {
	[self adjustUserInfoLabel];
}



- (void)willAnimateSecondHalfOfRotationFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation duration:(NSTimeInterval)duration {
	[self adjustUserInfoLabel];
}

- (void) adjustUserInfoLabel {
	if ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationLandscapeRight || [UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationLandscapeLeft) {
		appInfoLabel.hidden = YES;
	}
	else {
		appInfoLabel.hidden = NO;
	}
}

#pragma mark -
#pragma mark These two actions are tied to the buttons that launch video ads

-(IBAction)videoAdButtonPressed{
	
	if([AdColonyAdministratorPublic virtualCurrencyAwardAvailableForSlot:1]) {
		[AdColonyAdministratorPublic playVideoAdForSlot:1 withDelegate:self withV4VCPrePopup:YES andV4VCPostPopup:YES];		
	}
	
	else{
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Currency Available" message:@"V4VC has been disabled or you are at your play cap."
													   delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
		[alert show];
		[alert release];
		videoButton.enabled = NO;
    }
}

-(IBAction)navigationButtonPressed {
	if(userOptedIn && [AdColonyAdministratorPublic virtualCurrencyAwardAvailableForSlot:2]) {
        navigationVideoPlayed = YES;
		[AdColonyAdministratorPublic playVideoAdForSlot:2 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:YES];		
    }else{
        [self performNavigation];
    }
}

-(IBAction)videoOptInSwitchToggled {
	userOptedIn = videoOptInSwitch.on;
	[[NSUserDefaults standardUserDefaults] setBool:userOptedIn forKey:@"AdColonyInterstitialVideoOptIn"];
}

// Push the new view controller after any required video plays are completed
-(void) performNavigation {
	// Push View Controller
	GameContentViewController *svc = [[GameContentViewController alloc] initWithVideoPlayed:navigationVideoPlayed];
    svc.cm = cm;
	[self.navigationController pushViewController:svc animated:YES];
	[svc release];
	navigationVideoPlayed = NO;
}

#pragma mark extra param

-(NSString *)extraParameterText{
    return extraParameterField.text;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField{
	[textField resignFirstResponder];
	return YES;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    [extraParameterField resignFirstResponder];
}

-(void)keyboardAppeared:(NSNotification *)notification{    
    NSDictionary *d = [notification userInfo];
    
    NSNumber *animDuration = [d objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    double duration = [animDuration doubleValue];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:duration];
    self.view.center = CGPointMake(self.view.center.x, self.view.center.y - kAdjustAmount);
    [UIView commitAnimations];
}

-(void)keyboardGone:(NSNotification *)notification{        
    NSDictionary *d = [notification userInfo];
    
    NSNumber *animDuration = [d objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    double duration = [animDuration doubleValue];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:duration];
    self.view.center = CGPointMake(self.view.center.x, self.view.center.y + kAdjustAmount);
    [UIView commitAnimations];
}

#pragma mark -
#pragma mark AdColonyTakeoverAdDelegate

//app interrupted: called when video is played
-(void)adColonyTakeoverBeganForZone:(NSString *)zone{
	NSLog(@"Video ad or fullscreen banner launched for zone %@", zone);
}

//app interruption over: called when video ad is dismissed
-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward {
	NSLog(@"Video ad or fullscreen banner finished in zone %@", zone);
	
	// If the play came from the Navigation Button, continue with execution.
	if (userOptedIn && navigationVideoPlayed) {
		[self performNavigation];
	}
}

-(void)adColonyVideoAdNotServedForZone:(NSString *)zone
{
	NSLog(@"ad not available for zone %@", zone);
	
	// If the play came from the Navigation Button, continue with execution.
	if (userOptedIn && navigationVideoPlayed) {
		[self performNavigation];
	}	
}

#pragma mark -
#pragma mark currency manager delegate

-(void)currencyAwardSucceeded{    
    [virtualCurrencyLabel1 setCurrencyLabelToInt:[cm gold]];
    [virtualCurrencyLabel2 setCurrencyLabelToInt:[cm silver]];
}

-(void)currencyAwardFailed{
	if(![AdColonyAdministratorPublic virtualCurrencyAwardAvailableForSlot:1]) {
        videoButton.enabled = NO;
    }
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc {
    [cm removeDelegate:self];
	[videoOptInSwitch release];
	[virtualCurrencyLabel1 release];
    [virtualCurrencyLabel2 release];
	[videoButton release];
    [super dealloc];
}

@end
