//
//  CrittercismAddViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/5/10.
//  Copyright 2010 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "CrittercismUIAlertPrompt.h"
#import "CrittercismDescriptionViewController.h"
#import "CrittercismJSONKit.h"

@class Crittercism;

@interface CrittercismAddViewController : UIViewController<UIAlertViewDelegate> {

	NSString *feedbackTitle;
	NSString *category;
	
	IBOutlet UIImageView *backgroundImage;
	IBOutlet UILabel *titleLabel;
	IBOutlet UITextField *titleTextField;
	IBOutlet UILabel *nameLabel;
	IBOutlet UITextField *nameTextField;
	IBOutlet UILabel *descriptionLabel;
	IBOutlet UITextView *descriptionTextView;
	
	IBOutlet UIButton *ideaToggle;
	IBOutlet UIButton *bugToggle;
	IBOutlet UIButton *loveToggle;	
	IBOutlet UIButton *questionToggle;
    
    IBOutlet UIButton *_categoryButton;
	
	IBOutlet UIActivityIndicatorView *indicator;
	IBOutlet UIButton *submittingButton;
    IBOutlet UIScrollView *addScrollView;
	
}

@property(nonatomic, retain) NSString *feedbackTitle;
@property(nonatomic, retain) NSString *category;

@property(nonatomic, retain) IBOutlet UIImageView *backgroundImage;
@property(nonatomic, retain) IBOutlet UILabel *titleLabel;
@property(nonatomic, retain) IBOutlet UITextField *titleTextField;
@property(nonatomic, retain) IBOutlet UILabel *nameLabel;
@property(nonatomic, retain) IBOutlet UITextField *nameTextField;
@property(nonatomic, retain) IBOutlet UILabel *descriptionLabel;
@property(nonatomic, retain) IBOutlet UITextView *descriptionTextView;

@property (nonatomic, retain) IBOutlet UIButton *ideaToggle;
@property (nonatomic, retain) IBOutlet UIButton *bugToggle;
@property (nonatomic, retain) IBOutlet UIButton *loveToggle;
@property (nonatomic, retain) IBOutlet UIButton *questionToggle;
@property (nonatomic, retain) IBOutlet UIButton *categoryButton;

@property(nonatomic, retain) IBOutlet UIActivityIndicatorView *indicator;
@property(nonatomic, retain) IBOutlet UIButton *submittingButton;
@property(nonatomic, retain) IBOutlet UIScrollView *addScrollView;

-(void) saveFeedback;
-(id) initWithTitle:(NSString *)_feedbackTitle andCategory:(NSString *)_category;
-(void) showWorking;
-(void) showErrorMsg;
-(void) didRotate:(NSNotification *)notification;

-(IBAction) bugHit:(id) sender;
-(IBAction) ideaHit:(id) sender;
-(IBAction) loveHit:(id) sender;
-(IBAction) questionHit:(id) sender;

@end
