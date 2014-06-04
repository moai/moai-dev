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

#import "UAirship.h"
#import "UAPush.h"
#import "UAPushUI.h"
#import "UAPushSettingsViewController.h"
#import "UALocationService.h"

// Overall counts for sectioned table view
enum {
    SectionPushEnabled = 0,
    SectionAirshipLocationEnabled = 1,
    SectionQuietTime   = 2,
    SectionCount       = 3
};

// The section for the push enabled switch is 0
// The row count for the push table view is 1
enum {
    PushEnabledSectionSwitchCell = 0,
    PushEnabledSectionRowCount   = 1
};

// The section for the Airship is 1
// The row count is one
//static NSUInteger AirshipLocationEnabledSectionSwitchCell = 1;
static NSUInteger AirshipLocationEnabledSectionRowCount = 1;

// Enums for the Quiet time table view
enum {
    QuietTimeSectionSwitchCell  = 0,
    QuietTimeSectionStartCell   = 1,
    QuietTimeSectionEndCell     = 2,
    QuietTimeSectionRowCount    = 3
};

@implementation UAPushSettingsViewController

#pragma mark -
#pragma mark Lifecycle methods


- (void)viewDidLoad {
    [super viewDidLoad];
    [self initViews];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)viewDidUnload {

    self.pushEnabledSwitch = nil;
    self.pushEnabledLabel = nil;
    self.pushEnabledCell = nil;
    
    self.quietTimeSwitch = nil;
    self.quietTimeLabel = nil;
    self.quietTimeEnabledCell = nil;
    self.toCell = nil;
    self.fromCell = nil;
    
    self.tableView = nil;
    self.datePicker = nil;
    self.airshipLocationEnabledSwitch = nil;
    self.airshipLocationEnabledLabel = nil;
    self.airshipLocationEnabledCell = nil;
    [super viewDidUnload];
}

- (void)viewWillAppear:(BOOL)animated {
    
    //Hide the picker if it was left up last time
    [self updateDatePicker:NO];
    [super viewWillAppear:animated];
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    
    //if shown, update picker and scroll offset
    if (self.pickerDisplayed) {
        [self updateDatePicker:YES];
    }
}

#pragma mark -
#pragma mark UITableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    if (self.pushEnabledSwitch.on) {
        return SectionCount;
    } else {
        return SectionCount - 1;
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    switch (section) {
        case SectionPushEnabled:
            return PushEnabledSectionRowCount;
        case SectionAirshipLocationEnabled:
            return AirshipLocationEnabledSectionRowCount;
        case SectionQuietTime:
        {
            if (self.pushEnabledSwitch.on && self.quietTimeSwitch.on) {
                return QuietTimeSectionRowCount;
            } else if (self.pushEnabledSwitch.on) {
                return 1;
            }
        }
        default:
            break;
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == SectionQuietTime) {
        if (indexPath.row == QuietTimeSectionSwitchCell) {
            self.quietTimeEnabledCell.selectionStyle = UITableViewCellSelectionStyleNone;
            return self.quietTimeEnabledCell;
        } else if (indexPath.row == QuietTimeSectionStartCell) {
            return self.fromCell;
        } else {
            return self.toCell;
        }
    } else if (indexPath.section == SectionPushEnabled) {
        return self.pushEnabledCell;
    } else if (indexPath.section == SectionAirshipLocationEnabled) {
        return self.airshipLocationEnabledCell;
    }
    return nil;
}

#pragma mark -
#pragma mark UITableVieDelegate Methods
- (void)tableView:(UITableView *)view didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row == 1 || indexPath.row == 2) {
        [self updateDatePicker:YES];
    } else {
        [self updateDatePicker:NO];
    }
}

// UA_Push_Settings_Location_Enabled_Label
#pragma mark -
#pragma mark logic

