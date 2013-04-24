//
//  StripAdViewController.m
//
//  Created by Lev Trubov on 3/1/10.
//  Copyright 2010 Jirbo, Inc. All rights reserved.
//

#import "StripAdViewController.h"
//#import "AdColonyUIDeviceHardware.h"
#import	"AdColonyAdvancedAppAppDelegate.h"

@implementation StripAdViewController

//*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	time = 0;
	timeLabel.text = [NSString stringWithFormat:@"%1.1f", time];
	counter = [[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(timerFunction) userInfo:nil repeats:YES] retain];
	paused = NO;
}
//*/

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[timeLabel release];
	[counter release];
    [super dealloc];
}

-(IBAction)back{
	[counter invalidate];
	if([self parentViewController] && [[self parentViewController] modalViewController] && [[[self parentViewController] modalViewController] isEqual:self]){
		[self dismissModalViewControllerAnimated:YES];
	}else{
		[[self navigationController] popViewControllerAnimated:YES];
	}
}

-(void)timerFunction{
	if(!paused){
		time++;
		timeLabel.text = [NSString stringWithFormat:@"%1.1f", time/10.0f];
	}
}

#pragma mark takeover delegate 

-(void)adColonyTakeoverBegan{
	NSLog(@"strip view controller: ad launched");
	logMessage(@"Banner ad activated");
	paused = YES;
}

-(void)adColonyTakeoverEnded{
	NSLog(@"strip view controller: ad gone");
	logMessage(@"Banner ad closed");
	paused = NO;
}

-(void)adColonyBannerAdServed{
	logMessage(@"Banner ad served");
}

-(void)adColonyBannerAdNotServed{
	logMessage(@"Banner ad could not be served");
}

@end
