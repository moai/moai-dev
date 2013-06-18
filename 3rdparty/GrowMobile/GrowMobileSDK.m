// Copyright (C) 2012 by Grow Mobile, Inc.
//
// This file is part of the Grow Mobile SDK.
//
// By using the Grow Mobile SDK in your software, you agree to the terms of the Grow Mobile SDK License Agreement.
//
// The Grow Mobile SDK is bound by the Grow Mobile SDK License Agreement which can be found here:
// http://www.growmobile.com/sdk/license

#import "GrowMobileSDK.h"

// required for device access
#import <UIKit/UIKit.h>

// Required by getMacAddress
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

// Required by getLocalIpAddress
#import <ifaddrs.h>
#import <arpa/inet.h>

// You need to add AdSupport.framework to your project
// Required for iOS6, but for compatibility with pre-iOS6, make this link Optional
// Only necessary if you're using Xcode version 4.5 or above
#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_5_1
#import <AdSupport/AdSupport.h>
#endif

// Required by md5 and sha1
#import <CommonCrypto/CommonDigest.h>

// Use this compile switch to include or exclude UDID
#define NO_UDID

@interface GrowMobileSDK(Private)

+ (NSString *) getUniqueIdentifier;
+ (NSString *) getModel;
+ (NSString *) getSystem;
+ (NSString *) getLocale;
+ (NSString *) getLanguage;
+ (NSString *) getCountry;
+ (NSString *) getMacAddress;
+ (NSString *) getAdvertisingIdentifier;
+ (NSString *) isAdvertisingTrackingEnabled;
+ (NSString *) getLocalIpAddressForInterface:(NSString *)interface;
+ (NSString *) getLocalIpAddress;
+ (NSString *) md5:(NSString *)input;
+ (NSString *) sha1:(NSString *)input;
+ (NSString *) generateSignature:(NSString *)udid;
+ (NSMutableString *) getBaseUrl:(NSString *)action;
+ (void) invokeUrl:(NSString *)url;
+ (void) invokeUrlWithPerformSelector:(NSString *)url;
+ (void) invokeUrlWithPerformSelector:(NSString *)url afterDelay:(NSTimeInterval)delay;

@end

// We put connection delegate in a separate class to make the code cleaner
@interface GrowMobileSDKConnectionDelegate : NSObject<NSURLConnectionDelegate>

@end

// How the retry logic handles loss of network connectivity
// --------------------------------------------------------
// First we assume the Grow Mobile SDK will only be used in networked apps. In fact most of our events
// only make sense when there is network connectivity. The retry logic is designed to fully handle
// transient network errors. It will also handle persistent errors reliably and gracefully
//
// Below are some more details:
//
// 1. No persistence
// We don't persist events in Core Data (e.g.)
// When the app is closed, all outstanding events data will be lost
// Furthermore there is a limit on the number of concurrent requests (GROW_MOBILE_RETRY_THRESHOLD)
// If there are too many outstanding requests, retry will stop accumulating
// Only the first several requests will be retried if the app is persistently offline
//
// 2. Reachability
// The code does not check for network reachability before retrying
// Instead, it uses exponential delay
// The first retry will wait for 4 seconds (GROW_MOBILE_RETRY_INITIAL_DELAY)
// The second retry will double and wait for 8 seconds (GROW_MOBILE_RETRY_EXPONENTIAL_DELAY)
// Until a max retry is reached (GROW_MOBILE_RETRY_MAX_DELAY)
// This algorithm prevents us from pounding the network if the app is persistently offline
//
// 3. Stampede
// When the app wakes up from background and if there is network connectivity, it is possible that all
// the retries will wake up simultaneously and stampede with other more important activities such as
// in-app purchase, game center, and start session. To prevent those cases, the code implements
// a minimal delay between network requests (GROW_MOBILE_STAMPEDE_THROTTLE)
//

#pragma mark - GrowMobileSDKConnectionDelegate implementation
#pragma mark -

// Stop retrying if there are too many outstanding requests
#define GROW_MOBILE_RETRY_THRESHOLD 5

// Initial delay in seconds for each network retry
#define GROW_MOBILE_RETRY_INITIAL_DELAY 4

