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

 PHNotificationBadgeRenderer.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#import "PHNotificationRenderer.h"

/**
 * @internal
 *
 * @brief Notification renderer class. This class draws a "badge" type notification in
 * the default notification style familiar to iOS users (red circle, white
 * border, white text)
 **/
@interface PHNotificationBadgeRenderer : PHNotificationRenderer
@end
