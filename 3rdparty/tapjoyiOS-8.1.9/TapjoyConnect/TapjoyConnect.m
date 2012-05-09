// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



#import "TapjoyConnect.h"
#import "TJCConfig.h"
#import "OpenUDID.h"
#import <CommonCrypto/CommonHMAC.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <mach-o/dyld.h>
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>
#endif


static TapjoyConnect *sharedInstance_ = nil; //To make TapjoyConnect Singleton
static NSString *orignalRequest = TJC_SERVICE_URL;

@implementation TapjoyConnect

@synthesize appID = appID_;
@synthesize secretKey = secretKey_;
@synthesize userID = userID_;
@synthesize plugin = plugin_;
@synthesize isInitialConnect = isInitialConnect_;


+ (TapjoyConnect*)sharedTapjoyConnect
{
	if(!sharedInstance_)
	{
		sharedInstance_ = [[super alloc] init];
	}
	
	return sharedInstance_;
}


- (NSMutableDictionary*)genericParameters
{
	// Device info.
	UIDevice *device = [UIDevice currentDevice];
	NSString *model = [device model];
	NSString *systemVersion = [device systemVersion];
	
#if !defined (TJC_CONNECT_SDK)
	NSString *device_name = [device platform];
	//NSLog(@"device name: %@", device_name);
#endif
	
	// Locale info.
	NSLocale *locale = [NSLocale currentLocale];
	NSString *countryCode = [locale objectForKey:NSLocaleCountryCode];
	NSString *language;
	if ([[NSLocale preferredLanguages] count] > 0)
	{
		language = [[NSLocale preferredLanguages] objectAtIndex:0];
	}
	else
	{
		language = [locale objectForKey:NSLocaleLanguageCode];
	}
	
	// App info.
	NSString *bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
	
	NSString *lad = [self isJailBrokenStr];
	
	NSTimeInterval timeInterval = [[NSDate date] timeIntervalSince1970];
	// Get seconds since Jan 1st, 1970.
	NSString *timeStamp = [NSString stringWithFormat:@"%d", (int)timeInterval];
	
	// Compute verifier.
	NSString *verifier = [TapjoyConnect TJCSHA256CommonParamsWithTimeStamp:timeStamp];
	
	if (!appID_)
	{
		NSLog(@"requestTapjoyConnect:secretKey: must be called before any other Tapjoy methods!");
	}
	
	if (!plugin_)
	{
		plugin_ = TJC_PLUGIN_NATIVE;
	}
	
	NSString *multStr = [NSString stringWithFormat:@"%f", currencyMultiplier_];
	
#if !defined (TJC_CONNECT_SDK)
	NSString *connectionType = [TJCNetReachability getReachibilityType];
#endif
	
	NSString *macID = [TapjoyConnect getMACAddress];
	
	NSString *openUDID = [OpenUDID value];
	
	NSString *openUDIDSlotCount = [NSString stringWithFormat:@"%d", [OpenUDID getOpenUDIDSlotCount]];
	
	NSMutableDictionary * genericDict = [[NSMutableDictionary alloc] initWithObjectsAndKeys:
													 macID, TJC_UNIQUE_MAC_ID,
													 openUDID, TJC_OPEN_UDID,
													 openUDIDSlotCount, TJC_OPEN_UDID_COUNT,
													 model, TJC_DEVICE_TYPE_NAME,
													 systemVersion, TJC_DEVICE_OS_VERSION_NAME,
													 appID_, TJC_APP_ID_NAME,
													 bundleVersion, TJC_APP_VERSION_NAME,
													 TJC_LIBRARY_VERSION_NUMBER, TJC_CONNECT_LIBRARY_VERSION_NAME,
													 countryCode, TJC_DEVICE_COUNTRY_CODE,
													 language, TJC_DEVICE_LANGUAGE,
													 lad, TJC_DEVICE_LAD,
													 timeStamp, TJC_TIMESTAMP,
													 verifier, TJC_VERIFIER,
													 multStr, TJC_URL_PARAM_CURRENCY_MULTIPLIER,
													 plugin_, TJC_PLUGIN,
													 TJC_SDK_TYPE_VALUE, TJC_SDK_TYPE,
													 TJC_PLATFORM_IOS, TJC_PLATFORM,
#if !defined (TJC_CONNECT_SDK)
													 device_name, TJC_DEVICE_NAME,
													 connectionType, TJC_CONNECTION_TYPE_NAME,
#endif
													 nil];
	
	// Only send UDID as a parameter if it's not nil, namely opted-in.
	NSString *uniqueIdentifier = [TapjoyConnect getUniqueIdentifier];
	if (uniqueIdentifier)
	{
		[genericDict setObject:uniqueIdentifier forKey:TJC_UDID];
	}
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
	// Carrier info.
	CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
	CTCarrier *carrier = [netinfo subscriberCellularProvider];
	NSString *carrierName = [carrier carrierName];
	
	if (carrierName)
	{
		[genericDict setObject:carrierName forKey:TJC_CARRIER_NAME];
		
		// VOIP check only valid if carrier exists.
		NSString *allowsVOIP = @"no";
		if ([carrier allowsVOIP])
		{
			allowsVOIP = @"yes";
		}
		
		[genericDict setObject:allowsVOIP forKey:TJC_ALLOWS_VOIP];
		
	}
	
	NSString *isoCountryCode = [carrier isoCountryCode];
	
	if (isoCountryCode)
	{
		[genericDict setObject:isoCountryCode forKey:TJC_CARRIER_COUNTRY_CODE];
	}
	
	NSString *mobileCountryCode = [carrier mobileCountryCode];
	
	if (mobileCountryCode)
	{
		[genericDict setObject:mobileCountryCode forKey:TJC_MOBILE_COUNTRY_CODE];
	}
	
	NSString *mobileNetworkCode = [carrier mobileNetworkCode];
	
	if (mobileNetworkCode)
	{
		[genericDict setObject:mobileNetworkCode forKey:TJC_MOBILE_NETWORK_CODE];
	}
	
	[netinfo release];
#endif
	
	return [genericDict autorelease];
}


