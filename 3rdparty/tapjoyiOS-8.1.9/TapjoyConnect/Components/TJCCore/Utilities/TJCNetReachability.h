// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>


typedef enum 
{
	TJCNotReachable = 0,
	TJCReachableViaWiFi,
	TJCReachableViaWWAN,
} TJCNetworkStatus;
#define kTJCReachabilityChangedNotification @"kNetworkReachabilityChangedNotification"

/*!	\interface TJCNetReachability
 *	\brief The Tapjoy Connect Net Reachability class.
 *
 */
@interface TJCNetReachability : NSObject 
{
	BOOL localWiFiRef;
	SCNetworkReachabilityRef reachabilityRef;
}

//reachabilityWithHostName- Use to check the reachability of a particular host name. 
+ (TJCNetReachability*) reachabilityWithHostName: (NSString*) hostName;

//reachabilityWithAddress- Use to check the reachability of a particular IP address. 
+ (TJCNetReachability*) reachabilityWithAddress: (const struct sockaddr_in*) hostAddress;

//reachabilityForInternetConnection- checks whether the default route is available.  
//  Should be used by applications that do not connect to a particular host
+ (TJCNetReachability*) reachabilityForInternetConnection;

//reachabilityForLocalWiFi- checks whether a local wifi connection is available.
+ (TJCNetReachability*) reachabilityForLocalWiFi;

+ (NSString*)getReachibilityType;


/*!	\fn isUsingWifi
 *	\brief Checks whether the current device is using a wifi for its data connection.
 *
 *	\param n/a
 *	\return A boolean indicating whether the current device is using a wifi for its data connection.
 */
+ (BOOL) isUsingWifi;


/*!	\fn isUsingInternet
 *	\brief Checks whether the current device is using a cell tower for its data connection.
 *
 *	\param n/a
 *	\return A boolean indicating whether the current device is using a cell tower for its data connection.
 */
+ (BOOL) isUsingInternet;


//Start listening for reachability notifications on the current run loop
- (BOOL) startNotifier;
- (void) stopNotifier;

- (TJCNetworkStatus) currentReachabilityStatus;
//WWAN may be available, but not active until a connection has been established.
//WiFi may require a connection for VPN on Demand.
- (BOOL) connectionRequired;

@end
