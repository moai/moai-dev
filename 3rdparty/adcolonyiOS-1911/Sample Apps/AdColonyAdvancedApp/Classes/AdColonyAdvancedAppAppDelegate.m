//
//  AdColonyAdvancedAppAppDelegate.m
//  AdColonyAdvancedApp
//
//  Created by Lev Trubov on 2/16/10.
//  Copyright Jirbo, Inc 2010. All rights reserved.
//

#import "AdColonyAdvancedAppAppDelegate.h"
#import "AdColonyAdvancedAppViewController.h"

static BOOL onIPad = NO;
static BOOL onIPadDetermined = NO;

@implementation AdColonyAdvancedAppAppDelegate

@synthesize window;
@synthesize navController, viewController;
@synthesize sm;


+(BOOL)runningOnIPad{
	if(!onIPadDetermined){
		if([[UIDevice currentDevice] respondsToSelector:@selector(userInterfaceIdiom)]){
			if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad){
				onIPad = YES;
			}
		}
		onIPadDetermined = YES;
	}
	
	return onIPad;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application { 
	//set to no if you want to skip the prestitial video
	shouldPlayPrestitial = YES;
	justEnteredForeground = NO;
    
	//Registering default values
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSMutableDictionary *defaultValues = [NSMutableDictionary dictionary];
	[defaultValues setObject:[NSNumber numberWithBool:YES] forKey:@"musicOn"];
	[defaultValues setObject:[NSNumber numberWithFloat:0.5f] forKey:@"iconSize"];
	[defaults registerDefaults:defaultValues];
	
	NSString *imageFile = @"Default.png";
	if([AdColonyAdvancedAppAppDelegate runningOnIPad]){
		imageFile = @"Default-iPad.png";
	}
	defaultReplacement = [[UIImageView alloc] initWithImage:[UIImage imageNamed:imageFile]];
	defaultReplacement.center = CGPointMake(window.bounds.size.width/2, window.bounds.size.height/2);
	
	//Starting up background music
    sm = [[SoundManager alloc] init];
    
	sm.shouldPlayMusicDefaultsKey = @"musicOn";
	NSString *musicTrackFile = [[NSBundle mainBundle] pathForResource:@"bgmusicloop.mp3" ofType:nil];
	[sm loadMusicTrackWithFile:musicTrackFile repeats:-1];
	if(!shouldPlayPrestitial){
		[sm playMusic];
	}
	
	
	// Set up the logging on main screen
	viewController.loggedData = [NSMutableArray arrayWithCapacity:10];
	[[NSNotificationCenter defaultCenter] addObserver:viewController
											 selector:@selector(newIncomingLogStatement:)
												 name:@"newLogStatement"
											   object:nil];	
	
	// initialize AdColony and time it
	NSDate* preTime = [NSDate date];
    [AdColonyAdministratorPublic initAdministratorWithDelegate:self];
	NSDate* postTime = [NSDate date];
	NSTimeInterval initTime = [postTime timeIntervalSinceDate: preTime];
	logMessage([NSString stringWithFormat:@"AdColony initialized in %1.3f seconds", initTime]);
	
	// time how long it takes to load the videos
	mightHaveFill = YES;
    
	//Check if the video playlist has loaded
	[NSThread detachNewThreadSelector:@selector(loadCheck) toTarget:self withObject:nil];
	
	[window addSubview:navController.view];	
	if(shouldPlayPrestitial){
		[window addSubview:defaultReplacement];
		[self performSelector:@selector(videoin) withObject:nil afterDelay:2.0];
	}
	[window makeKeyAndVisible];
}

- (void)applicationWillEnterForeground:(UIApplication *)application{
    if(shouldPlayPrestitial){
        if(![AdColonyAdministratorPublic videoAdCurrentlyRunning]){
            justEnteredForeground = YES;
        }
    }
}

-(void)applicationDidBecomeActive:(UIApplication *)application{
    if(justEnteredForeground){
        justEnteredForeground = NO;
        [AdColonyAdministratorPublic playVideoAdForSlot:1 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:NO];
    }
}


/* loadCheck does not run on the main thread. This method periodically pings
 the adcolony library to check if the video playlist has loaded. Waits for
 10 seconds and returns if the videos have still not loaded. Use this method
 for a pre-stitial video.
 */