// Delay will double for the next retry
#define GROW_MOBILE_RETRY_EXPONENTIAL_DELAY 2

// Up to a maximal delay in seconds
#define GROW_MOBILE_RETRY_MAX_DELAY 512 // roughly 10 minutes

// The number of current outstanding requests
static int outstandingRequests = 0;

// The current retry delay
static NSTimeInterval retryDelay = GROW_MOBILE_RETRY_INITIAL_DELAY;

@implementation GrowMobileSDKConnectionDelegate

- (NSCachedURLResponse*) connection:(NSURLConnection*)connection willCacheResponse:(NSCachedURLResponse*)cachedResponse
{
    // NSLog(@"willCacheResponse called");

    // Disable caching
	return nil;
}

- (void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    // NSLog(@"didReceiveResponse called");
}

- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    // NSLog(@"didReceiveData called");
}

- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSString *url = [[error userInfo] objectForKey:NSURLErrorFailingURLStringErrorKey];
    NSLog(@"didFailWithError - %@ %@", [error localizedDescription], url);

    if (outstandingRequests > GROW_MOBILE_RETRY_THRESHOLD)
    {
        // Decrement the counter for abandoned requests
        outstandingRequests --;
    }
    else
    {
        NSTimeInterval delay = retryDelay;

        // Double the delay
        retryDelay *= GROW_MOBILE_RETRY_EXPONENTIAL_DELAY;
        if (retryDelay > GROW_MOBILE_RETRY_MAX_DELAY)
            retryDelay = GROW_MOBILE_RETRY_MAX_DELAY;
        
        [GrowMobileSDK invokeUrlWithPerformSelector:url afterDelay:delay];
    }
}

- (void) connectionDidFinishLoading:(NSURLConnection *)connection
{
    // NSLog(@"connectionDidFinishLoading called");
    
    // Decrement the counter for successful requests
    outstandingRequests --;

    // Reset the current delay once we know there is network connectivity
    retryDelay = GROW_MOBILE_RETRY_INITIAL_DELAY;
}

@end

#pragma mark - GrowMobileSDK implementation
#pragma mark -

@implementation GrowMobileSDK

// Consecutive opens within 10 minutes are counted as one open
#define GROW_MOBILE_RETENTION_THRESHOLD 600

+ (void) reportOpen
{
    static NSTimeInterval lastOpen = 0;
    NSTimeInterval now = [[NSDate date] timeIntervalSince1970];
    if (lastOpen == 0 || now - lastOpen > GROW_MOBILE_RETENTION_THRESHOLD)
    {
        NSMutableString *url = [self getBaseUrl:@"open"];
        [self invokeUrlWithPerformSelector:url];
        lastOpen = now;
    }
}