- (NSString*)createQueryStringFromDict:(NSDictionary*) paramDict
{
	if(!paramDict)
	{
#if !defined (TJC_CONNECT_SDK)
		[TJCLog logWithLevel:LOG_DEBUG format:@"Sending Nil Getting Generic Dictionary Now"];
#endif
		paramDict = [[TapjoyConnect sharedTapjoyConnect] genericParameters];
	}
	
	NSMutableArray *parts = [NSMutableArray array];
	for (id key in [paramDict allKeys])
	{
		id value = [paramDict objectForKey: key];
		
		// Encode string to a legal URL string.
		NSString *encodedString = (NSString*)CFURLCreateStringByAddingPercentEscapes(NULL,
																											  (CFStringRef)value,
																											  NULL,
																											  (CFStringRef)@"!*'();:@&=+$,/?%#[]",
																											  kCFStringEncodingUTF8);
		
		NSString *part = [NSString stringWithFormat: @"%@=%@", key, encodedString];
		
		[encodedString release];
		
		[parts addObject: part];
	}
	return [parts componentsJoinedByString: @"&"];
}


+ (NSString*)createQueryStringFromDict:(NSDictionary*)paramDict
{
	return [[TapjoyConnect sharedTapjoyConnect] createQueryStringFromDict:paramDict];
}


