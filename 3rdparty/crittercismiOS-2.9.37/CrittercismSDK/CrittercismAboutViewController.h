//
//  CrittercismAboutViewController.h
//  CrittercismIPhone3
//
//  Created by Robert Kwok on 10/5/10.
//  Copyright 2010 Crittercism. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Crittercism.h"

@interface CrittercismAboutViewController : UIViewController <UIWebViewDelegate> {

	IBOutlet UITextField *emailField;
	IBOutlet UIButton *submitButton;
	IBOutlet UILabel *questionLabel;
	IBOutlet UIImageView *hamster;
	IBOutlet UILabel *crittercismLabel;
	IBOutlet UILabel *crittercismDescriptionLabel;
	IBOutlet UIButton *optCheckbox;
}

@property(nonatomic, retain) IBOutlet UIImageView *hamster;
@property(nonatomic, retain) IBOutlet UILabel *questionLabel;
@property(nonatomic, retain) IBOutlet UITextField *emailField;
@property(nonatomic, retain) IBOutlet UIButton *submitButton;
@property(nonatomic, retain) IBOutlet UILabel *crittercismLabel;
@property(nonatomic, retain) IBOutlet UILabel *crittercismDescriptionLabel;
@property(nonatomic, retain) IBOutlet UIButton *optCheckbox;

-(IBAction) submitHit:(id)sender;
-(IBAction) optHit:(id)sender;
-(void) showThanks;
-(void) submitEmail;
-(void) sendEmail;
-(void) saveSettings;


@end
