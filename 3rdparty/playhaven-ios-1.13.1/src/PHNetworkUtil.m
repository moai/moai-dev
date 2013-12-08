/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHNetworkUtil.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 5/4/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHNetworkUtil.h"
#import "PHConstants.h"
#import "PHAPIRequest.h"

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import <netinet/in.h>
#include <arpa/inet.h>

@interface PHNetworkUtil ()
- (void)_backgroundCheckDNSResolutionForURLPath:(NSString *)urlPath;
@end

@implementation PHNetworkUtil

+ (id)sharedInstance
{
    static dispatch_once_t pred;
    static PHNetworkUtil *shared = nil;

    dispatch_once(&pred, ^{
        shared = [[PHNetworkUtil alloc] init];
    });
    return shared;
}

- (void)checkDNSResolutionForURLPath:(NSString *)urlPath
{
    [self performSelectorInBackground:@selector(_backgroundCheckDNSResolutionForURLPath:) withObject:urlPath];
}

- (void)_backgroundCheckDNSResolutionForURLPath:(NSString *)urlPath
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    Boolean   result    = FALSE;
    CFHostRef hostRef   = NULL;
    NSArray  *addresses = NULL;
    NSString *hostname  = [urlPath substringFromIndex:7];

    hostRef = CFHostCreateWithName(kCFAllocatorDefault, (CFStringRef)hostname);
    if (hostRef) {
        result = CFHostStartInfoResolution(hostRef, kCFHostAddresses, NULL); // Pass an error instead of NULL here to find out why it failed
        if (result == TRUE) {
            addresses = (NSArray*)CFHostGetAddressing(hostRef, &result);
        }
    }
    if (result == TRUE) {
        [addresses enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
            NSString *strDNS = [NSString stringWithUTF8String:inet_ntoa(*((struct in_addr *)obj))];
            NSLog(@"Resolved %@ -> %@", hostname, strDNS);
        }];

    } else {
        NSLog(@"Could not resolve %@", hostname);
    }

    if (hostRef)
    {
        CFRelease(hostRef);
    }

    [pool release];
}

- (CFDataRef)newMACBytes
{
    if ([PHAPIRequest optOutStatus]) {
        return nil;
    }

    int                 mib[6];
    size_t              len;
    char               *buf;
    uint8_t            *ptr;
    struct if_msghdr   *ifm;
    struct sockaddr_dl *sdl;

    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    mib[5] = if_nametoindex("en0");

    if (mib[5] == 0)
    {
        mib[5] = if_nametoindex("en1");
        
        if (mib[5] == 0)
        {
            PH_NOTE(@"Error: if_nametoindex error\n");
            return NULL;
        }
    }

    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        PH_NOTE(@"Error: sysctl, take 1\n");
        return NULL;
    }

    if ((buf = malloc(len)) == NULL)
    {
        PH_NOTE(@"Could not allocate memory. error!\n");
        return NULL;
    }

    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        PH_NOTE(@"Error: sysctl, take 2");
        free(buf);
        return NULL;
    }

    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (uint8_t *)LLADDR(sdl);
    CFDataRef data = CFDataCreate(NULL, (uint8_t*)ptr, 6);

    free(buf);
    return data;
}

- (NSString *)stringForMACBytes:(CFDataRef)macBytes
{
    const uint8_t *ptr = CFDataGetBytePtr(macBytes);
    return [[NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X",
                            *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)] lowercaseString];
}

- (NSString *)ODIN1ForMACBytes:(CFDataRef)macBytes
{
    unsigned char messageDigest[CC_SHA1_DIGEST_LENGTH];

    CC_SHA1(CFDataGetBytePtr((CFDataRef)macBytes),
            CFDataGetLength((CFDataRef)macBytes),
            messageDigest);

    CFMutableStringRef string = CFStringCreateMutable(NULL, 40);
    for(int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++) {
        CFStringAppendFormat(string,
                             NULL,
                             (CFStringRef)@"%02X",
                             messageDigest[i]);
    }

    CFStringLowercase(string, CFLocaleGetSystem());

    NSString *odinstring = [[[NSString alloc] initWithString:(NSString*)string] autorelease];
    CFRelease(string);

    return odinstring;
}
@end
