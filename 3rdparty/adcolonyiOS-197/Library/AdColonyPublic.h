//
//  AdColonyPublic.h
//
//  Created by Lev Trubov on 2/9/10.
//  Copyright 2010 Jirbo, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//Should be self-explanatory: used to set a parameter to determine how much information
//is logged out to the console in the course of running the ad manager and related
//advertising.

extern NSString* AdColonyLoggingOn;
extern NSString* AdColonyLoggingOff;

typedef enum {
    ADCOLONY_ZONE_STATUS_NO_ZONE = 0,
	ADCOLONY_ZONE_STATUS_OFF,
	ADCOLONY_ZONE_STATUS_LOADING,
	ADCOLONY_ZONE_STATUS_ACTIVE,
    ADCOLONY_ZONE_STATUS_UNKNOWN
} ADCOLONY_ZONE_STATUS;//*/

//so the new library can be simply dropped in without changing any implementation code

#define AdColonyAdministratorDelegate AdColonyDelegate
#define AdColonyAdministratorPublic   AdColony
#define initAdministratorWithDelegate initAdColonyWithDelegate


#pragma mark -
#pragma mark protocols
//--------********--------********--------********--------********--------********--------********--------*******

//The AdColonyDelegate protocol methods must be implemented in order to
//supply necessary configuration data about the app.
@protocol AdColonyDelegate <NSObject>

//Should return the application id provided by the AdColony website.
-(NSString *)adColonyApplicationID;

//Should return a dictionary mapping unique integer keys to zone ids.
//Provides a list of all ad zones in use throughout the app.
//Keys should be NSNumber objects with integer values (called the slot number).
//Values should be NSString objects with zone id values provided by the AdColony website.
//Do not use the same key for multiple zones.
-(NSDictionary *)adColonyAdZoneNumberAssociation;


@optional
//Should return the application version, which should be a numerical string, like @"1.1"
-(NSString *)adColonyApplicationVersion;

//Should return approrpriate constant string to determine the amount of AdColony console logging.
-(NSString *)adColonyLoggingStatus;

//Is called when the video zone is turned off or server fails to return videos
-(void)adColonyNoVideoFillInZone:(NSString *)zone;

//Is called when the video zone is ready to serve ads
-(void)adColonyVideoAdsReadyInZone:(NSString *)zone;

//is called when, temporarily or permanently, video ads have become
//unavailable for any reason
//requesting ads after this method returns and before a subsequent 
//adColonyVideoAdsReadyInZone: callback with the same zone will produce no video ads
-(void)adColonyVideoAdsNotReadyInZone:(NSString *)zone;

//Should implement any app-specific code that should be run when AdColony has successfully rewarded
//virtual currency after a video. For example, contact a game server to determine the current total of 
//virtual currency after the award.
-(void)adColonyVirtualCurrencyAwardedByZone:(NSString *)zone currencyName:(NSString *)name currencyAmount:(int)amount;

//Should implement any app-specific code that should be run when AdColony has failed to reward virtual
//currency after a video. For example, update the user interface with the results of calling 
//virtualCurrencyAwardAvailable to disable or enable launching virtual currency videos.
-(void)adColonyVirtualCurrencyNotAwardedByZone:(NSString *)zone currencyName:(NSString *)name currencyAmount:(int)amount reason:(NSString *)reason;


-(NSString *)adColonySupplementalVCParametersForZone:(NSString *)zone;
@end

//--------********--------********--------********--------********--------********--------********--------********

//Methods that should be implemented in classes that launch 
//ads that will take over the screen, either on appearance, or on click. 
@protocol AdColonyTakeoverAdDelegate <NSObject>

@optional
//Should implement any app-specific code that should be run when an ad that takes over the screen begins
//(for example, pausing a game if a video ad is being served in the middle of a session).
-(void)adColonyTakeoverBeganForZone:(NSString *)zone;

//Should implement any app-specific code that should be run when an ad that takes over the screen ends
//(for example, resuming a game if a video ad was served in the middle of a session).
-(void)adColonyTakeoverEndedForZone:(NSString *)zone withVC:(BOOL)withVirtualCurrencyAward;

//Should implement any app-specific code that should be run when AdColony is unable to play a video ad 
//or virtual currency video ad
-(void)adColonyVideoAdNotServedForZone:(NSString *)zone;

-(void)adColonyVideoAdPausedInZone:(NSString *)zone;
-(void)adColonyVideoAdResumedInZone:(NSString *)zone;

@end

#pragma mark -
#pragma mark adcolony
//--------********--------********--------********--------********--------********--------********--------********

//This class contains methods used to initialize AdColony and display all the types of
//advertisements supported by AdColony. It should be initialized with an object that implements
//the AdColonyDelegate protocol methods that supply necessary data about the app.
@interface AdColony : NSObject {
}

