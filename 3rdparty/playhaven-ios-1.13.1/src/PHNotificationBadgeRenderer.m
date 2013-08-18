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

 PHNotificationBadgeRenderer.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHNotificationBadgeRenderer.h"
#import "PHConstants.h"

static UIImage *BadgeImage;

@implementation PHNotificationBadgeRenderer

+ (void)initialize
{
    if (self == [PHNotificationBadgeRenderer class]) {
        UIImage *badge;
        if (IS_RETINA_DISPLAY())
            badge = convertByteDataToUIImage((playHavenImage *)&badge_2x_image);
        else
            badge = convertByteDataToUIImage((playHavenImage *)&badge_image);

        BadgeImage = [[badge stretchableImageWithLeftCapWidth:14 topCapHeight:0] retain];
    }
}

- (void)drawNotification:(NSDictionary *)notificationData inRect:(CGRect)rect
{
    NSString *value = [notificationData valueForKey:@"value"];
    if ([value isEqualToString:@"0"]) {
        return;
    }
    CGSize notificationSize = [self sizeForNotification:notificationData];
    [BadgeImage drawInRect:CGRectMake(0, 0, notificationSize.width, BadgeImage.size.height)];

    [[UIColor whiteColor] set];
    [value drawAtPoint:CGPointMake(10.0f, 1.0f) withFont:[UIFont boldSystemFontOfSize:17.0f]];
}

- (CGSize)sizeForNotification:(NSDictionary *)notificationData
{
    NSString *value = [notificationData valueForKey:@"value"];
    if ([value isEqualToString:@"0"]) {
        return CGSizeZero;
    }

    CGFloat valueWidth = [value sizeWithFont:[UIFont boldSystemFontOfSize:17.0f]].width + 20.0f;
    return CGSizeMake(valueWidth, BadgeImage.size.height);
}
@end