- (void)loadCheck
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	logMessage(@"Preparing video ads");
	NSDate* preTime = [NSDate date];
	int repetitions = 1000;
	while (repetitions > 0 && mightHaveFill) {
		--repetitions;
        ADCOLONY_ZONE_STATUS s = [AdColony zoneStatusForSlot:1];
        if(s != ADCOLONY_ZONE_STATUS_ACTIVE){
            [NSThread sleepForTimeInterval:0.1f];
			continue;
        }else{
            NSDate* postTime = [NSDate date];
			NSTimeInterval loadTime = [postTime timeIntervalSinceDate:preTime];
			logMessage([NSString stringWithFormat:@"Video ads readied in %1.3f seconds", loadTime]);
			break;
        }
	}
    
    if([AdColony zoneStatusForSlot:1] != ADCOLONY_ZONE_STATUS_ACTIVE){
		logMessage(@"Server unable to provide video ads");
	}
	[pool release];
}

- (void)videoin
{
	isPlayingPrestitial = YES;
    [AdColonyAdministratorPublic playVideoAdForSlot:1 withDelegate:self withV4VCPrePopup:NO andV4VCPostPopup:NO];
}


- (void)dealloc {
	[defaultReplacement release];
	[viewController release];
	[navController release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark -
#pragma mark AdColonyAdministratorDelegate
#pragma mark -

#pragma mark app and zone info
//include the KEY provided by adcolony.com to the publisher
//Return appid provided by adcolony.com
-(NSString *)adColonyApplicationID{
	static BOOL doOnce = YES;
	if (doOnce) {
		logMessage(@"AdColony retrieved application ID");
		doOnce = NO;
	}
	return @"app4e541454aa92e";
}


//A dictionary associating each zone id used within the app with a number
//Provides a list of all ad zones used by the app
//If keys used are NSNumber objects with integer values, ads for specific zones are possible
//to initiate with that number (called the slot number) instead of directly by zone id
//See the -(id)initWithNoNibForSlot:(int)slotNumber series of methods below
//Entries into dictionary should be of form key=[NSNumber numberWithInt:anInteger], value=zoneID
//Do not use the same key for multiple zones

-(NSDictionary *)adColonyAdZoneNumberAssociation{
	static BOOL doOnce = YES;
	if (doOnce) {
		logMessage(@"AdColony retrieved ad zones");
		doOnce = NO;
	}

    //*
    return [NSDictionary dictionaryWithObjectsAndKeys:
            @"z4e54154dd2d35", [NSNumber numberWithInt:1],  //prestitial
            @"z4e5415e6f0673", [NSNumber numberWithInt:2],  //when tapping in-app button
            nil];//*/
    
}

//Return the application version. Currently 1.0
-(NSString *)adColonyApplicationVersion{
	NSString* appVersion = @"1.1";
	static BOOL doOnce = YES;
	if (doOnce) {
		logMessage([NSString stringWithFormat:@"AdColony retrieved application version %@", appVersion]);
		doOnce = NO;
	}
	return appVersion;
}

-(NSString *)adColonyLoggingStatus{
	return AdColonyLoggingOn;
}

#pragma mark fill info
//Callbacks from the Video Administrator
// with information about video fill 

-(void)adColonyNoVideoFillInZone:(NSString *)zone{
    NSLog(@"AdColony: videos unavailable in zone: %@", zone);
}

-(void)adColonyVideoAdsReadyInZone:(NSString *)zone{
    NSLog(@"AdColony: videos available in zone: %@", zone);
}

-(void)adColonyVideoAdsNotReadyInZone:(NSString *)zone{
    NSLog(@"AdColony: videos temporarily unavailable in zone: %@", zone);

}


#pragma mark -
#pragma mark -
#pragma mark AdColonyTakeoverDelegate
#pragma mark -

//Should implement any app-specific code that should be run when an ad that takes over the screen is launched
//(for example, pausing a game if the ad is in the middle of a session).
-(void)adColonyTakeoverBeganForZone:(NSString *)zone{
	NSLog(@"app delegate: ad launched in zone %@", zone);
    [sm pauseMusic];
}

//Should implement any app-specific code that should be run when an ad that takes over the screen is quit
//(for example, resuming a game if the ad was in the middle of a session).
-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward{
	
    [sm playMusic];
	NSLog(@"app delegate: ad gone in zone %@", zone);
	if(isPlayingPrestitial){
		isPlayingPrestitial = NO;
        if(defaultReplacement.superview){
            [defaultReplacement removeFromSuperview];
        }
	}
}

//Should implement any app-specific code that should be run when a video ad has failed to deliver
-(void)adColonyVideoAdNotServedForZone:(NSString *)zone{
	NSLog(@"app delegate: failed to load ad in zone %@", zone);
	[sm playMusic];
	if(isPlayingPrestitial){
		isPlayingPrestitial = NO;
        if(defaultReplacement.superview){
            [defaultReplacement removeFromSuperview];
        }
	}
}
@end