+ (void) reportActionWithKey:(NSString *)key
                    andValue:(NSString *)value
{
    NSMutableString *url = [self getBaseUrl:@"action"];
    key = [key stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    value = [value stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [url appendFormat:@"&key=%@&value=%@", key, value];
    [self invokeUrlWithPerformSelector:url];
}

+ (void) reportInAppPurchaseWithCurrency:(NSString *)currency
                               andAmount:(NSString *)amount
{
    NSMutableString *url = [self getBaseUrl:@"in_app_purchase"];
    [url appendFormat:@"&currency=%@&amount=%@", currency, amount];
    [self invokeUrlWithPerformSelector:url];
}

+ (void) reportAdViewWithAdNetwork:(NSString *)adNetwork
{
    NSMutableString *url = [self getBaseUrl:@"ad_view"];
    adNetwork = [adNetwork stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [url appendFormat:@"&ad_network=%@", adNetwork];
    [self invokeUrlWithPerformSelector:url];
}

+ (void) reportOfferWallWithUsdAmount:(NSString *)usdAmount
                         andAdNetwork:(NSString *)adNetwork
{
    NSMutableString *url = [self getBaseUrl:@"offer_wall"];
    adNetwork = [adNetwork stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    [url appendFormat:@"&usd_amount=%@&ad_network=%@", usdAmount, adNetwork];
    [self invokeUrlWithPerformSelector:url];
}

#pragma mark - Private methods
#pragma mark -

+ (NSString *) getUniqueIdentifier
{
#ifndef NO_UDID
    return [[UIDevice currentDevice] uniqueIdentifier];
#else
    return nil;
#endif
}

+ (NSString *) getModel
{
    return [[UIDevice currentDevice] model];
}

+ (NSString *) getSystem
{
    NSString *name = [[UIDevice currentDevice] systemName];
    NSString *version = [[UIDevice currentDevice] systemVersion];
    return [NSString stringWithFormat:@"%@ %@", name, version];
}

+ (NSString *) getLocale
{
    return [[NSLocale currentLocale] objectForKey:NSLocaleIdentifier];
}

+ (NSString *) getLanguage
{
    return [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];
}

+ (NSString *) getCountry
{
    return [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
}

// Source: https://github.com/erica/uidevice-extension/blob/master/UIDevice-Hardware.m
+ (NSString *) getMacAddress
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
        NSLog(@"Error: Memory allocation error\n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        NSLog(@"Error: sysctl, take 2\n");
        free(buf);
        return NULL;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    NSString *outstring =
        [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
            *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    free(buf);
    return outstring;
}

+ (NSString *) getAdvertisingIdentifier
{
#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_5_1
    id adSupport = NSClassFromString(@"ASIdentifierManager");
    if (adSupport != nil)
    {
        return [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    }
#endif
    
    return nil;
}

+ (NSString *) isAdvertisingTrackingEnabled
{
#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_5_1
    id adSupport = NSClassFromString(@"ASIdentifierManager");
    if (adSupport != nil)
    {
        return [[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled] ? @"true" : @"false";
    }
#endif
    
    return nil;
}

// Source: https://github.com/erica/uidevice-extension/blob/master/UIDevice-Reachability.m
+ (NSString *) getLocalIpAddressForInterface:(NSString *)interface
{
    BOOL success;
    struct ifaddrs * addrs;
    const struct ifaddrs * cursor;
    NSString *retval = nil;
    
    success = getifaddrs(&addrs) == 0;
    if (success)
    {
        cursor = addrs;
        while (cursor != NULL)
        {
            // The second test keeps from picking up the loopback address
            if (cursor->ifa_addr->sa_family == AF_INET &&
                (cursor->ifa_flags & IFF_LOOPBACK) == 0)
            {
                NSString *name = [NSString stringWithUTF8String:cursor->ifa_name];
                if ([name hasPrefix:interface])
                {
                    retval = [NSString stringWithUTF8String:
                              inet_ntoa(((struct sockaddr_in *)cursor->ifa_addr)->sin_addr)];
                    break;
                }
            }
            cursor = cursor->ifa_next;
        }
        
        freeifaddrs(addrs);
    }
    
    return retval;
}

+ (NSString *) getLocalIpAddress
{
    // First we attempt to get WiFi address
    NSString *localIpAddress = [self getLocalIpAddressForInterface:@"en"];
    if (!localIpAddress)
    {
        // If WiFi does not work, attempt to get Cellular address
        localIpAddress = [self getLocalIpAddressForInterface:@"pdp_ip"];
    }
    
    return localIpAddress;
}

+ (NSString *) md5:(NSString *)input
{
    const char *cStr = [input UTF8String];
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    CC_MD5(cStr, strlen(cStr), digest);

    NSMutableString *output = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(int i = 0; i < CC_MD5_DIGEST_LENGTH; ++ i)
        [output appendFormat:@"%02x", digest[i]];

    return output;
}

+ (NSString *) sha1:(NSString *)input
{
    NSData *stringBytes = [input dataUsingEncoding:NSUTF8StringEncoding];
    unsigned char digest[CC_SHA1_DIGEST_LENGTH];

    NSMutableString *output = nil;
    if (CC_SHA1([stringBytes bytes], [stringBytes length], digest))
    {
        output = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
        for(int i = 0; i < CC_SHA1_DIGEST_LENGTH; ++ i)
            [output appendFormat:@"%02x", digest[i]];
    }

    return output;
}

#define GROW_MOBILE_API_URL @"https://api.growmobile.com/tracking"

+ (NSString *) generateSignature:(NSString *)udid
{
    NSString *signature =
        [NSString stringWithFormat:@"%@:%@:%@",
            GROW_MOBILE_APP_KEY, udid, GROW_MOBILE_APP_SECRET];

    return [self sha1:signature];
}

+ (NSMutableString *) getBaseUrl:(NSString *)action
{
    NSString *udid = [self getUniqueIdentifier];
    NSString *udid_md5;
    NSString *udid_sha1;
    if (!udid)
    {
        udid = @"unknown";
        udid_md5 = @"unknown";
        udid_sha1 = @"unknown";
    }
    else
    {
        udid_md5 = [self md5:udid];
        udid_sha1 = [self sha1:udid];
    }

    NSString *mac = [self getMacAddress];
    NSString *mac_md5;
    NSString *mac_sha1;
    if (!mac)
    {
        mac = @"unknown";
        mac_md5 = @"unknown";
        mac_sha1 = @"unknown";
    }
    else
    {
        mac_md5 = [self md5:mac];
        mac_sha1 = [self sha1:mac];
    }

    NSMutableString *baseUrl = [NSMutableString stringWithFormat:
        @"%@/%@?app_key=%@&udid=%@&udid_md5=%@&udid_sha1=%@&mac=%@&mac_md5=%@&mac_sha1=%@",
        GROW_MOBILE_API_URL, action, GROW_MOBILE_APP_KEY, udid, udid_md5, udid_sha1, mac, mac_md5, mac_sha1];

    NSString *model = [[self getModel] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *system = [[self getSystem] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *locale = [self getLocale];
    NSString *language = [self getLanguage];
    NSString *country = [self getCountry];

    [baseUrl appendFormat:@"&model=%@&system=%@&locale=%@&language=%@&country=%@",
        model, system, locale, language, country];

    NSString *aid = [self getAdvertisingIdentifier];
    if (aid != nil)
    {
        NSString *aid_enabled = [self isAdvertisingTrackingEnabled];
        [baseUrl appendFormat:@"&aid=%@&aid_enabled=%@", aid, aid_enabled];
    }

    NSString *device_ip = [self getLocalIpAddress];
    if (!device_ip)
        device_ip = @"unknown";
    [baseUrl appendFormat:@"&device_ip=%@", device_ip];

#ifndef NO_UDID
    NSString *signature = [self generateSignature:udid];
#else
    NSString *signature = [self generateSignature:mac];
#endif
    [baseUrl appendFormat:@"&signature=%@", signature];

    return baseUrl;
}

// iOS default is 60 seconds
#define GROW_MOBILE_TIMEOUT_INTERVAL 60

// To avoid stampede where all network retries wake up at the same time,
// we add some minimal delays between consecutive calls
#define GROW_MOBILE_STAMPEDE_THROTTLE 2 // seconds

+ (void) invokeUrl:(NSString *)url
{
    static GrowMobileSDKConnectionDelegate *connectionDelegate = nil;
    if (!connectionDelegate)
    {
        // Initialize a singleton and don't bother with releasing it
        connectionDelegate = [[GrowMobileSDKConnectionDelegate alloc] init];
    }

    static NSTimeInterval lastCall = 0;
    NSTimeInterval now = [[NSDate date] timeIntervalSince1970];
    if (lastCall == 0 || now - lastCall > GROW_MOBILE_STAMPEDE_THROTTLE)
    {
        NSLog(@"Calling %@...", url);
        NSMutableURLRequest *urlRequest =
            [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]
                                    cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData
                                timeoutInterval:GROW_MOBILE_TIMEOUT_INTERVAL];

        // Calling server asynchronously
        [NSURLConnection connectionWithRequest:urlRequest delegate:connectionDelegate];
        lastCall = now;
    }
    else
    {
        // Roughly delay the whole throttle
        [self invokeUrlWithPerformSelector:url afterDelay:GROW_MOBILE_STAMPEDE_THROTTLE];
    }
}

+ (void) invokeUrlWithPerformSelector:(NSString *)url
{
    // Increment the counter for new requests
    outstandingRequests ++;

    // Use a selector with 0 delay because we don't want to block the run loop during app launch
    [self invokeUrlWithPerformSelector:url afterDelay:0];
}

+ (void) invokeUrlWithPerformSelector:(NSString *)url afterDelay:(NSTimeInterval)delay
{
    [self performSelector:@selector(invokeUrl:) withObject:url afterDelay:delay];
}

@end
