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

 PHNotificationRenderer.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

/**
 * @internal
 *
 * @brief Base notification rendering class. This trivial implementation is used when
 * the type of notification being rendered is unknown, which will result in no
 * badge being rendered. See PHNotificationBadgeRenderer for a default badge
 * implementation.
 **/
@interface PHNotificationRenderer : NSObject

/**
 * Subclasses should override this method and draw the notification represented
 * by \c notificationData in CGRect \c rect
 *
 * @param notificationData
 *   The notification data to draw
 *
 * @return rect
 *   The rect into which it should be drawn
 **/
- (void)drawNotification:(NSDictionary *)notificationData inRect:(CGRect)rect;

/**
 * Subclasses should override this method and return the CGSize needed to draw
 * the notification represented by \c notificationData
 *
 * @param notificationData
 *   The notification data
 *
 * @return
 *   The size needed to draw the notification
 **/
- (CGSize)sizeForNotification:(NSDictionary *)notificationData;
@end
