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

 PHReward.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 7/11/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <Foundation/Foundation.h>

/**
 * @internal
 *
 * @brief Class that contains information about an unlocked reward.
 **/
// TODO: Move validation methods from PHPublisherContentRequest to here
@interface PHReward : NSObject {
    NSString *_reward;
    NSInteger _quantity;
    NSString *_receipt;
}

@property (nonatomic, copy)   NSString *name;     /**< Reward name, this is a string value that is configured on the publisher Dashboard
                                                       and should correspond to unlockables recognized by the publisher's game */
@property (nonatomic, assign) NSInteger quantity; /**< Quantity of the reward to unlock */
@property (nonatomic, copy)   NSString *receipt;  /**< Unique receipt value, used for reward validation in the SDK */
@end
