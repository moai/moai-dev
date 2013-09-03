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

 PHNotificationViewTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/30/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHNotificationView.h"

@interface PHNotificationViewTest : SenTestCase
@end

@interface PHNotificationViewDeallocTest : SenTestCase {
    PHNotificationView *_notificationView;
}
@end

@implementation PHNotificationViewTest

- (void)testInstance
{
    PHNotificationView *notificationView = [[PHNotificationView alloc] initWithApp:@"" secret:@"" placement:@""];
    STAssertNotNil(notificationView, @"expected notification view instance, got nil");
    STAssertTrue([notificationView respondsToSelector:@selector(refresh)], @"refresh method not present");
}
@end

@implementation PHNotificationViewDeallocTest
- (void)setUp
{
    _notificationView = [[PHNotificationView alloc] initWithApp:@"TOKEN" secret:@"SECRET" placement:@"more_games"];
    [_notificationView refresh];
}

- (void)testRelease
{
    [_notificationView release];
}
@end
