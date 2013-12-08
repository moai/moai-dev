//
//  MobileAppTracker.h
//  MobileAppTracker
//
//  Created by HasOffers on 1/23/12.
//  Copyright (c) 2012 HasOffers. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MATVERSION @"2.0.1"

/*!
 MobileAppTracker provides the methods to send events and actions to the
 HasOffers servers.
 */

@protocol MobileAppTrackerDelegate;

@interface MobileAppTracker : NSObject

#pragma mark -
#pragma mark Properties

/// A Delegate used by MobileAppTracker to callback
@property (nonatomic, assign) id <MobileAppTrackerDelegate> delegate;

/*!
 Provides a view of the parameters used by the sdk at any time.
 */
@property (nonatomic, readonly) NSDictionary *sdkDataParameters;

#pragma mark -
#pragma mark Constructors

/*!
 A singleton of the MobileAppTracker Class
 */
+ (id)sharedManager;

#pragma mark -
#pragma mark Data Parameters

/** @name Advertiser Id */

/*!
 Sets the advertiser id for the engine.
 @param advertiser_id The string id for the advertiser id.
 */
- (void)setAdvertiserId:(NSString *)advertiser_id;

/** @name Advertiser Key */

/*!
 Sets the advertiser key for the engine.
 @param advertiser_key The string value for the advertiser key.
 */
- (void)setAdvertiserKey:(NSString *)advertiser_key;

/** @name Currency Code */

/*!
 Sets the currency code for the engine.
 Default: USD
 @param currency_code The string name for the currency code.
 */
- (void)setCurrencyCode:(NSString *)currency_code;

/** @name Redirect URL */

/*!
 Sets a url to be used with app to app tracking so that
 the sdk can open the download (redirect) url. This is
 used in conjunction with the SetTracking() methods.
 @param redirect_url The string name for the url.
 */
- (void)setRedirectUrl:(NSString *)redirect_url;

/** @name Device Id */

/*!
 A device id that can be used for tracking events.
 @param device_id The string name for the device id.
 */
- (void)setDeviceId:(NSString *)device_id;

/** @name User Id */

/*!
 Sets the user id for the engine.
 @param user_id The string name for the user id.
 */
- (void)setUserId:(NSString *)user_id;

/*!
 Sets the package name (bundle_id) for the engine.
 Defaults to the bundle_id of the app that is running the sdk.
 @param package_name The string name for the package.
 */
- (void)setPackageName:(NSString *)package_name;

/** @name Open UDID */

/*!
 Set the OpenUDID for the tracker.
 @param open_udid a string value for the open udid value.
 */

- (void)setOpenUDID:(NSString *)open_udid;

/** @name Truste TPID */

/*!
 Set the Trusted Preference Identifier (TPID).
 @param truste_tpid - Trusted Preference Identifier (TPID)
 */

- (void)setTrusteTPID:(NSString *)truste_tpid;

/** @name Advertiser Identifier */

/*!
 Set the Advertiser Identifier available in iOS 6
 @param advertiser_identifier - Advertiser Identifier
 */
- (void)setAdvertiserIdentifier:(NSString *)advertiser_identifier;

/*!
 Set the Vendor Identifier available in iOS 6
 @param vendor_identifier - Vendor Identifier
 */
- (void)setVendorIdentifier:(NSString * )vendor_identifier;

/** @name Cookie Tracking */

/*!
 Sets whether or not to user cookie based tracking.
 Default: NO
 @param yesorno YES/NO for cookie based tracking.
 */
- (void)setUseCookieTracking:(BOOL)yesorno;

/** @name Use HTTPS */
/*!
 Use HTTPS for the urls to the tracking engine.
 YES/NO
 @param yesorno yes means use https, default is yes.
 */
- (void)setUseHTTPS:(BOOL)yesorno;

/** @name Should Auto Generate Mac Address */

/*!
 Specifies if the sdk should auto generate a mac address identifier.
 YES/NO
 @param yesorno yes will create a mac address, defaults to yes.
 */
- (void)setShouldAutoGenerateMacAddress:(BOOL)yesorno;

/** @name Should Auto Generate ODIN1 Key */
/*!
 Specifies if the sdk should auto generate an ODIN-1 key.
 YES/NO
 @param yesorno yes will create an ODIN-1 key, defaults to yes.
 */
