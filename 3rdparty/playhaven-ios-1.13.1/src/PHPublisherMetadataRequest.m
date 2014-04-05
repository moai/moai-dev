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

 PHPublisherMetadataRequest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHPublisherMetadataRequest.h"
#import "PHConstants.h"

@interface PHAPIRequest (Private)
- (id)initWithApp:(NSString *)token secret:(NSString *)secret;
@end

@interface PHPublisherMetadataRequest (Private)
- (id)initWithApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate;
@end

@implementation PHPublisherMetadataRequest
@synthesize placement = _placement;

+ (id)requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate
{
    return [[[[self class] alloc] initWithApp:token secret:secret placement:placement delegate:delegate] autorelease];
}

- (id)initWithApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate
{
    if ((self = [self initWithApp:token secret:secret])) {
        self.placement = placement;
        self.delegate  = delegate;
    }

    return self;
}

- (void)dealloc
{
    [_placement release], _placement = nil;
    [super dealloc];
}

#pragma mark - PHAPIRequest

- (NSString *)urlPath
{
    return PH_URL(/v3/publisher/content/);
}

- (NSDictionary *)additionalParameters
{
    return [NSDictionary dictionaryWithObjectsAndKeys:
                                self.placement, @"placement_id",
                                @"1", @"metadata", nil];
}
@end
