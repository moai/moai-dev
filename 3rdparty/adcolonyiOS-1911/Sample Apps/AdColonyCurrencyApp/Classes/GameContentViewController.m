//
//  GameContentViewController.m
//  AdColonyBasicApp
//
//  Created by Erik Seeley on 2/22/11.
//  Copyright 2011 Jirbo. All rights reserved.
//

#import "GameContentViewController.h"
#import "AppDelegate.h"


@implementation GameContentViewController

@synthesize cm;

// This is the designated initializer for this class
-(id) initWithVideoPlayed:(BOOL)videoWatched {
	NSString *xibName = @"GameContentViewController";
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		xibName = @"GameContentViewController-iPad";
	}
	
	if ((self = [super initWithNibName:xibName bundle:nil])) {
		showedVideo = videoWatched;
	}
	
	return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
    [cm addDelegate:self];
	if (showedVideo) {
		balanceEarnedLabel.text = [NSString stringWithFormat:@"You've earned +%i %@.", 
								   [AdColonyAdministratorPublic getVirtualCurrencyRewardAmountForSlot:2],
								   [AdColonyAdministratorPublic getVirtualCurrencyNameForSlot:2]];
		disclaimerLabel.hidden = NO;
	}
	else {
		balanceEarnedLabel.text = [NSString stringWithFormat:@"User is now in the app.  No Virtual Currency was earned."];
		disclaimerLabel.hidden = YES;
	}
    
    [currentBalanceLabel1 setCurrencyLabelToInt:[cm gold]];
    [currentBalanceLabel2 setCurrencyLabelToInt:[cm silver]];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return YES;
}

-(void) updateCurrencyDisplay {
    currentBalanceLabel1.text = [NSString stringWithFormat:@"%i", [cm gold]];
    currentBalanceLabel2.text = [NSString stringWithFormat:@"%i", [cm silver]];
}

- (void)dealloc {
	[balanceEarnedLabel release];
	[currentBalanceLabel1 release];
    [currentBalanceLabel2 release];
	[disclaimerLabel release];
    [super dealloc];
}

-(IBAction) backButtonPressed {
    [cm removeDelegate:self];
	[self.navigationController popViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark cm delegate

-(void)currencyAwardSucceeded{
    [self updateCurrencyDisplay];
}

-(void)currencyAwardFailed{
    
}

@end
