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

 PublisherIAPTrackingViewController.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 2/23/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <UIKit/UIKit.h>
#import "ExampleViewController.h"
#import "PlayHavenSDK.h"

@interface PublisherIAPTrackingViewController : ExampleViewController <PHAPIRequestDelegate>
@property (retain, nonatomic) IBOutlet UITextField *productField;
@property (retain, nonatomic) IBOutlet UITextField *quantityField;
@property (retain, nonatomic) IBOutlet UITextField *receiptField;
@property (retain, nonatomic) IBOutlet UISegmentedControl *resolutionSegment;
@end
