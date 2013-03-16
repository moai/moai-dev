//
//  AdColonyAdvancedAppViewController.m
//  AdColonyAdvancedApp
//
//  Created by Lev Trubov on 2/16/10.
//  Copyright Jirbo, Inc 2010. All rights reserved.
//

#import "AdColonyAdvancedAppViewController.h"
#import "LoggingTableCell.h"

#define kDefaultButtonMessage   @"Instant Video"
#define kUnpauseButtonMessage   @"Resume Ad"

#define kAlertFailTag           1
#define kAlertPauseTag          2

@implementation AdColonyAdvancedAppViewController

@synthesize loggedData;

-(void)viewDidLoad{
	[super viewDidLoad];
	time = 0;
	startingTime = [[NSDate date] timeIntervalSince1970];
	timeLabel.text = [NSString stringWithFormat:@"%1.1f", time];
	paused = NO;
    adPaused = NO;
	
	[self startTimer];
}

-(void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	[timeLabel release];
	[loggedData release];
	[super dealloc];
}

#pragma mark test ads

-(IBAction)buttonPushed:(id)sender{
    if(adPaused){
        [self unpauseVideoAd:YES];
    }else{
        [self startVideoAd];
    }
}

-(void)startVideoAd{
    [[delegate sm] pauseMusic];
	logMessage(@"Requesting video ad");
	//Call to play video
    [AdColony playVideoAdForSlot:2 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:NO];
}

-(void)pauseVideoAd{
    [AdColony pauseVideoAdForSlot:2 andGoIntoBackground:[backgroundSwitch isOn]];
}

-(void)unpauseVideoAd:(BOOL)backgrounded{
    adPaused = NO;
    [AdColony unpauseVideoAdForSlot:2];
    if(backgrounded){
        mainButton.titleLabel.textColor = [UIColor whiteColor];
        mainButton.titleLabel.text = kDefaultButtonMessage;
    }
}


#pragma mark ad takeover delegate
//Should implement any app-specific code that should be run when an ad that takes over the screen is launched
//(for example, pausing a game if the ad is in the middle of a session).
-(void)adColonyTakeoverBeganForZone:(NSString *)zone{
	logMessage(@"Video ad takeover began");
	NSLog(@"root view controller: ad launched in zone %@", zone);
	paused = YES;
    if([pauseSwitch isOn]){
        [self performSelector:@selector(pauseVideoAd) withObject:nil afterDelay:5.0];
    }
}

//Should implement any app-specific code that should be run when an ad that takes over the screen is quit
//(for example, resuming a game if the ad was in the middle of a session).
-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward{
	[[delegate sm] playMusic];
	NSLog(@"root view controller: ad gone in zone %@", zone);
	logMessage(@"Video ad takeover ended");
	paused = NO;
}

//Should implement any app-specific code that should be run when a video ad has failed to deliver
-(void)adColonyVideoAdNotServedForZone:(NSString *)zone{
	logMessage(@"Video ad could not be served");
	[[delegate sm] playMusic];
	UIAlertView *fail_alert = [[UIAlertView alloc] initWithTitle:@"ADC Video" message:@"Oops, no video fill at this time. Please check with an Adcolony Admin" delegate:self cancelButtonTitle:@"ok" otherButtonTitles:nil];
    fail_alert.tag = kAlertFailTag;
	[fail_alert show];
	[fail_alert release];
}

-(void)adColonyVideoAdPausedInZone:(NSString *)zone{
    adPaused = YES;
    if([backgroundSwitch isOn]){
        mainButton.titleLabel.textColor = [UIColor redColor];
        mainButton.titleLabel.text = kUnpauseButtonMessage;
    }else{
        UIAlertView *pauseAlert = [[UIAlertView alloc] initWithTitle:@"Pause" message:@"Video Ad Paused" delegate:self cancelButtonTitle:@"Resume" otherButtonTitles:nil];
        pauseAlert.tag = kAlertPauseTag;
        [pauseAlert show];
        [pauseAlert release];
    }
}

-(void)adColonyVideoAdResumedInZone:(NSString *)zone{
    
}

#pragma mark alert callback
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView.tag == kAlertFailTag){
        paused = NO;
    }else{
        [self unpauseVideoAd:NO];
    }
}

#pragma mark timer

// New Timer
-(void) startTimer
{
	NSThread* timerThread = [[NSThread alloc] initWithTarget:self selector:@selector(startTimerThread) object:nil]; //Create a new thread
	[timerThread start]; //start the thread
	[timerThread release];
}

//the thread starts by sending this message
-(void) startTimerThread
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSRunLoop* runLoop = [NSRunLoop currentRunLoop];
	[NSTimer scheduledTimerWithTimeInterval: 0.1
									  target: self
									selector: @selector(timerTick:)
									userInfo: nil
									 repeats: YES];
	
	[runLoop run];
	[pool release];
}

- (void)timerTick:(NSTimer *)timer
{
	if(!paused){
		
		float timeFloat = time / 10.0f;
		time++;
		timeLabel.text = [NSString stringWithFormat:@"%1.1f", timeFloat];
	}
}

#pragma mark -
#pragma mark Table View Data Source Methods
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section
{
	return [loggedData count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView
		 cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	NSUInteger row = [indexPath row];
	NSString *logEntry = [loggedData objectAtIndex:row];
	
	static NSString *loggingCellIdentifier = @"loggingCellIdentifier";
	
	LoggingTableCell *cell = (LoggingTableCell *)[tableView dequeueReusableCellWithIdentifier:loggingCellIdentifier];
	if (cell == nil) {
		NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"LoggingTableCell" owner:self options:nil];
		cell = [nib objectAtIndex:0];
	}
	
	cell.logLabel.text =logEntry;
	
	cell.selectionStyle = UITableViewCellSelectionStyleNone;

	return cell;
}	

- (CGFloat)tableView:(UITableView *)tableView
heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return 20;
}

- (void)newIncomingLogStatement:(NSNotification *)aNotification
{
	// NSLog(@"I am a new log statement");
	NSDictionary *parameters = [aNotification object];
	NSString *newLog = [parameters objectForKey:@"log"];
	[loggedData addObject:newLog];
		
	[self performSelectorOnMainThread:@selector(scrollTableToEnd) withObject:nil waitUntilDone:NO];
}

-(void) scrollTableToEnd
{
	[loggingTableView reloadData];
	
	if([loggedData count] > 0)
	{
		NSIndexPath *indPath = [NSIndexPath indexPathForRow:([loggedData count] - 1) inSection:0];
		[loggingTableView scrollToRowAtIndexPath:indPath atScrollPosition:UITableViewScrollPositionBottom animated:NO];
	}
}

@end
