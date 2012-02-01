//
//  CrittercismAddCommentViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/12/10.
//  Copyright 2010 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "CrittercismUIAlertPrompt.h"

@interface CrittercismAddCommentViewController : UIViewController {

	IBOutlet UITextView *commentTextView;
	
	NSString *feedbackID;
	NSString *parentID;
}

@property(nonatomic, retain) NSString *parentID;
@property(nonatomic, retain) NSString *feedbackID;
@property(nonatomic, retain) IBOutlet UITextView *commentTextView;

-(void) saveHit;
-(id) initWithFeedbackID:(NSString *)_feedbackID andParentID:(NSString *)_parentID;


@end
