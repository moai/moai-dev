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

 RootViewController.h
 example

 Created by Jesus Fernandez on 4/25/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <UIKit/UIKit.h>

@interface RootViewController : UITableViewController {
    UITextField *tokenField;
    UITextField *secretField;
}
@property (nonatomic, retain) IBOutlet UITextField *tokenField;
@property (nonatomic, retain) IBOutlet UITextField *secretField;
@property (retain, nonatomic) IBOutlet UISwitch    *optOutStatusSlider;
@property (retain, nonatomic) IBOutlet UILabel     *serviceURLField;

- (void)touchedToggleStatusBar:(id)sender;
- (IBAction)touchedOptOutStatusSlider:(id)sender;
@end
