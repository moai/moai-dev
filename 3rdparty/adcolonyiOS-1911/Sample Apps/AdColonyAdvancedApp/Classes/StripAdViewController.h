//
//  StripAdViewController.h
//
//  Created by Lev Trubov on 3/1/10.
//  Copyright 2010 Jirbo, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdColonyPublic.h"


@interface StripAdViewController : UIViewController <AdColonyTakeoverAdDelegate>{
	IBOutlet UILabel *timeLabel;
	int time;
	NSTimer *counter;
	BOOL paused;
}

-(IBAction)back;
-(void)timerFunction;


@end
