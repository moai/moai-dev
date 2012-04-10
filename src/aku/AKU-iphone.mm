// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <UIKit/UIKit.h>
#import <aku/AKU-iphone.h>
#import <moaiext-iphone/moaiext-iphone.h>
#import <moaiext-iphone/Reachability.h>
#import <moaicore/MOAIEnvironment.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

#import <Crittercism.h>
#import <TapjoyConnect.h>

#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>




// Return the local MAC addy
// Courtesy of FreeBSD hackers email list
// Accidentally munged during previous update. Fixed thanks to erica sadun & mlamb.
static NSString * MacAddress(void) 
{
    
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        printf("Error: if_nametoindex error\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 1\n");
        return NULL;
    }
    
    if ((buf = (char *)malloc(len)) == NULL) {
        printf("Could not allocate memory. error!\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 2");
        free(buf);
        return NULL;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X", 
                           *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    free(buf);
    
    return outstring;
}

#import <CommonCrypto/CommonDigest.h>

@interface NSString(MD5Addition)

- (NSString *) stringFromMD5;

@end


@implementation NSString(MD5Addition)

- (NSString *) stringFromMD5{
    
    if(self == nil || [self length] == 0)
        return nil;
    
    const char *value = [self UTF8String];
    
    unsigned char outputBuffer[CC_MD5_DIGEST_LENGTH];
    CC_MD5(value, strlen(value), outputBuffer);
    
    NSMutableString *outputString = [[NSMutableString alloc] initWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(NSInteger count = 0; count < CC_MD5_DIGEST_LENGTH; count++){
        [outputString appendFormat:@"%02x",outputBuffer[count]];
    }
    
    return [outputString autorelease];
}

@end



//----------------------------------------------------------------//
void AKUAppDidFailToRegisterForRemoteNotificationsWithError ( NSError* error ) {

	MOAIApp::Get ().DidFailToRegisterForRemoteNotificationsWithError ( error );
}

//----------------------------------------------------------------//
void AKUAppDidReceiveLocalNotification ( UILocalNotification* notification ) {

	MOAIApp::Get ().DidReceiveLocalNotification ( notification );
}

//----------------------------------------------------------------//
void AKUAppDidReceiveRemoteNotification ( NSDictionary* userInfo ) {

	MOAIApp::Get ().DidReceiveRemoteNotification ( userInfo );
}

//----------------------------------------------------------------//
void AKUAppDidRegisterForRemoteNotificationsWithDeviceToken ( NSData* deviceToken ) {

	MOAIApp::Get ().DidRegisterForRemoteNotificationsWithDeviceToken ( deviceToken );
}

//-----------------------------------------------------------------//
void AKUAppDidStartSession () {

	MOAIApp::Get ().DidStartSession ();
}

//-----------------------------------------------------------------//
void AKUAppOpenFromURL ( NSURL* url ) {
	
	MOAIApp::Get ().AppOpenedFromURL ( url );
}

//-----------------------------------------------------------------//
void AKUAppWillEndSession () {

	MOAIApp::Get ().WillEndSession ();
}

//-----------------------------------------------------------------//
const char* AKUGetGUID ( ) {

	CFUUIDRef uuid = CFUUIDCreate( NULL );
	NSString* session_uuid = ( NSString * ) CFUUIDCreateString( NULL, uuid );
	CFRelease( uuid );
	return [ session_uuid UTF8String ];
}

//-----------------------------------------------------------------//
long AKUGetIphoneNetworkReachability ( ) {


	Reachability *reach = [ Reachability reachabilityForInternetConnection ];
	NetworkStatus status = [ reach currentReachabilityStatus ];
		
	if ( status == NotReachable ) {
		return ( long )CONNECTION_TYPE_NONE;
	} else if ( status == ReachableViaWWAN ) {
		// Update network information
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		MOAIEnvironment::Get ().SetCarrierISOCountryCode ( [ carrierInfo.isoCountryCode UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileCountryCode ( [[carrierInfo mobileCountryCode ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierName ( [[carrierInfo carrierName ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileNetworkCode ( [[carrierInfo mobileNetworkCode ] UTF8String ]);
		return ( long )CONNECTION_TYPE_WWAN;
	} else if ( status == ReachableViaWiFi ) {
		return ( long )CONNECTION_TYPE_WIFI;
	}
	
	return ( long )CONNECTION_TYPE_NONE;
}

//----------------------------------------------------------------//
void AKUIphoneInit ( UIApplication* application ) {

	loadMoaiLib_NSArray ();
	loadMoaiLib_NSData ();
	loadMoaiLib_NSDate ();
	loadMoaiLib_NSDictionary ();
	loadMoaiLib_NSError ();
	loadMoaiLib_NSNumber ();
	loadMoaiLib_NSObject ();
	loadMoaiLib_NSString ();

	MOAIApp::Affirm ().SetApplication ( application );
	MOAIGameCenter::Affirm ();
	
	MOAIEnvironment::Affirm ();
	// Device properties	
	MOAIEnvironment::Get ().SetAppVersion ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleVersion" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppID ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppDisplayName ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ] );
	MOAIEnvironment::Get ().SetCacheDirectory ( [[ NSSearchPathForDirectoriesInDomains ( NSCachesDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	MOAIEnvironment::Get ().SetCountryCode ( [[[ NSLocale currentLocale ] objectForKey: NSLocaleCountryCode ] UTF8String ]);
	MOAIEnvironment::Get ().SetDocumentDirectory ( [[ NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	MOAIEnvironment::Get ().SetGUIDFunc ( &AKUGetGUID );
	MOAIEnvironment::Get ().SetLanguageCode ( [[[ NSLocale currentLocale ] objectForKey: NSLocaleLanguageCode ] UTF8String ]);
	MOAIEnvironment::Get ().SetOSBrand ( "iOS" );
	MOAIEnvironment::Get ().SetOSVersion ( [[ UIDevice currentDevice ].systemVersion UTF8String ] );
	MOAIEnvironment::Get ().SetResourceDirectory ( [[[ NSBundle mainBundle ] resourcePath ] UTF8String ]);
//	MOAIEnvironment::Get ().SetUDID ( [[ UIDevice currentDevice ].uniqueIdentifier UTF8String ] );
	MOAIEnvironment::Get ().SetUDID ( [[MacAddress() stringFromMD5] UTF8String ] );
	MOAIEnvironment::Get ().SetDevModel ( [[ UIDevice currentDevice ].platform UTF8String ] );

	if ([[ UIScreen mainScreen ] scale ] == 2.0 ) {
		//this is retina
		MOAIEnvironment::Get ().SetIsRetinaDisplay ( true );
	}
	else {
		MOAIEnvironment::Get ().SetIsRetinaDisplay ( false );
	}	
			
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
	REGISTER_LUA_CLASS ( MOAIGameCenter )
	REGISTER_LUA_CLASS ( MOAIWebView )
			
	#ifndef DISABLE_TAPJOY
		REGISTER_LUA_CLASS ( MOAITapjoy )
	#endif

	#ifndef DISABLE_CRITTERCISM
		REGISTER_LUA_CLASS ( MOAICrittercism )
	#endif
}

//-----------------------------------------------------------------//
void AKUSetConnectionType ( long type ) {

	MOAIEnvironment::Get ().SetConnectionType ( type );
	
	// If we have a cellualr connection, get carrier information
	if ( type == CONNECTION_TYPE_WWAN ) {
	
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		MOAIEnvironment::Get ().SetCarrierISOCountryCode ( [ carrierInfo.isoCountryCode UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileCountryCode ( [[ carrierInfo mobileCountryCode ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierName ( [[ carrierInfo carrierName ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileNetworkCode ( [[ carrierInfo mobileNetworkCode ] UTF8String ]);
	}
}

//-----------------------------------------------------------------//
void AKUSetDefaultFrameBuffer ( GLuint frameBuffer ) {

	MOAIGfxDevice::Get ().SetDefaultFrameBuffer ( frameBuffer );
}

//-----------------------------------------------------------------//
void AKUWasLaunchedWithRemoteNotification ( NSDictionary* remoteNotificationPayload ) {

	if ( remoteNotificationPayload ) {
	
		MOAIApp::Get ().SetRemoteNotificationPayload ( remoteNotificationPayload );
	}
}