- (void)connectWithParam:(NSMutableDictionary*)genericDict
{
	NSString *requestString = [NSString stringWithFormat:@"%@%@?%@", orignalRequest, TJC_CONNECT_API, [self createQueryStringFromDict:genericDict]];
	
	NSURL * myURL = [[NSURL alloc] initWithString:requestString];
	NSMutableURLRequest *myRequest = [NSMutableURLRequest requestWithURL: myURL
																				cachePolicy: NSURLRequestReloadIgnoringLocalAndRemoteCacheData
																		  timeoutInterval: 30];
	[myURL release];
	
	if (data_)
	{
		[data_ release];
		data_ = nil;
	}
	
	if (connection_)
	{
		[connection_ cancel];
		[connection_ release];
		connection_ = nil;
	}
	
	dispatch_async(dispatch_get_main_queue(), ^{
		connection_ = [[NSURLConnection alloc] initWithRequest: myRequest delegate: self];
	});
	
	connectAttempts_++;	
}


- (void)setUserIDWithParam:(NSMutableDictionary*)genericDict
{
	// Set user ID in the dictionary before creating the URL.
	if (![TapjoyConnect getUserID])
	{
		return;
	}
	
	[genericDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	NSString *requestString = [NSString stringWithFormat:@"%@%@?%@", orignalRequest, TJC_SET_USER_ID_API, [self createQueryStringFromDict:genericDict]];
	
	NSURL * myURL = [[NSURL alloc] initWithString:requestString];
	NSMutableURLRequest *myRequest = [NSMutableURLRequest requestWithURL: myURL
																				cachePolicy: NSURLRequestReloadIgnoringLocalAndRemoteCacheData
																		  timeoutInterval: 30];
	[myURL release];
	
	if (data_)
	{
		[data_ release];
		data_ = nil;
	}
	
	if (connection_)
	{
		[connection_ cancel];
		[connection_ release];
		connection_ = nil;
	}
	
	connection_ = [[NSURLConnection alloc] initWithRequest: myRequest delegate: self];
	connectAttempts_++;	
}


- (NSCachedURLResponse*)connection:(NSURLConnection*)connection willCacheResponse:(NSCachedURLResponse*)cachedResponse 
{
	// Returning nil will ensure that no cached response will be stored for the connection.
	// This is in case the cache is being used by something else.
	return nil;
}


static const char* jailbreak_apps[] =
{
	"/bin/bash",
	"/Applications/Cydia.app", 
	"/Applications/limera1n.app", 
	"/Applications/greenpois0n.app", 
	"/Applications/blackra1n.app",
	"/Applications/blacksn0w.app",
	"/Applications/redsn0w.app",
	NULL,
};

- (BOOL)isJailBroken
{
#if TARGET_IPHONE_SIMULATOR
	return NO;
#endif
	
	// Check for known jailbreak apps. If we encounter one, the device is jailbroken.
	for (int i = 0; jailbreak_apps[i] != NULL; ++i)
	{
		if ([[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:jailbreak_apps[i]]])
		{
			//NSLog(@"isjailbroken: %s", jailbreak_apps[i]);
			return YES;
		}		
	}
	
	return NO;
}

- (NSString*)isJailBrokenStr
{
	if ([self isJailBroken])
	{
		return @"42";
	}
	
	return @"0";
}


- (void)initConnectWithAppID:(NSString*)appID withSecretKey:(NSString*)secretKey
{
	appID_ = [appID retain];
	secretKey_ = [secretKey retain];
	connectAttempts_ = 0;
	if (connection_)
	{
		[connection_ release];
		connection_ = nil;
	}
}


+ (void)deviceNotificationReceived
{
	// Since we're relying on UIApplicationDidBecomeActiveNotification, we need to make sure we don't call connect twice in a row
	// upon initial start-up of the applicaiton.
	if ([[TapjoyConnect sharedTapjoyConnect] isInitialConnect])
	{
		[TapjoyConnect sharedTapjoyConnect].isInitialConnect = NO;
	}
	else
	{
		// Call connect when the app 
		[[TapjoyConnect sharedTapjoyConnect] connectWithParam:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	}
	
#if !defined (TJC_CONNECT_SDK)
	// When the app goes into the background, refresh the offers web view to clear out stale offers.
	if ([[TJCOffersWebView sharedTJCOffersWebView] isViewVisible])
	{
		//[[TJCOffersWebView sharedTJCOffersWebView] loadView];
		[[TJCOffersWebView sharedTJCOffersWebView] refreshWebView];
	}
	
	// iOS5 apparently automatically pauses videos on app resume.
	// If a video is currently playing, ensure that it continues when the application is brought back to the foreground.
	if ([[[TJCVideoManager sharedTJCVideoManager] videoView] videoAdCurrentlyPlaying])
	{
		[[[TJCVideoManager sharedTJCVideoManager] videoView] videoActionFromAppResume];
	}
	
#if !defined (TJC_GAME_STATE_SDK)
	// Update tap points.
	[TapjoyConnect getTapPoints];
#endif
	
#endif
}


#if defined (TJC_GAME_STATE_SDK)
+ (void)forceGameStateSave
{
	[[TJCGameState sharedTJCGameState] forceResave];
}
#endif


+ (TapjoyConnect*)requestTapjoyConnect:(NSString*)appID secretKey:(NSString*)secretKey
{
	[[TapjoyConnect sharedTapjoyConnect] initConnectWithAppID:appID withSecretKey:secretKey];
	
	// Default the currency multiplier to 1.
	[[TapjoyConnect sharedTapjoyConnect] setCurrencyMultiplier:1.0f];
	
	if (![[TapjoyConnect sharedTapjoyConnect] plugin])
	{
		// Default user id to the UDID.
		[[TapjoyConnect sharedTapjoyConnect] setPlugin:TJC_PLUGIN_NATIVE];
	}
	
	// This should really only be set to YES here ever.
	[TapjoyConnect sharedTapjoyConnect].isInitialConnect = YES;
	
	[[TapjoyConnect sharedTapjoyConnect] connectWithParam:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(deviceNotificationReceived) 
																name:UIApplicationWillEnterForegroundNotification 
															 object:nil];
#else
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(deviceNotificationReceived) 
																name:UIApplicationDidBecomeActiveNotification 
															 object:nil];	
#endif
	
	// Force a game state save on app pause/exit.
#if defined (TJC_GAME_STATE_SDK)
	// Set the application pausing notification.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(forceGameStateSave) 
																name:UIApplicationWillResignActiveNotification
															 object:nil];
	
	// We want to make sure that if the app is set to not run in the background (quit), we also force a save.
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(forceGameStateSave) 
																name:UIApplicationWillTerminateNotification
															 object:nil];