- (void)initViews {
    self.title = UA_PU_TR(@"UA_Push_Settings_Title");
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                            target:self
                                                                                            action:@selector(quit)];

    UIRemoteNotificationType type = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
    if (type == UIRemoteNotificationTypeNone || ![UAPush shared].pushEnabled) {
        self.pushEnabledSwitch.on = NO;
    } else {
        self.pushEnabledSwitch.on = YES;
    }
    if ([UALocationService airshipLocationServiceEnabled]) {
        self.airshipLocationEnabledSwitch.on = YES;
    }
    else {
        self.airshipLocationEnabledSwitch.on = NO;
    }
    
    self.pushEnabledLabel.text = UA_PU_TR(@"UA_Push_Settings_Enabled_Label");
    self.airshipLocationEnabledLabel.text = UA_PU_TR(@"UA_Push_Settings_Location_Enabled_Label");
    self.quietTimeLabel.text = UA_PU_TR(@"UA_Push_Settings_Quiet_Time_Label");
    
    self.fromCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    self.toCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    self.fromCell.textLabel.text = UA_PU_TR(@"UA_Quiet_Time_From");
    self.toCell.textLabel.text = UA_PU_TR(@"UA_Quiet_Time_To");

    
    NSDate *date1 = nil;
    NSDate *date2 = nil;
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setLocale:[[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"]];
    
    
    NSDictionary *quietTime = [[UAPush shared] quietTime];
    [formatter setDateFormat:@"HH:mm"];
    self.quietTimeSwitch.on = [UAPush shared].quietTimeEnabled;
    if (quietTime != nil) {
        UALOG(@"Quiet time dict found: %@ to %@", [quietTime objectForKey:@"start"], [quietTime objectForKey:@"end"]);
        date1 = [formatter dateFromString:[quietTime objectForKey:@"start"]];
        date2 = [formatter dateFromString:[quietTime objectForKey:@"end"]];
    }
    
    if (date1 == nil || date2 == nil) {
        date1 = [formatter dateFromString:@"22:00"];//default start
        date2 = [formatter dateFromString:@"07:00"];//default end
    }

    [formatter setLocale:[NSLocale currentLocale]];
    [formatter setDateStyle:NSDateFormatterNoStyle];
    [formatter setTimeStyle:NSDateFormatterShortStyle];
    self.fromCell.detailTextLabel.text = [formatter stringFromDate:date1];
    self.toCell.detailTextLabel.text = [formatter stringFromDate:date2];

    NSDate *now = [[NSDate alloc] init];
    [self.datePicker setDate:now animated:NO];

    self.pickerDisplayed = NO;
    self.pickerShownFrame = CGRectZero;
    self.pickerHiddenFrame = CGRectZero;

    // make our existing layout work in iOS7
    if ([self respondsToSelector:NSSelectorFromString(@"edgesForExtendedLayout")]) {
        self.edgesForExtendedLayout = UIRectEdgeNone;
    }

    [self.view setNeedsLayout];
}

- (void)updatePickerLayout {
    
    CGRect viewBounds = self.view.bounds;
    
    //Manually set the size of the picker for better landscape experience
    //Older  devies do not like the custom size. It breaks the picker.
                    
    //If the picker is in a portrait container, use std portrait picker dims
    if (viewBounds.size.height >= viewBounds.size.width) {
        self.datePicker.bounds = CGRectMake(0, 0, 320, 216);
    } else {
        self.datePicker.bounds = CGRectMake(0, 0, 480, 162);
    }
    
    // reset picker subviews
    for (UIView* subview in self.datePicker.subviews) {
        subview.frame = self.datePicker.bounds;
    }
    
    // reset the visible/hidden views
    int viewOffset = self.view.frame.origin.y;
    CGRect pickerBounds = self.datePicker.bounds;
    self.pickerShownFrame = CGRectMake(0, viewOffset+viewBounds.size.height-pickerBounds.size.height,
                                  viewBounds.size.width, pickerBounds.size.height);
    self.pickerHiddenFrame = CGRectMake(0, viewOffset+viewBounds.size.height,
                                   viewBounds.size.width, pickerBounds.size.height);
    
    //reset actual frame
    if (self.pickerDisplayed) {
        self.datePicker.frame = self.pickerShownFrame;
    } else {
        self.datePicker.frame = self.pickerHiddenFrame;
    }
}

- (IBAction)quit {
    
    if (self.dirty) {
        
        [UAPush shared].pushEnabled = self.pushEnabledSwitch.on;
        
        if (self.pushEnabledSwitch.on) {
            [self updateQuietTime];
        }
        
        self.dirty = NO;
    }
    
    [UAPush closeApnsSettingsAnimated:YES];
}

- (IBAction)pickerValueChanged:(id)sender {

    self.dirty = YES;
    
    NSDate *date = [self.datePicker date];
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateStyle:NSDateFormatterNoStyle];
    [formatter setTimeStyle:NSDateFormatterShortStyle];
    
    NSInteger row = [[self.tableView indexPathForSelectedRow] row];
    if (row == QuietTimeSectionStartCell) {
        self.fromCell.detailTextLabel.text = [formatter stringFromDate:date];
        [self.fromCell setNeedsLayout];
    } else if (row == QuietTimeSectionEndCell) {
        self.toCell.detailTextLabel.text = [formatter stringFromDate:date];
        [self.toCell setNeedsLayout];
    } else {
        NSDate *now = [[NSDate alloc] init];
        [self.datePicker setDate:now animated:YES];
        return;
    }

}

