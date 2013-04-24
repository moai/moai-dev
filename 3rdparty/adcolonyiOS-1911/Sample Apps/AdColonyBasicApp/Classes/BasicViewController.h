//
//  BasicViewController.h
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdColonyPublic.h"

@interface BasicViewController : UIViewController <AdColonyTakeoverAdDelegate>{
	
	IBOutlet UIButton *videoButton1, *videoButton2;
    IBOutlet UILabel *zone1MessageLabel, *zone2MessageLabel;
    
    IBOutlet UIActivityIndicatorView *zone1Spinner, *zone2Spinner;

    IBOutlet UIImageView *zone1LoadingImage, *zone2LoadingImage;
}

-(IBAction) videoAdButtonPressed:(UIButton *)sender;

-(void)appRelaunched;
-(void)checkAdStatus;

-(void)videoAdsReadyCheckForZoneIndex:(NSNumber *)zoneIndex;

-(void)changeUIToSuccessForZoneIndex:(NSNumber *)zoneIndex;
-(void)changeUIToFailureForZoneIndex:(NSNumber *)zoneIndex;
-(void)changeUIForZoneIndex:(int)index toSuccess:(BOOL)success;

@end