#endif
	
	// Only the Offers and VG SDKs will need to grab tap points upon init.
#if !defined (TJC_CONNECT_SDK) && !defined (TJC_GAME_STATE_SDK)
	// Update tap points.
	[TapjoyConnect getTapPoints];
#endif
	
#if !defined (TJC_CONNECT_SDK)
	[[NSNotificationCenter defaultCenter] addObserver:self
														  selector:@selector(orientationChanged:)
																name:UIApplicationDidChangeStatusBarFrameNotification
															 object:nil];
#endif
	
	return [TapjoyConnect sharedTapjoyConnect];
}


#if !defined (TJC_CONNECT_SDK)
+ (void)orientationChanged:(NSNotification*)notifyObj
{
	//UIInterfaceOrientation orientation = [[notifyObj userInfo] objectForKey:UIApplicationStatusBarOrientationUserInfoKey];
	[TapjoyConnect updateViewsWithOrientation:[UIApplication sharedApplication].statusBarOrientation];
	
	// HACK: Scroll view frame wasn't being updated properly. It seems to be updated some time after this notification is sent...
	[self performSelector:@selector(orientationChangedDelay)
				  withObject:nil 
				  afterDelay:.2f];
}

+ (void)orientationChangedDelay
{
	[TapjoyConnect updateViewsWithOrientation:[UIApplication sharedApplication].statusBarOrientation];
}
#endif


