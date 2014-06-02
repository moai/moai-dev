/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <UIKit/UIKit.h>

@interface UAPushSettingsViewController : UIViewController <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, strong) IBOutlet UITableView *tableView;
@property (nonatomic, strong) IBOutlet UIDatePicker *datePicker;
@property (nonatomic, assign) CGRect pickerShownFrame;
@property (nonatomic, assign) CGRect pickerHiddenFrame;


@property (nonatomic, strong) IBOutlet UITableViewCell *pushEnabledCell;
@property (nonatomic, strong) IBOutlet UILabel *pushEnabledLabel;
@property (nonatomic, strong) IBOutlet UISwitch *pushEnabledSwitch;

@property (nonatomic, strong) IBOutlet UITableViewCell *quietTimeEnabledCell;
@property (nonatomic, strong) IBOutlet UILabel *quietTimeLabel;
@property (nonatomic, strong) IBOutlet UISwitch *quietTimeSwitch;
@property (nonatomic, strong) UITableViewCell *fromCell;
@property (nonatomic, strong) UITableViewCell *toCell;

@property (nonatomic, strong) IBOutlet UISwitch *airshipLocationEnabledSwitch;
@property (nonatomic, strong) IBOutlet UILabel *airshipLocationEnabledLabel;
@property (nonatomic, strong) IBOutlet UITableViewCell *airshipLocationEnabledCell;


@property (nonatomic, assign) BOOL dirty;
@property (nonatomic, assign) BOOL pickerDisplayed;


- (IBAction)quit;
- (IBAction)pickerValueChanged:(id)sender;
- (IBAction)switchValueChanged:(id)sender;

// Private Methods
- (void)initViews;
- (void)updatePickerLayout;
- (void)updateDatePicker:(BOOL)show;
- (void)updateQuietTime;


@end
