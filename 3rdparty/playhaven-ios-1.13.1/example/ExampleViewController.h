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

 ExampleViewController.h
 playhaven-sdk-ios

 Created by Jesus Fernandez on 4/25/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <UIKit/UIKit.h>


@interface ExampleViewController : UIViewController <UITableViewDelegate, UITableViewDataSource> {
    NSMutableArray *_messages;
    UITableView    *_tableView;

    NSString *_token;
    NSString *_secret;
    NSDate   *_startRequestDate;
    NSDate   *_deltaRequestDate;
}
@property (nonatomic, retain) IBOutlet UITableView *tableView;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *secret;

- (void)addMessage:(NSString *)message;
- (void)addElapsedTime;

- (void)startRequest;
- (void)finishRequest;

- (void)startTimers;
@end