+ (TapjoyConnect*)actionComplete:(NSString*)actionID
{
	// Get the generic params.
 	NSMutableDictionary *paramDict = [[TapjoyConnect sharedTapjoyConnect] genericParameters];
	
	// Overwrite the appID with the actionID. This is how actions are sent.
	[paramDict setObject:[NSString stringWithString:actionID] forKey:TJC_APP_ID_NAME];
	
	// Ping the server.
	[[TapjoyConnect sharedTapjoyConnect] connectWithParam:paramDict];
	
	return [TapjoyConnect sharedTapjoyConnect];
}


+ (NSString*)getAppID
{
	return [[TapjoyConnect sharedTapjoyConnect] appID];
}


+ (void)setUserID:(NSString*)theUserID
{
	[[TapjoyConnect sharedTapjoyConnect] setUserID:theUserID];
	
	// Ping server with user id.
	[[TapjoyConnect sharedTapjoyConnect] setUserIDWithParam:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
}


+ (NSString*)getUserID
{	
	return [[TapjoyConnect sharedTapjoyConnect] userID];
}


+ (NSString*)getSecretKey
{
	return [[TapjoyConnect sharedTapjoyConnect] secretKey];
}


+ (void)setPlugin:(NSString*)thePlugin
{
	[[TapjoyConnect sharedTapjoyConnect] setPlugin:thePlugin];
}


- (void)setCurrencyMultiplier:(float)mult
{
	currencyMultiplier_ = mult;
}


+ (void)setCurrencyMultiplier:(float)mult
{
	[[TapjoyConnect sharedTapjoyConnect] setCurrencyMultiplier:mult];
}


- (float)getCurrencyMultiplier
{
	return currencyMultiplier_;	
}


+ (float)getCurrencyMultiplier
{
	return [[TapjoyConnect sharedTapjoyConnect] getCurrencyMultiplier];
}


+ (NSString*)TJCSHA256CommonParamsWithTimeStamp:(NSString*)timeStamp
{
	NSString *appID = [TapjoyConnect getAppID];
	
	NSString *keyStr = [TapjoyConnect getSecretKey];
	
	NSString *deviceID = [TapjoyConnect getUniqueIdentifier];
	if (!deviceID)
	{
		deviceID = [TapjoyConnect getMACAddress];
	}
	
	NSString *verifierStr = [NSString stringWithFormat:@"%@:%@:%@:%@",
									 appID,
									 deviceID,
									 timeStamp,
									 keyStr];
	
	NSString *hashStr = [TapjoyConnect TJCSHA256WithString:verifierStr];    
	
	return hashStr;
}


+ (NSString*)TJCSHA256CommonParamsWithTimeStamp:(NSString*)timeStamp tapPointsAmount:(int)points guid:(NSString*)guid
{
	NSString *appID = [TapjoyConnect getAppID];
	
	NSString *keyStr = [TapjoyConnect getSecretKey];
	
	NSString *deviceID = [TapjoyConnect getUniqueIdentifier];
	if (!deviceID)
	{
		deviceID = [TapjoyConnect getMACAddress];
	}
	
	NSString *amountStr = [NSString stringWithFormat:@"%d", points];
	
	NSString *verifierStr = [NSString stringWithFormat:@"%@:%@:%@:%@:%@:%@",
									 appID,
									 deviceID,
									 timeStamp,
									 keyStr,
									 amountStr,
									 guid];
	
	NSString *hashStr = [TapjoyConnect TJCSHA256WithString:verifierStr];    
	
	return hashStr;
}


+ (NSString*)TJCSHA256WithString:(NSString*)dataStr
{
	unsigned char SHAStr[CC_SHA256_DIGEST_LENGTH];
	
	CC_SHA256([dataStr UTF8String],
				 [dataStr lengthOfBytesUsingEncoding:NSUTF8StringEncoding],
				 SHAStr);
	
	NSData *SHAData = [[NSData alloc] initWithBytes:SHAStr
														  length:sizeof(SHAStr)];
	
	//NSString *result = [HMAC TJCBase64EncodedString];
	NSString *result = [[SHAData description] stringByReplacingOccurrencesOfString:@" " withString:@""];
	result = [result substringWithRange:NSMakeRange(1, [result length] - 2)];
	
	[SHAData release];
	
	return result;
}


- (void)dealloc 
{
	[appID_ release];
	[secretKey_ release];
	[userID_ release];
	[sharedInstance_ release];
	[data_ release];
	[connection_ release];
	
	[super dealloc];
}







#pragma mark delegate methods for asynchronous requests

- (void)connection:(NSURLConnection*) myConnection didReceiveResponse:(NSURLResponse*) myResponse;
{
#if !defined (TJC_CONNECT_SDK)	
	NSHTTPURLResponse *HTTPResponse = (NSHTTPURLResponse*)myResponse;
	
	int responseCode = [HTTPResponse statusCode];
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"RequestTapjoyConnect response code:%d", responseCode];
#endif
}


