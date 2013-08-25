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

 PHPublisherSubcontentRequest.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 5/19/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import "PHAPIRequest.h"

@class PHContentView;

/**
 * @internal
 *
 * @brief Request for handling subcontent requests during a PHPublisherContentRequest
 * session. Subcontent requests require a fully qualified and presigned content
 * request URL.
 **/
@interface PHPublisherSubContentRequest : PHAPIRequest {
    PHContentView *_source;
    NSString      *_callback;
}

@property (nonatomic, assign) PHContentView *source;   /**< Originating content view instance */
@property (nonatomic, copy)   NSString      *callback; /**< Originating callback id */
@end
