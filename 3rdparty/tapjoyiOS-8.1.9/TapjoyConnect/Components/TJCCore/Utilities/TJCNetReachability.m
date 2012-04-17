// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <sys/socket.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#import <netdb.h>
#import <CoreFoundation/CoreFoundation.h>
#import "TJCNetReachability.h"


#define kShouldPrintReachabilityFlags 0

static void PrintReachabilityFlags(SCNetworkReachabilityFlags    flags, const char* comment)
{
#if kShouldPrintReachabilityFlags
	
    NSLog(@"TJCNetReachability Flag Status: %c%c %c%c%c%c%c%c%c %s\n",
		  (flags & kSCNetworkReachabilityFlagsIsWWAN)				? 'W' : '-',
		  (flags & kSCNetworkReachabilityFlagsReachable)            ? 'R' : '-',
		  (flags & kSCNetworkReachabilityFlagsTransientConnection)  ? 't' : '-',
		  (flags & kSCNetworkReachabilityFlagsConnectionRequired)   ? 'c' : '-',
		  (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic)  ? 'C' : '-',
		  (flags & kSCNetworkReachabilityFlagsInterventionRequired) ? 'i' : '-',
		  (flags & kSCNetworkReachabilityFlagsConnectionOnDemand)   ? 'D' : '-',
		  (flags & kSCNetworkReachabilityFlagsIsLocalAddress)       ? 'l' : '-',
		  (flags & kSCNetworkReachabilityFlagsIsDirect)             ? 'd' : '-',
		  comment
		  );
#endif
}


@implementation TJCNetReachability


static void ReachabilityCallback(SCNetworkReachabilityRef target, SCNetworkReachabilityFlags flags, void* info)
{
#pragma unused (target, flags)
	NSCAssert(info != NULL, @"info was NULL in ReachabilityCallback");
	NSCAssert([(NSObject*) info isKindOfClass: [TJCNetReachability class]], @"info was wrong class in ReachabilityCallback");
	
	//We're on the main RunLoop, so an NSAutoreleasePool is not necessary, but is added defensively
	// in case someon uses the Reachablity object in a different thread.
	NSAutoreleasePool* myPool = [[NSAutoreleasePool alloc] init];
	
	TJCNetReachability* noteObject = (TJCNetReachability*) info;
	// Post a notification to notify the client that the network reachability changed.
	[[NSNotificationCenter defaultCenter] postNotificationName: kTJCReachabilityChangedNotification object: noteObject];
	
	[myPool release];
}

- (BOOL) startNotifier
{
	BOOL retVal = NO;
	SCNetworkReachabilityContext	context = {0, self, NULL, NULL, NULL};
	if(SCNetworkReachabilitySetCallback(reachabilityRef, ReachabilityCallback, &context))
	{
		if(SCNetworkReachabilityScheduleWithRunLoop(reachabilityRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode))
		{
			retVal = YES;
		}
	}
	return retVal;
}

- (void) stopNotifier
{
	if(reachabilityRef!= NULL)
	{
		SCNetworkReachabilityUnscheduleFromRunLoop(reachabilityRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	}
}

- (void) dealloc
{
	[self stopNotifier];
	if(reachabilityRef!= NULL)
	{
		CFRelease(reachabilityRef);
	}
	[super dealloc];
}

+ (TJCNetReachability*) reachabilityWithHostName: (NSString*) hostName;
{
	TJCNetReachability* retVal = NULL;
	SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, [hostName UTF8String]);
	if(reachability!= NULL)
	{
		retVal= [[[self alloc] init] autorelease];
		if(retVal!= NULL)
		{
			retVal->reachabilityRef = reachability;
			retVal->localWiFiRef = NO;
		}
	}
	return retVal;
}

+ (TJCNetReachability*) reachabilityWithAddress: (const struct sockaddr_in*) hostAddress;
{
	SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (const struct sockaddr*)hostAddress);
	TJCNetReachability* retVal = NULL;
	if(reachability!= NULL)
	{
		retVal= [[[self alloc] init] autorelease];
		if(retVal!= NULL)
		{
			retVal->reachabilityRef = reachability;
			retVal->localWiFiRef = NO;
		}
	}
	return retVal;
}

