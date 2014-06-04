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

#import "UAPushMoreSettingsViewController.h"
#import "UAirship.h"
#import "UAPush.h"
#import "UAPushUI.h"
#import "UAPushSettingsTokenViewController.h"
#import "UAPushSettingsAliasViewController.h"
#import "UAPushSettingsTagsViewController.h"
#import "UAPushSettingsSoundsViewController.h"
#import "UAPushSettingsUserInfoViewController.h"
#import "UALocationSettingsViewController.h"
#import "UAUser.h"

#define kUAPushDeviceTokenPath @"deviceToken"

enum {
    SectionDeviceToken = 0,
    SectionUser        = 1,
    SectionHelp        = 2,
    SectionLocation    = 3,
    SectionCount       = 4
};

enum {
    DeviceTokenSectionTypesCell = 0,
    DeviceTokenSectionDisabledTypesCell = 1,
    DeviceTokenSectionTokenCell = 2,
    DeviceTokenSectionAliasCell = 3,
    DeviceTokenSectionTagsCell  = 4,
    DeviceTokenSectionRowCount  = 5
};

enum {
    HelpSectionSounds   = 0,
    HelpSectionRowCount = 1
};

static NSUInteger locationRowCount = 1;
static NSUInteger userRowCount = 1;

@implementation UAPushMoreSettingsViewController

- (void)dealloc {
    [[UAPush shared] removeObserver:self];



    
    


    [[NSNotificationCenter defaultCenter] removeObserver:self.userCreatedObserver name:UAUserCreatedNotification object:nil];
    
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.title = @"Push Notification Demo";

    self.navigationItem.backBarButtonItem = [[UIBarButtonItem alloc]
                                              initWithTitle:@"Back"
                                              style:UIBarButtonItemStyleBordered
                                              target:nil
                                              action:nil];

    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]
                                              initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                              target:self
                                              action:@selector(quit)];

    // make our existing layout work beyond iOS6
    if ([self respondsToSelector:NSSelectorFromString(@"edgesForExtendedLayout")]) {
        [self setValue:[NSNumber numberWithInt:0] forKey:@"edgesForExtendedLayout"];
    }

    [self initCells];

    [[UAPush shared] addObserver:self];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    [[UAPush shared] addObserver:self forKeyPath:kUAPushDeviceTokenPath options:NSKeyValueObservingOptionNew context:nil];

    [self updateCellValues];
    [self.tableView deselectRowAtIndexPath:[self.tableView indexPathForSelectedRow] animated:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:NO];
    [[UAPush shared] removeObserver:self forKeyPath:kUAPushDeviceTokenPath];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];

    [self.tableView flashScrollIndicators];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)viewDidUnload {
    [super viewDidUnload];

    self.deviceTokenCell = nil;
    self.deviceTokenTypesCell = nil;
    self.deviceTokenDisabledTypesCell = nil;
    self.deviceTokenAliasCell = nil;
    self.deviceTokenTagsCell = nil;
    self.helpSoundsCell = nil;
    self.helpLogCell = nil;
    self.locationCell = nil;

    self.footerImageView = nil;
    self.tableView = nil;
    
}

#pragma mark -