- (void)connection:(NSURLConnection*) myConnection didReceiveData:(NSData*) myData;
{
	if (!data_) 
	{
		data_ = [[NSMutableData alloc] init];
	}
	
	[data_ appendData: myData];
}


- (void)connection:(NSURLConnection*) myConnection didFailWithError:(NSError*) myError;
{
	[connection_ release];
	connection_ = nil;
	
	if (connectAttempts_ >=2)
	{	
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_CONNECT_FAILED object:nil];
		return;
	}
	
	if (connectAttempts_ < 2)
	{	
		orignalRequest = TJC_SERVICE_URL_ALTERNATE;
		[[TapjoyConnect sharedTapjoyConnect] connectWithParam:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	}
}


- (void)connectionDidFinishLoading:(NSURLConnection*) myConnection;
{
	[connection_ release];
	connection_ = nil;
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
	[self startParsing:data_];
#else
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_CONNECT_SUCCESS object:nil];
#endif
}

#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
- (void)startParsing:(NSData*) myData 
{
	NSData *xmlData = myData;//(Get XML as NSData)
	NSXMLParser *parser = [[[NSXMLParser alloc] initWithData:xmlData] autorelease];
	[parser setDelegate:self];
	[parser parse];
}


- (void)parser:(NSXMLParser*)parser didStartElement:(NSString*)elementName namespaceURI:(NSString*)namespaceURI qualifiedName:(NSString*)qualifiedName attributes:(NSDictionary*)attributeDict 
{
	currentXMLElement_ = elementName;
	if ([elementName isEqualToString:@"ErrorMessage"])
	{
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_CONNECT_FAILED object:nil];
	}
}


- (void)parser:(NSXMLParser*)parser foundCharacters:(NSString*)string
{
	if([currentXMLElement_ isEqualToString:@"Success"] && [string isEqualToString:@"true"])
	{
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_CONNECT_SUCCESS object:nil];
	}
}


- (void) parser:(NSXMLParser*)parser 
  didEndElement:(NSString*)elementName
   namespaceURI:(NSString*)namespaceURI 
  qualifiedName:(NSString*)qName
{
	
}
#endif




+ (NSString*)getMACAddress
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
	
	if ((mib[5] = if_nametoindex("en0")) == 0) 
	{
		NSLog(@"Error: if_nametoindex error\n");
		return NULL;
	}
	
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
	{
		NSLog(@"Error: sysctl, take 1\n");
		return NULL;
	}
	
	if ((buf = malloc(len)) == NULL) 
	{
		NSLog(@"Could not allocate memory. error!\n");
		return NULL;
	}
	
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) 
	{
		NSLog(@"Error: sysctl, take 2");
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	NSString *macAddress = [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X", 
									*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	macAddress = [macAddress lowercaseString];
	free(buf);
	
	return macAddress;
}


+ (NSString*)getUniqueIdentifier
{
#if TJC_UDID_OPT_IN
	if ([[UIDevice currentDevice] respondsToSelector:@selector(uniqueIdentifier)])
	{
		return [[UIDevice currentDevice] uniqueIdentifier];
	}
	
	return nil;
#else
	return nil;
#endif
}

@end