+ (TJCNetReachability*) reachabilityForInternetConnection;
{
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	return [self reachabilityWithAddress: &zeroAddress];
}

+ (TJCNetReachability*) reachabilityForLocalWiFi;
{
	struct sockaddr_in localWifiAddress;
	bzero(&localWifiAddress, sizeof(localWifiAddress));
	localWifiAddress.sin_len = sizeof(localWifiAddress);
	localWifiAddress.sin_family = AF_INET;
	// IN_LINKLOCALNETNUM is defined in <netinet/in.h> as 169.254.0.0
	localWifiAddress.sin_addr.s_addr = htonl(IN_LINKLOCALNETNUM);
	TJCNetReachability* retVal = [self reachabilityWithAddress: &localWifiAddress];
	if(retVal!= NULL)
	{
		retVal->localWiFiRef = YES;
	}
	return retVal;
}


+ (BOOL) isUsingWifi
{
	TJCNetworkStatus wifiStatus = [[TJCNetReachability reachabilityForLocalWiFi] currentReachabilityStatus];
	
    if (wifiStatus == TJCNotReachable)
	{
		return FALSE;
	}
	
	return TRUE;
}


+ (BOOL) isUsingInternet
{
	TJCNetworkStatus internetStatus = [[TJCNetReachability reachabilityForInternetConnection] currentReachabilityStatus];
	
    if (internetStatus == TJCNotReachable)
	{
		return FALSE;
	}
	
	return TRUE;
}


+ (NSString*)getReachibilityType
{
	if ([TJCNetReachability isUsingWifi])
	{
		return @"wifi";
	}
	
	if ([TJCNetReachability isUsingInternet])
	{
		return @"mobile";
	}
	
	return @"none";
}



#pragma mark Network Flag Handling

- (TJCNetworkStatus) localWiFiStatusForFlags: (SCNetworkReachabilityFlags) flags
{
	PrintReachabilityFlags(flags, "localWiFiStatusForFlags");
	
	BOOL retVal = TJCNotReachable;
	if((flags & kSCNetworkReachabilityFlagsReachable) && (flags & kSCNetworkReachabilityFlagsIsDirect))
	{
		retVal = TJCReachableViaWiFi;	
	}
	return retVal;
}

- (TJCNetworkStatus) networkStatusForFlags: (SCNetworkReachabilityFlags) flags
{
	PrintReachabilityFlags(flags, "networkStatusForFlags");
	if ((flags & kSCNetworkReachabilityFlagsReachable) == 0)
	{
		// if target host is not reachable
		return TJCNotReachable;
	}
	
	BOOL retVal = TJCNotReachable;
	
	if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0)
	{
		// if target host is reachable and no connection is required
		//  then we'll assume (for now) that your on Wi-Fi
		retVal = TJCReachableViaWiFi;
	}
	
	
	if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) ||
		 (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0))
	{
		// ... and the connection is on-demand (or on-traffic) if the
		//     calling application is using the CFSocketStream or higher APIs
		
		if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0)
		{
			// ... and no [user] intervention is needed
			retVal = TJCReachableViaWiFi;
		}
	}
	
	if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN)
	{
		// ... but WWAN connections are OK if the calling application
		//     is using the CFNetwork (CFSocketStream?) APIs.
		retVal = TJCReachableViaWWAN;
	}
	return retVal;
}

- (BOOL) connectionRequired;
{
	NSAssert(reachabilityRef != NULL, @"connectionRequired called with NULL reachabilityRef");
	SCNetworkReachabilityFlags flags;
	if (SCNetworkReachabilityGetFlags(reachabilityRef, &flags))
	{
		return (flags & kSCNetworkReachabilityFlagsConnectionRequired);
	}
	return NO;
}

- (TJCNetworkStatus) currentReachabilityStatus
{
	NSAssert(reachabilityRef != NULL, @"currentNetworkStatus called with NULL reachabilityRef");
	TJCNetworkStatus retVal = TJCNotReachable;
	SCNetworkReachabilityFlags flags;
	if (SCNetworkReachabilityGetFlags(reachabilityRef, &flags))
	{
		if(localWiFiRef)
		{
			retVal = [self localWiFiStatusForFlags: flags];
		}
		else
		{
			retVal = [self networkStatusForFlags: flags];
		}
	}
	return retVal;
}
@end