- (void)setShouldAutoGenerateODIN1Key:(BOOL)yesorno;

/** @name Should Auto Generate Open UDID Key */
/*!
 Specifies if the sdk should auto generate an Open UDID key.
 YES/NO
 @param yesorno yes will create an ODIN-1 key, defaults to yes.
 */
- (void)setShouldAutoGenerateOpenUDIDKey:(BOOL)yesorno;

/** @name Should Auto Generate Vendor Identifier */
/*!
 Specifies if the sdk should pull a vendor identifier from the device.
 YES/NO
 @param yesorno yes will set the vendor identifier key, defaults to no.
 */
- (void)setShouldAutoGenerateVendorIdentifier:(BOOL)yesorno;

/** @name Should Auto Generate Advertiser Identifier */
/*!
 Specifies if the sdk should pull a advertiser identifier from the device.
 YES/NO
 @param yesorno yes will set the advertiser identifier key, defaults to no.
 */
- (void)setShouldAutoGenerateAdvertiserIdentifier:(BOOL)yesorno;

/** @name Set this to see debug output from server requests */
/*!
 Specifies that the server responses should log to the console. NOTE: You must turn this off for release builds.
 If set to YES, debug data will be sent to the console.
 NOTE: This is for testing, turn off for release.
 @param yesorno defaults to NO.
 */
- (void)setShouldDebugResponseFromServer:(BOOL)yesorno;

/** @name Set this to allow duplicate requests to the tracking engine */
/*!
 Set to YES to allow duplicate requests to be registered with the tracking engine.
 NOTE: This is for testing, turn off for release.
 @param yesorno defaults to NO.
 */
- (void)setShouldAllowDuplicateRequests:(BOOL)yesorno;

#pragma mark -
#pragma mark Main Initializer

/** @name Intitializing MobileAppTracker with Advertiser Information */

/*!
 Starts Mobile App Tracker with Advertiser Id and Advertiser Key. Both values are required.
 @param aid the Advertiser Id provided in Mobile App Tracking.
 @param key the Advertiser Key provided in Mobile App Tracking.
 @param error optional, use this to view an error in starting mobile app tracker.
 */

- (void)startTrackerWithAdvertiserId:(NSString *)aid advertiserKey:(NSString *)key withError:(NSError **)error;

#pragma mark -
#pragma mark Track Actions

/** @name Track Actions */

/*!
 Record a Track Action for an Event Id or Name.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId;

/*!
 Record a Track Action for an Event Id or Name, revenue and currency.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param revenueAmount The revenue amount for the event.
 @param currencyCode The currency code override for the event. Blank defaults to sdk setting.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                      revenueAmount:(NSString *)revenueAmount
                       currencyCode:(NSString *)currencyCode;

/*!
 Record a Track Action for an Event Id or Name and reference id.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param refId The referencId for an event.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                        referenceId:(NSString *)refId;

/*!
 Record a Track Action for an Event Id or Name and reference id, revenue and currency.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param refId The referencId for an event.
 @param revenueAmount The revenue amount for the event.
 @param currencyCode The currency code override for the event. Blank defaults to sdk setting.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                        referenceId:(NSString *)refId
                      revenueAmount:(NSString *)revenueAmount
                       currencyCode:(NSString *)currencyCode;
/*!
 Record a Track Action for an Event Id or Name and a list of event items.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param eventItems The event items list. Valid dictionary item elements are: item, unit_price, quantity, revenue
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                         eventItems:(NSArray *)eventItems;

/*!
 Record a Track Action for an Event Name or Id.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param eventItems The event items list. Valid dictionary item elements are: item, unit_price, quantity, revenue
 @param revenueAmount The revenue amount for the event.
 @param currencyCode The currency code override for the event. Blank defaults to sdk setting.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                         eventItems:(NSArray *)eventItems
                      revenueAmount:(NSString *)revenueAmount
                       currencyCode:(NSString *)currencyCode;

/*!
 Record a Track Action for an Event Name or Id.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param eventItems The event items list. Valid dictionary item elements are: item, unit_price, quantity, revenue
 @param refId The referencId for an event.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                         eventItems:(NSArray *)eventItems
                        referenceId:(NSString *)refId;

/*!
 Record a Track Action for an Event Name or Id.
 @param eventIdOrName The event name or event id.
 @param isId Yes if the event is an Id otherwise No if the event is a name only.
 @param eventItems The event items list. Valid dictionary item elements are: item, unit_price, quantity, revenue
 @param refId The referencId for an event.
 @param revenueAmount The revenue amount for the event.
 @param currencyCode The currency code override for the event. Blank defaults to sdk setting.
 */
