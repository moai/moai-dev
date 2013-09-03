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

 PHPublisherMetadataRequest.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 6/22/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>
#import "PHAPIRequest.h"

/**
 * @internal
 *
 * @brief Request class for getting placement metadata for a specific placement.
 * Placement metadata is used to render notification views, for instance.
 **/
@interface PHPublisherMetadataRequest : PHAPIRequest {
    NSString *_placement;
}

/**
 * Returns a metadata request for a given placement
 *
 * @param token
 *   The token
 *
 * @param secret
 *   The secret
 *
 * @param placement
 *   The placement
 *
 * @param delegate
 *   The delegate
 *
 * @return
 *   A new request
 **/
+ (id)requestForApp:(NSString *)token secret:(NSString *)secret placement:(NSString *)placement delegate:(id)delegate;

@property (nonatomic,copy) NSString *placement; /**< The placement id for this request */
@end
