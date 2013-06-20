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

 PHNotificationView.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <UIKit/UIKit.h>
#import "PHAPIRequest.h"

@class PHNotificationRenderer;
@class PHPublisherMetadataRequest;

/**
 * @internal
 *
 * @brief View class that renders notification badges for placements. Uses
 * PHNotificationRenderer classes to allow publishers to customize the display
 * of notification badges without needing to subclass or handle additional
 * functionality. See "Customizing notification rendering with
 * PHNotificationRenderer" in the README for more on how that works.
 **/
@interface PHNotificationView : UIView <PHAPIRequestDelegate> {
    NSString *_app;
    NSString *_secret;
    NSString *_placement;

    NSDictionary               *_notificationData;
    PHNotificationRenderer     *_notificationRenderer;
    PHPublisherMetadataRequest *_request;
}

/**
 * Set the specific PHNotificationRenderer subclass for the type. Currently only
 * "badge" is supported, but other types may be returned by the API in
 * the future
 *
 * @param rendererClass
 *   The renderer class
 *
 * @param type
 *   The type
 **/
+ (void)setRendererClass:(Class)rendererClass forType:(NSString *)type;

/**
 * Returns a new instance of the PHNotificationRenderer subclass for the
 * notification type indicated in \c notificationData
 *
 * @param notificationData
 *   The notification data
 *
 * @return
 *   A renderer subclass
 **/
+ (PHNotificationRenderer *)newRendererForData:(NSDictionary *)notificationData;

/**
 * Create a new notification view set up to make request using \c token, \c secret, \c placement
 *
 * @param app
 *   The app
 *
 * @param secret
 *   The secret
 *
 * @param placement
 *   The placement
 *
 * @return
 *   A new notification view
 **/
- (id)initWithApp:(NSString *)app secret:(NSString *)secret placement:(NSString *)placement;

@property (nonatomic,retain) NSDictionary *notificationData; /**< Getter/setter with notification data returned from the last request to the API */

/**
 * Retrieve latest notification data from the API and redraw the
 * notification view
 **/
- (void)refresh;

/**
 * @deprecated (to show a compiler warning)
 *
 * A test method that always returns a notification badge with value "1" useful for positioning the badge on the screen
 **/
- (void)test DEPRECATED_ATTRIBUTE; // TODO: Fix Doxygen docs for this member

/**
 * Resets the notification data and hides the notification view
 **/
- (void)clear;
@end