- (void)trackActionForEventIdOrName:(NSString *)eventIdOrName
                          eventIsId:(BOOL)isId
                         eventItems:(NSArray *)eventItems
                        referenceId:(NSString *)refId
                      revenueAmount:(NSString *)revenueAmount
                       currencyCode:(NSString *)currencyCode;

#pragma mark -
#pragma mark Track Update Methods

/** @name Track Installs and/or Updates */

/*!
 Record a Track Update or Install
 Called when an app opens typically in the didFinishLaunching event. If
 updateOnly is YES, then the sdk will never record an install. The SDK will
 always record updates to an app in any case.
 @param updateOnly only record udpates and no installs.
 */
- (void)trackInstallWithUpdateOnly:(BOOL)updateOnly;

/*!
 Record a Track Update or Install
 Called when an app opens typically in the didFinishLaunching event. If
 updateOnly is YES, then the sdk will never record an install. The SDK will
 always record updates to an app in any case.
 @param updateOnly only record udpates and no installs.
 @param refId A reference id used to track an install and/or update.
 */
- (void)trackInstallWithUpdateOnly:(BOOL)updateOnly
                       referenceId:(NSString *)refId;

#pragma mark -
#pragma mark Track Setters

/** @name Start a Tracking Session used with App To App Tracking */

/*!
 Start a Tracking Session for a Target App Id
 @param targetAppId The string name for the app id.
 @param shouldRedirect Should redirect to the download url.
 */
- (void)setTracking:(NSString*)targetAppId redirect:(BOOL)shouldRedirect;

/*!
 Start a Tracking Session a Target App Id and a publisher id.
 @param targetAppId The string name for the app id.
 @param advertiserId The string name for a advertiser id.
 @param shouldRedirect Should redirect to the download url.
 */
- (void)setTracking:(NSString*)targetAppId advertiserId:(NSString*)advertiserId redirect:(BOOL)shouldRedirect;

/*!
 Start a Tracking Session a Target App Id and a campaign id.
 @param targetAppId The string name for a target app id.
 @param campaignId The string name for the campaign.
 @param shouldRedirect Should redirect to the download url.
 */
- (void)setTracking:(NSString*)targetAppId campaignId:(NSString*)campaignId redirect:(BOOL)shouldRedirect;

/*!
 Start a Tracking Session a Target App Id and a publisher id and campaign id.
 @param targetAppId The string name for a target app id.
 @param advertiserId The string name for a advertiser id.
 @param campaignId The string name for the campaign.
 @param publisherId The string nmame for a publisher id.
 @param shouldRedirect Should redirect to the download url.
 */
- (void)setTracking:(NSString*)targetAppId
       advertiserId:(NSString*)advertiserId
         campaignId:(NSString*)campaignId
        publisherId:(NSString *)publisherId
           redirect:(BOOL)shouldRedirect;


#pragma mark -
#pragma mark Re-Engagement Method

/** @name Application Re-Engagement */

/*!
 Record the URL and Source when an application is opened via a URL scheme.
 This typically occurs during OAUTH or when an app exits and is returned
 to via a URL. The data will be sent to the HasOffers server so that a
 Re-Engagement can be recorded.
 @param urlString the url string used to open your app.
 @param sourceApplication the source used to open your app. For example, mobile safari.
 */
- (void)applicationDidOpenURL:(NSString *)urlString sourceApplication:(NSString *)sourceApplication;

@end


/** @name MobileAppTrackerDelegate */

/*!
 Protocol that allows for callbacks from the MobileAppTracker methods.
 To use, implement the delegate in your app and implement these methods.
 */
@protocol MobileAppTrackerDelegate <NSObject>
@optional
- (void)mobileAppTracker:(MobileAppTracker *)tracker didSucceedWithData:(id)data;
- (void)mobileAppTracker:(MobileAppTracker *)tracker didFailWithError:(NSError *)error;
@end

