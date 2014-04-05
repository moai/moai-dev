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

 PHContent.m (formerly PHContent.m)
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/31/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHContent.h"

@implementation PHContent
@synthesize URL        = _URL;
@synthesize transition = _transition;
@synthesize context    = _context;
@synthesize closeButtonDelay   = _closeButtonDelay;
@synthesize closeButtonURLPath = _closeButtonURLPath;

+ (id)contentWithDictionary:(NSDictionary *)dictionaryRepresentation
{
    BOOL
    shouldCreateInstance = !![dictionaryRepresentation valueForKey:@"frame"];
    shouldCreateInstance = shouldCreateInstance && !![dictionaryRepresentation valueForKey:@"url"];
    shouldCreateInstance = shouldCreateInstance && !![dictionaryRepresentation valueForKey:@"transition"];

    if (shouldCreateInstance) {
        PHContent *result = [[[PHContent alloc] init] autorelease];

        id frameValue = [dictionaryRepresentation valueForKey:@"frame"];
        if ([frameValue isKindOfClass:[NSString class]]) {
            [result setFramesWithDictionary:[NSDictionary dictionaryWithObjectsAndKeys:frameValue, frameValue, nil]];
        } else if ([frameValue isKindOfClass:[NSDictionary class]]) {
            [result setFramesWithDictionary:frameValue];
        } else {
            // We seem to have some invalid value here, yuck
            return nil;
        }

        NSString *url = [dictionaryRepresentation valueForKey:@"url"];
        result.URL = [NSURL URLWithString:url];

        NSString *transition = [dictionaryRepresentation valueForKey:@"transition"];
        if ([@"PH_MODAL" isEqualToString:transition]) {
            result.transition = PHContentTransitionModal;
        } else if ([@"PH_DIALOG" isEqualToString:transition]) {
            result.transition = PHContentTransitionDialog;
        }

        NSDictionary *payload = [dictionaryRepresentation valueForKey:@"context"];
        result.context = payload;

        NSTimeInterval closeButtonDelay = [[dictionaryRepresentation valueForKey:@"close_delay"] floatValue];
        if (closeButtonDelay > 0.0f) {
            result.closeButtonDelay = closeButtonDelay;
        }

        NSString *closeButtonURLPath = [dictionaryRepresentation valueForKey:@"close_ping"];
        result.closeButtonURLPath = closeButtonURLPath;

        return result;
    } else {
        return nil;
    }

}

- (id)init
{
    if ((self = [super init])) {
        _closeButtonDelay = 10.0f;
    }

    return  self;
}

- (void)dealloc
{
    [_URL release], _URL = nil;
    [_context release], _context = nil;
    [_closeButtonURLPath release], _closeButtonURLPath = nil;
    [_frameDict release], _frameDict = nil;
    [super dealloc];
}

- (CGRect)frameForOrientation:(UIInterfaceOrientation)orientation
{
    NSString *orientationKey = (UIInterfaceOrientationIsLandscape(orientation)) ? @"PH_LANDSCAPE" : @"PH_PORTRAIT";
    NSDictionary *frameValue = [_frameDict valueForKey:orientationKey];

    if (!![_frameDict valueForKey:@"PH_FULLSCREEN"]) {
        CGRect frame = [UIScreen mainScreen].applicationFrame;
        CGFloat
        width = frame.size.width,
        height = frame.size.height;

        if (UIInterfaceOrientationIsLandscape(orientation)) {
            return CGRectMake(0, 0, height, width);
        } else {
            return CGRectMake(0, 0, width, height);
        }
    } else if (!!frameValue) {

        CGFloat
        x = [[frameValue valueForKey:@"x"] floatValue],
        y = [[frameValue valueForKey:@"y"] floatValue],
        w = [[frameValue valueForKey:@"w"] floatValue],
        h = [[frameValue valueForKey:@"h"] floatValue];

        return CGRectMake(x, y, w, h);
    } else {
        // No frame data for this orientation
        return CGRectNull;
    }

}

- (void)setFramesWithDictionary:(NSDictionary *)frameDict
{
    if (_frameDict != frameDict) {
        [_frameDict release], _frameDict = [frameDict retain];
    }
}
@end