- (void)initCells {
    self.deviceTokenCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.deviceTokenCell.textLabel.text = @"Device Token";
    self.deviceTokenCell.accessibilityLabel = @"Device Token";
    self.deviceTokenCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    self.deviceTokenTypesCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.deviceTokenTypesCell.textLabel.text = @"Notification Types";

    self.deviceTokenDisabledTypesCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.deviceTokenDisabledTypesCell.textLabel.text = @"Disabled Notification Types";
    
    self.deviceTokenAliasCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.deviceTokenAliasCell.textLabel.text = @"Alias";
    self.deviceTokenAliasCell.accessibilityLabel = @"Alias";
    self.deviceTokenAliasCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    self.deviceTokenTagsCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.deviceTokenTagsCell.textLabel.text = @"Tags";
    self.deviceTokenTagsCell.accessibilityLabel = @"Tags";
    self.deviceTokenTagsCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

    self.usernameCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:nil];
    self.usernameCell.textLabel.text = @"Username";
    self.usernameCell.accessibilityLabel = @"Username";
    self.usernameCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

    //if the user is still being created, update the cell once that is complete.
    if (![[UAUser defaultUser] defaultUserCreated]) {
        self.userCreatedObserver = [[NSNotificationCenter defaultCenter] addObserverForName:UAUserCreatedNotification object:nil queue:nil usingBlock:^(NSNotification *note){
            [self updateCellValues];
            [self.usernameCell setNeedsLayout];

            [[NSNotificationCenter defaultCenter] removeObserver:self.userCreatedObserver name:UAUserCreatedNotification object:nil];
            self.userCreatedObserver = nil;
        }];
    }

    self.helpSoundsCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    self.helpSoundsCell.textLabel.text = @"Notification Sounds";
    self.helpSoundsCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

    self.helpLogCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    self.helpLogCell.textLabel.text = @"Device Log";
    self.helpLogCell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    self.locationCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    self.locationCell.textLabel.text = @"Location";
    
    [self updateCellValues];
}

- (void)quit {
    [UAPushUI closeTokenSettingsAnimated:YES];
}

#pragma mark -
#pragma mark UITableViewDataSource Methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return SectionCount;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    switch (section) {
        case SectionDeviceToken:
            return DeviceTokenSectionRowCount;
        case SectionHelp:
            return HelpSectionRowCount;
        case SectionLocation:
            return locationRowCount;
        case SectionUser:
            return userRowCount;
        default:
            break;
    }
    return 0;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    
    switch (section) {
        case SectionDeviceToken:
            return @"Token Settings";
        case SectionHelp:
            return @"Bundle Info";
        case SectionLocation:
            return @"Location";
        case SectionUser:
            return @"User Info";
        default:
            break;
    }
    return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = nil;
    if (indexPath.section == SectionDeviceToken) {
        
        switch (indexPath.row) {
            case DeviceTokenSectionTokenCell:
                cell = self.deviceTokenCell;
                break;
            case DeviceTokenSectionTypesCell:
                cell = self.deviceTokenTypesCell;
                break;
            case DeviceTokenSectionDisabledTypesCell:
                cell = self.deviceTokenDisabledTypesCell;
                break;
            case DeviceTokenSectionAliasCell:
                cell = self.deviceTokenAliasCell;
                break;
            case DeviceTokenSectionTagsCell:
                cell = self.deviceTokenTagsCell;
                break;
            default:
                break;
        }
        
    } else if (indexPath.section == SectionUser) {
        cell = self.usernameCell ;
    } else if (indexPath.section == SectionHelp) {

        if (indexPath.row == HelpSectionSounds) {
            cell = self.helpSoundsCell;
        } 
        
    } else if (indexPath.section == SectionLocation) {
        cell = self.locationCell;
    }

    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    }
    return cell;
}

#pragma mark -
#pragma mark UITableViewDelegate Methods

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return indexPath.section == 0 ? 55 : 44;
}

