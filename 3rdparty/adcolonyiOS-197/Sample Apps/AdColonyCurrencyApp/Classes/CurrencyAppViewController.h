//
//  CurrencyAppViewController.h
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdColonyPublic.h"
#import "CurrencyManager.h"

@interface CurrencyAppViewController : UIViewController <AdColonyTakeoverAdDelegate, CurrencyManagerDelegate, UITextFieldDelegate>{
	IBOutlet UIButton *videoButton;
	IBOutlet UILabel *virtualCurrencyLabel1, *virtualCurrencyLabel2;
	IBOutlet UISwitch *videoOptInSwitch;
	IBOutlet UILabel *appInfoLabel;
	
	BOOL userOptedIn;
	BOOL navigationVideoPlayed;
    
    CurrencyManager *cm;
    IBOutlet UITextField *extraParameterField;
    BOOL onIPadDetermined, onIPad;
}

@property(nonatomic, assign) CurrencyManager *cm;

-(IBAction) videoAdButtonPressed;
-(IBAction) navigationButtonPressed;
-(IBAction) videoOptInSwitchToggled;

-(void)videoAdsReadyCheck;
-(void)enableVideoButton;


-(void) performNavigation;
-(void) adjustUserInfoLabel;

-(NSString *)extraParameterText;

-(void)keyboardAppeared:(NSNotification *)notification;
-(void)keyboardGone:(NSNotification *)notification;

@end