- (IBAction)switchValueChanged:(id)sender {
    
    self.dirty = YES;
    
    if (!self.quietTimeSwitch.on || !self.pushEnabledSwitch.on) {
        [self updateDatePicker:NO];
    }
    [self.tableView reloadData];
    
    if (self.airshipLocationEnabledSwitch.on){
        [UALocationService setAirshipLocationServiceEnabled:YES];
    }
    else {
        [UALocationService setAirshipLocationServiceEnabled:NO];
    }

}

- (void)updateDatePicker:(BOOL)show {
    
    [self updatePickerLayout];
    
    [UIView beginAnimations:nil context:nil];
    [UIView setAnimationDuration:0.4];
    if (show) {
        [self.view addSubview:self.datePicker];
        self.pickerDisplayed = YES;
        self.datePicker.frame = self.pickerShownFrame;
        
        //Scroll the table view so the "To" field is just above the top of the data picker
        int scrollOffset = MAX(0, 
                               self.toCell.frame.origin.y
                               + self.toCell.frame.size.height
                               + self.tableView.sectionFooterHeight
                               - self.datePicker.frame.origin.y);
        self.tableView.contentOffset = CGPointMake(0, scrollOffset);
    } else {
        self.pickerDisplayed = NO;
        self.tableView.contentOffset = CGPointZero;//reset scroll offset
        self.datePicker.frame = self.pickerHiddenFrame;
        [self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:NO];
    }
    [UIView commitAnimations];
    
    //remove picker display after animation
    if (!self.pickerDisplayed) {
        [self.datePicker removeFromSuperview];
    }

    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateStyle:NSDateFormatterNoStyle];
    [formatter setTimeStyle:NSDateFormatterShortStyle];
    
    NSString *fromString = self.fromCell.detailTextLabel.text;
    NSString *toString = self.toCell.detailTextLabel.text;

    NSInteger row = [[self.tableView indexPathForSelectedRow] row];
    if (row == 1 && [fromString length] != 0) {
        NSDate *fromDate = [formatter dateFromString:fromString];
        [self.datePicker setDate:fromDate animated:YES];
    } else if (row == 2 && [toString length] != 0) {
        NSDate *toDate = [formatter dateFromString:toString];
        [self.datePicker setDate:toDate animated:YES];
    }
}

- (void)updateQuietTime {
    
    if (self.quietTimeSwitch.on) {
        
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateStyle:NSDateFormatterNoStyle];
        [formatter setTimeStyle:NSDateFormatterShortStyle];
        
        NSString *fromString = self.fromCell.detailTextLabel.text;
        NSString *toString = self.toCell.detailTextLabel.text;
        NSDate *fromDate = [formatter dateFromString:fromString];
        NSDate *toDate = [formatter dateFromString:toString];
                
        [UAPush shared].quietTimeEnabled = YES;
        [[UAPush shared] setQuietTimeFrom:fromDate to:toDate withTimeZone:[NSTimeZone localTimeZone]];
        [[UAPush shared] updateRegistration];
    } else {
        [UAPush shared].quietTimeEnabled = NO;
        [[UAPush shared] updateRegistration];
    }


}

@end
