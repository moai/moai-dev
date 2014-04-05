//
//  AdColonyAdvancedAppViewController.h
//  AdColonyAdvancedApp
//
//  Created by Lev Trubov on 2/16/10.
//  Copyright Jirbo, Inc 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdColonyPublic.h"
#import "AdColonyAdvancedAppAppDelegate.h"

@interface AdColonyAdvancedAppViewController : UIViewController 
<AdColonyTakeoverAdDelegate, UITableViewDelegate, UITableViewDataSource, UIScrollViewDelegate>
{
	IBOutlet UILabel *timeLabel;
	int time;
	NSTimer *counter;
	BOOL paused;
	
	IBOutlet UITableView *loggingTableView;
	NSMutableArray *loggedData;
	double startingTime;
    
    IBOutlet AdColonyAdvancedAppAppDelegate *delegate;
    
    //there is now functionality to pause a video ad and then resume it
    IBOutlet UISwitch *pauseSwitch, *backgroundSwitch;
    IBOutlet UIButton *mainButton;
    BOOL adPaused;
}

@property (nonatomic, retain) NSMutableArray* loggedData;

-(void) scrollTableToEnd;
-(void) startTimer;

-(IBAction)buttonPushed:(id)sender;
-(void)startVideoAd;
-(void)pauseVideoAd;
-(void)unpauseVideoAd:(BOOL)backgrounded;

@end