//This initializes AdColony's internal state, including readying video ads to be played.
//The delegate object is required for AdColony to get data from 
//the AdColonyDelegate protocol
//IMPORTANT: This method must be called before using any other AdColony methods or classes
+(void)initAdColonyWithDelegate:(id<AdColonyDelegate>)del;

//This method permanently turns off all AdColony ads. After this method is called, no ads will be played unless the app is deleted and reinstalled
+(void)turnAllAdsOff;

//Check zone status to determine whether ads are ready to be played,
//or will be ready
+(ADCOLONY_ZONE_STATUS)zoneStatusForZone:(NSString *)zoneID;
+(ADCOLONY_ZONE_STATUS)zoneStatusForSlot:(int)slotNumber;

//Calls to check if video ads for the zone are ready to be played.
//Returns YES if the video playlist has been filled and readied successfully.
//deprecated, use zoneStatusForZone: and zoneStatusForSlot: methods instead
//ADCOLONY_ZONE_STATUS_ACTIVE corresponds to a YES return value
//other status values correspond to NO
+(BOOL)didVideoFinishLoadingForZone:(NSString *)zoneID __attribute__ ((deprecated));
+(BOOL)didVideoFinishLoadingForSlot:(int)slotNumber __attribute__ ((deprecated));

//Call to check if it is possible to get a virtual currency reward for playing a video in the zone.
//Returns NO if virtual currency hasn't been configured in the AdColony.com control panel
//Returns NO if the user's daily reward cap has been reached
//Returns YES otherwise
+(BOOL)virtualCurrencyAwardAvailableForZone:(NSString *)zoneID;
+(BOOL)virtualCurrencyAwardAvailableForSlot:(int)slotNumber;

//Returns the name of the virtual currency rewarded on each video play in the zone
//The returned value will match the value set in the the adcolony.com control panel
//Note: You must first initialize AdColony using initAdColonyWithDelegate: before using this function
//Returns nil if the VC has not been set for the presenter's zone
+(NSString*)getVirtualCurrencyNameForZone:(NSString *)zoneID;
+(NSString*)getVirtualCurrencyNameForSlot:(int)slotNumber;

//Returns the amount of virtual currency rewarded on each video play in the zone
//The returned value will match the value set in the the adcolony.com control panel
//Note: You must first initialize AdColony using initAdColonyWithDelegate: before using this function
//Returns 0 if the VC has not been set for the zone
+(int)getVirtualCurrencyRewardAmountForZone:(NSString *)zoneID;
+(int)getVirtualCurrencyRewardAmountForSlot:(int)slotNumber;

//Calls to play a video ad. AdColonyTakeoverDelegate callbacks will be made to the given
//		delegate
//Note: These calls should not be attempted right after initAdColonyWithDelegate:,
//		unless used in conjunction with didVideoFinishLoadingForZone: method
//Note: whether virtual currency is awarded is determined by whether the parameter is set
//		in the adcolony web interface for the corresponding zone. In a call to play a video with VC
//		popups in the zone without virtual currency, the usePopup parameters will be ignored.
+(void)playVideoAdForZone:(NSString *)zoneID withDelegate:(id<AdColonyTakeoverAdDelegate>)del
		 withV4VCPrePopup:(BOOL)showPrePopup andV4VCPostPopup:(BOOL)showPostPopup;
+(void)playVideoAdForSlot:(int)slotNumber withDelegate:(id<AdColonyTakeoverAdDelegate>)del
		 withV4VCPrePopup:(BOOL)showPrePopup andV4VCPostPopup:(BOOL)showPostPopup;
+(void)playVideoAdForZone:(NSString *)zoneID;
+(void)playVideoAdForSlot:(int)slotNumber;
+(void)playVideoAdForZone:(NSString *)zoneID withDelegate:(id<AdColonyTakeoverAdDelegate>)del;
+(void)playVideoAdForSlot:(int)slotNumber withDelegate:(id<AdColonyTakeoverAdDelegate>)del;

//returns yes if an ad is currently running. Starting another ad when this is true is not
//advised
+(BOOL)videoAdCurrentlyRunning;

//returns the remaining number of virtual currency awards for video ads watched
//if there is no daily cap, returns -1
//if this function returns 0, no video ads will be played and no virtual currency
//awarded until the next day
+(int)virtualCurrencyAwardsAvailableTodayInZone:(NSString *)zoneID;
+(int)virtualCurrencyAwardsAvailableTodayInSlot:(int)slotNumber;

+(void)pauseVideoAdForZone:(NSString *)zoneID andGoIntoBackground:(BOOL)background;
+(void)pauseVideoAdForSlot:(int)slotNumber andGoIntoBackground:(BOOL)background;

+(void)unpauseVideoAdForZone:(NSString *)zoneID;
+(void)unpauseVideoAdForSlot:(int)slotNumber;

@end