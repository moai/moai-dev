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

 PHContent.h (formerly PHContent.h)
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/31/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/** @file */
/**
 * Content view transition type: currently two are supported.
 **/
typedef enum {
    PHContentTransitionUnknown,
    PHContentTransitionModal, /**< PHContentTransitionModal (PH_MODAL): full-screen modal view controller that slides up from the bottom of the screen. */
    PHContentTransitionDialog /**< PHContentTransitionDialog (PH_DIALOG): an arbitrarily sized and positioned content unit view that 'pops' into view like an alert view */
} PHContentTransitionType;

/**
 * @internal
 *
 * @brief Content unit definition. This class separates out the usable components of
 * the content request response so that they can be used by other classes in the
 * SDK
 **/
@interface PHContent : NSObject {
    NSDictionary           *_frameDict;
    NSURL                  *_URL;
    PHContentTransitionType _transition;
    NSDictionary           *_context;
    NSTimeInterval          _closeButtonDelay;
    NSString               *_closeButtonURLPath;
}

/**
 * Returns a PHContent instance iff the response dictionary
 * \c dictionaryRepresentation has valid values for all required keys, otherwise
 * returns nil
 *
 * @param dictionaryRepresentation
 *   The response dictionary
 *
 * @return
 *   A PHContent instance
 **/
+ (id)contentWithDictionary:(NSDictionary *)dictionaryRepresentation;


@property (nonatomic, retain) NSURL         *URL;                 /**< Content template URL */
@property (nonatomic, retain) NSDictionary  *context;             /**< Content unit context object, this is what is sent to the
                                                                       content template after a ph://loadContext dispatch */
@property (nonatomic, assign) NSTimeInterval closeButtonDelay;    /**< Amount of time (in seconds) to wait after displaying the
                                                                       overlay before showing the native close button */
@property (nonatomic, copy)   NSString      *closeButtonURLPath;  /**< The URL that should be pinged when the native close button is used */
@property (nonatomic, assign) PHContentTransitionType transition; /**< Transition type, see PHContentTransitionType above */

/**
 * Returns a CGRect if this content unit has a valid frame for \c orientation, otherwise returns CGRectNull
 *
 * @param orientation
 *   The orientation
 *
 * @return
 *   A CGRect or CGRectNull
 **/
- (CGRect)frameForOrientation:(UIInterfaceOrientation)orientation;

/**
 * Allows for manually setting the frameDict instance variable, values in
 * frameDict are used for frameForOrientation:
 *
 * @param frameDict
 *   The new frameDict
 **/
- (void)setFramesWithDictionary:(NSDictionary *)frameDict;
@end
