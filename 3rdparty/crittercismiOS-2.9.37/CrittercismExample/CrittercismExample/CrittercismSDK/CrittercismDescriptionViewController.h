//
//  CrittercismDescriptionViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/5/10.
//  Copyright 2010 Crittercism. All rights reserved.


#import <UIKit/UIKit.h>
#import "CrittercismAddCommentViewController.h"
#import "Crittercism.h"

@interface CrittercismDescriptionViewController : UIViewController {

	NSDictionary *feedbackDict;
	NSString *feedbackID;
	
	IBOutlet UITextView *commentTextView;
	IBOutlet UILabel *descriptionLabel;
	
	IBOutlet UILabel *titleLabel;
	IBOutlet UIImageView *categoryImage;
	IBOutlet UIImageView *avatarImage;
	IBOutlet UILabel *avatarLabel;
	IBOutlet UILabel *avatarSubmitLabel;
	IBOutlet UIButton *thumbsUpButton;
	IBOutlet UIButton *thumbsDownButton;
	
	IBOutlet UILabel *thumbsUpLabel;
	IBOutlet UILabel *thumbsDownLabel;
	
	IBOutlet UIScrollView *commentsScrollView;
	IBOutlet UIButton *feedbackBubble;
	IBOutlet UIButton *addCommentButton;
	
	IBOutlet UIButton *showVotesButton;
	
	NSMutableArray *addedComponents;
}

@property(retain) NSMutableArray *addedComponents;
@property(retain) NSDictionary *feedbackDict;
@property(nonatomic, retain) NSString *feedbackID;

@property(nonatomic, retain) IBOutlet UITextView *commentTextView;
@property(nonatomic, retain) IBOutlet UILabel *descriptionLabel;

@property(nonatomic, retain) IBOutlet UILabel *titleLabel;
@property(nonatomic, retain) IBOutlet UIImageView *categoryImage;
@property(nonatomic, retain) IBOutlet UIImageView *avatarImage;
@property(nonatomic, retain) IBOutlet UILabel *avatarLabel;
@property(nonatomic, retain) IBOutlet UILabel *avatarSubmitLabel;
@property(nonatomic, retain) IBOutlet UILabel *thumbsUpLabel;
@property(nonatomic, retain) IBOutlet UILabel *thumbsDownLabel;
@property(nonatomic, retain) IBOutlet UIButton *thumbsUpButton;
@property(nonatomic, retain) IBOutlet UIButton *thumbsDownButton;

@property(nonatomic, retain) IBOutlet UIScrollView *commentsScrollView;
@property(nonatomic, retain) IBOutlet UIButton *feedbackBubble;
@property(nonatomic, retain) IBOutlet UIButton *addCommentButton;

@property (nonatomic, retain) IBOutlet UIButton *showVotesButton;

-(id) initWithFeedbackID:(NSString *)_feedbackID;
-(void) vote:(int)isUp;
-(IBAction) thumbsUpHit:(id) sender;
-(IBAction) thumbsDownHit:(id) sender;
-(IBAction) addCommentHit:(id) sender;
-(void) refreshView;

-(void) hideVotes;
-(void) showVotesLeft;

@end