- (void)tableView:(UITableView *)view didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == SectionDeviceToken) {
        if (indexPath.row == DeviceTokenSectionTokenCell) {
            if (!self.tokenViewController) {
                self.tokenViewController = [[UAPushSettingsTokenViewController alloc]
                                       initWithNibName:@"UAPushSettingsTokenView" bundle:nil];
            }
            [self.navigationController pushViewController:self.tokenViewController animated:YES];
        } else if (indexPath.row == DeviceTokenSectionAliasCell) {
            if (!self.aliasViewController) {
                self.aliasViewController = [[UAPushSettingsAliasViewController alloc]
                                       initWithNibName:@"UAPushSettingsAliasView" bundle:nil];
            }
            [self.navigationController pushViewController:self.aliasViewController animated:YES];
            
        } else if (indexPath.row == DeviceTokenSectionTagsCell) {
            if (!self.tagsViewController) {
                self.tagsViewController = [[UAPushSettingsTagsViewController alloc]
                                      initWithNibName:@"UAPushSettingsTagsViewController" bundle:nil];
            }
            [self.navigationController pushViewController:self.tagsViewController animated:YES];
            
        } else {
            [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
        }
    } else if (indexPath.section == SectionUser) {
        if (!self.userInfoViewController ) {
            self.userInfoViewController = [[UAPushSettingsUserInfoViewController alloc] initWithNibName:@"UAPushSettingsUserInfoView" bundle:nil];
        }
        [self.navigationController pushViewController:self.userInfoViewController animated:YES];
    } else if (indexPath.section == SectionHelp) {
        if (indexPath.row == HelpSectionSounds) {
            UAPushSettingsSoundsViewController *soundsViewController = [[UAPushSettingsSoundsViewController alloc] 
                                                                         initWithNibName:@"UAPushSettingsSoundsViewController" bundle:nil];
            [self.navigationController pushViewController:soundsViewController animated:YES];
        } else {
            [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
        }
        

    } else if(indexPath.section == SectionLocation) {
        UALocationSettingsViewController* locationViewController = [[UALocationSettingsViewController alloc] 
                                                                     initWithNibName:@"UALocationSettingsViewController" 
                                                                     bundle:nil];
        [self.navigationController pushViewController:locationViewController animated:YES];
        [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    }
    else {
        [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    }

}

#pragma mark -
#pragma mark KVO methods

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context {
    if ([keyPath isEqualToString:kUAPushDeviceTokenPath]) {
        [self updateCellValues];

        [self.deviceTokenCell setNeedsLayout];
        [self.deviceTokenTypesCell setNeedsLayout];
        [self.deviceTokenDisabledTypesCell setNeedsLayout];
        [self.deviceTokenAliasCell setNeedsLayout];
        [self.deviceTokenTagsCell setNeedsLayout];
    }
}


- (void)updateCellValues {
    
    self.deviceTokenCell.detailTextLabel.text = [UAPush shared].deviceToken ? [UAPush shared].deviceToken : @"Unavailable";
    self.deviceTokenTypesCell.detailTextLabel.text = [self pushTypeString:[[UIApplication sharedApplication] enabledRemoteNotificationTypes]];
    
    UIRemoteNotificationType disabledTypes = [[UIApplication sharedApplication] enabledRemoteNotificationTypes] ^ [UAPush shared].notificationTypes;
    self.deviceTokenDisabledTypesCell.detailTextLabel.text = [self pushTypeString:disabledTypes];
    
    self.deviceTokenAliasCell.detailTextLabel.text = [UAPush shared].alias ? [UAPush shared].alias : @"Not Set";
    
    if ([[UAPush shared].tags count] > 0) {
        self.deviceTokenTagsCell.detailTextLabel.text = [[UAPush shared].tags componentsJoinedByString:@", "];
    } else {
        self.deviceTokenTagsCell.detailTextLabel.text = @"None";
    }

    self.usernameCell.detailTextLabel.text = [UAUser defaultUser].username ?: @"Unavailable";
}

- (NSString *)pushTypeString:(UIRemoteNotificationType)types {
    NSMutableArray *typeArray = [NSMutableArray arrayWithCapacity:3];

    //Use the same order as the Settings->Notifications panel
    if (types & UIRemoteNotificationTypeBadge) {
        [typeArray addObject:UA_PU_TR(@"UA_Notification_Type_Badges")];
    }

    if (types & UIRemoteNotificationTypeAlert) {
        [typeArray addObject:UA_PU_TR(@"UA_Notification_Type_Alerts")];
    }

    if (types & UIRemoteNotificationTypeSound) {
        [typeArray addObject:UA_PU_TR(@"UA_Notification_Type_Sounds")];
    }

    if ([typeArray count] > 0) {
        return [typeArray componentsJoinedByString:@", "];
    }

    return UA_PU_TR(@"None");
}

@end
