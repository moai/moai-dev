//
//  BasicViewController.m
//  Basic
//
//  Created by John Fernandes-Salling on 10/29/10.
//  Copyright Jirbo, INC. 2010. All rights reserved.
//

#define kZoneSuccessfullyLoadedString   @"Zone %d loaded. Tap the button to watch a video ad."
#define kZoneFailedToLoadString         @"Zone %d failed to load. Button is disabled."

#import "BasicViewController.h"

@implementation BasicViewController

#pragma mark -
#pragma mark AdColonyTakeoverAdDelegate

//app interrupted: called when video is played or
//banner ad is expanded
-(void)adColonyTakeoverBeganForZone:(NSString *)zone{
	NSLog(@"Video ad launched for zone %@", zone);
}

//app interruption over: called when video ad or expanded
//banner ad is dismissed
-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward{
	NSLog(@"Video ad finished in zone %@", zone);
}

#pragma mark -
#pragma mark This action is tied to the button that launches video ads

-(IBAction) videoAdButtonPressed:(UIButton *)sender{
	//launch the video ad for the app's video zone
    if([sender isEqual:videoButton1]){
        [AdColonyAdministratorPublic playVideoAdForSlot:1 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:NO];
    }else if([sender isEqual:videoButton2]){
        [AdColonyAdministratorPublic playVideoAdForSlot:2 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:NO];
    }
}


#pragma mark overridden

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    if(&UIApplicationWillEnterForegroundNotification != NULL){
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appRelaunched) name:UIApplicationWillEnterForegroundNotification object:nil];
    }
     
    
    [self checkAdStatus];
    
}

- (void)dealloc {
	[videoButton1 release];
    [videoButton2 release];
    
    [zone1MessageLabel release];
    [zone2MessageLabel release];
    [zone1Spinner release];
    [zone2Spinner release];
    
    [zone1LoadingImage release];
    [zone2LoadingImage release];
    [super dealloc];
}

#pragma mark ad readiness

-(void)appRelaunched{
    zone1LoadingImage.image = nil;
    zone2LoadingImage.image = nil;
    
    [self checkAdStatus];
}

-(void)checkAdStatus{
    ADCOLONY_ZONE_STATUS z1Status = [AdColony zoneStatusForSlot:1];
    if(z1Status == ADCOLONY_ZONE_STATUS_NO_ZONE ||
       z1Status == ADCOLONY_ZONE_STATUS_OFF ||
       z1Status == ADCOLONY_ZONE_STATUS_UNKNOWN){
        [self changeUIForZoneIndex:1 toSuccess:NO];
    }else if(z1Status == ADCOLONY_ZONE_STATUS_ACTIVE){
        [self changeUIForZoneIndex:1 toSuccess:YES];
    }else{
        //*
        videoButton1.enabled = NO;
        [zone1Spinner startAnimating];
        [NSThread detachNewThreadSelector:@selector(videoAdsReadyCheckForZoneIndex:) toTarget:self withObject:[NSNumber numberWithInt:1]];//*/
    }
    
    ADCOLONY_ZONE_STATUS z2Status = [AdColony zoneStatusForSlot:2];
    if(z2Status == ADCOLONY_ZONE_STATUS_NO_ZONE ||
       z2Status == ADCOLONY_ZONE_STATUS_OFF ||
       z2Status == ADCOLONY_ZONE_STATUS_UNKNOWN){
        [self changeUIForZoneIndex:2 toSuccess:NO];
    }else if(z2Status == ADCOLONY_ZONE_STATUS_ACTIVE){
        [self changeUIForZoneIndex:2 toSuccess:YES];
    }else{
        //*
        videoButton2.enabled = NO;
        [zone2Spinner startAnimating];
        [NSThread detachNewThreadSelector:@selector(videoAdsReadyCheckForZoneIndex:) toTarget:self withObject:[NSNumber numberWithInt:2]];//*/
    }
}

-(void)videoAdsReadyCheckForZoneIndex:(NSNumber *)zoneIndex{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    const float checkEvery = 0.1f; //seconds
    const int maxWait = 1000;      //time out after 100 sec
    
    BOOL zoneReady = ([AdColony zoneStatusForSlot:[zoneIndex intValue]] == ADCOLONY_ZONE_STATUS_ACTIVE);
    
    int cycles = 0;
    while (!zoneReady && cycles < maxWait) {
        [NSThread sleepForTimeInterval:checkEvery];
        //zoneReady = [AdColonyAdministratorPublic didVideoFinishLoadingForSlot:[zoneIndex intValue]];
        zoneReady = ([AdColony zoneStatusForSlot:[zoneIndex intValue]] == ADCOLONY_ZONE_STATUS_ACTIVE);
        cycles++;
    }
    
    SEL resultAction;
    
    if(cycles < maxWait){
        resultAction = @selector(changeUIToSuccessForZoneIndex:);
    }else{
        resultAction = @selector(changeUIToFailureForZoneIndex:);
    }
    
    [self performSelectorOnMainThread:resultAction withObject:zoneIndex waitUntilDone:NO];
    
    [pool release];
}

-(void)changeUIToSuccessForZoneIndex:(NSNumber *)zoneIndex{
    [self changeUIForZoneIndex:[zoneIndex intValue] toSuccess:YES];
}

-(void)changeUIToFailureForZoneIndex:(NSNumber *)zoneIndex{
    [self changeUIForZoneIndex:[zoneIndex intValue] toSuccess:NO];
}

-(void)changeUIForZoneIndex:(int)index toSuccess:(BOOL)success{
    UIImageView *icon;
    UILabel *label;
    UIButton *button;
    UIActivityIndicatorView *spinner;
    if(index == 1){
        icon = zone1LoadingImage;
        label = zone1MessageLabel;
        button = videoButton1;
        spinner = zone1Spinner;
    }else{
        icon = zone2LoadingImage;
        label = zone2MessageLabel;
        button = videoButton2;
        spinner = zone2Spinner;
    }
    
    [spinner stopAnimating];
    
    NSString *message;
    UIImage *image;
    if(success){
        message = [NSString stringWithFormat:kZoneSuccessfullyLoadedString, index];
        image = [UIImage imageNamed:@"check.png"];
        button.enabled = YES;
    }else{
        message = [NSString stringWithFormat:kZoneFailedToLoadString, index];
        image = [UIImage imageNamed:@"error.png"];
        button.enabled = NO;
    }
    
    label.text = message;
    icon.image = image;
}

@end
