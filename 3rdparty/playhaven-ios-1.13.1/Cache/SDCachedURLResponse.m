//
//  SDCachedURLResponse.m
//  SDURLCache
//
//  Created by Olivier Poitrey on 12/05/12.
//  Copyright (c) 2012 Dailymotion. All rights reserved.
//

#import "SDCachedURLResponse.h"

@implementation PH_SDCACHEDURLRESPONSE_CLASS

@synthesize response;

+ (id)cachedURLResponseWithNSCachedURLResponse:(NSCachedURLResponse *)response
{
    PH_SDCACHEDURLRESPONSE_CLASS *wrappedResponse = [[PH_SDCACHEDURLRESPONSE_CLASS alloc] init];
    wrappedResponse.response = response;
    return [wrappedResponse autorelease];
}

#pragma mark NSCopying Methods

- (id)copyWithZone:(NSZone *)zone
{
    PH_SDCACHEDURLRESPONSE_CLASS *newResponse = [[[self class] allocWithZone:zone] init];

    if (newResponse)
    {
        newResponse.response = [[self.response copyWithZone:zone] autorelease];
    }

    return newResponse;
}

#pragma mark NSCoding Methods
- (void)encodeWithCoder:(NSCoder *)coder
{
    // force write the data of underlying cached response
    [coder encodeDataObject:self.response.data];
    [coder encodeObject:self.response.response forKey:@"response"];
    [coder encodeObject:self.response.userInfo forKey:@"userInfo"];
    [coder encodeInt:self.response.storagePolicy forKey:@"storagePolicy"];
}

- (id)initWithCoder:(NSCoder *)coder
{
    if ((self = [super init]))
    {
        self.response = [[[NSCachedURLResponse alloc] initWithResponse:[coder decodeObjectForKey:@"response"]
                                                                  data:[coder decodeDataObject]
                                                              userInfo:[coder decodeObjectForKey:@"userInfo"]
                                                         storagePolicy:[coder decodeIntForKey:@"storagePolicy"]] autorelease];
    }

    return self;
}

- (void)dealloc
{
    [response release], response = nil;
    [super dealloc];
}

@end
