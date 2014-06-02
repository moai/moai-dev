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

#import "UAPushSettingsAddTagViewController.h"
#import "UAPush.h"
#import "UATagUtils.h"

#if __IPHONE_OS_VERSION_MAX_ALLOWED < 60000
// This is available in iOS 6.0 and later, define it for older versions
#define NSLineBreakByWordWrapping 0
#endif

enum TagSections {
    TagSectionCustom = 0,
    TagSectionPreset = 1,
    TagSectionCount = 2
};


@implementation UAPushSettingsAddTagViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"New Tag";
    
    self.tagField.text = @"";
    
    if (!self.presetTags) {
        self.presetTags = [UATagUtils createTags:
                           UATagTypeCountry|UATagTypeDeviceType|UATagTypeLanguage|UATagTypeTimeZone|UATagTypeTimeZoneAbbreviation];
    }
    
    //Create an add button in the nav bar
    if (self.cancelButton == nil) {
        self.cancelButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(cancel:)];
    }
    self.navigationItem.leftBarButtonItem = self.cancelButton;
    
    if (self.saveButton == nil) {
        self.saveButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(save:)];
    }
    self.navigationItem.rightBarButtonItem = self.saveButton;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)viewDidUnload {
    [super viewDidUnload];
}

#pragma mark -
#pragma mark UITableViewDelegate

#define kCellPaddingHeight 11

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *text;
    if (indexPath.section == TagSectionCustom) {
        text = @"Custom Tag";
    } else {
        text = [self.presetTags objectAtIndex:indexPath.row];
    }

    CGFloat height = [text sizeWithFont:self.tagField.font
                      constrainedToSize:CGSizeMake(240, 1500)
                          lineBreakMode:NSLineBreakByWordWrapping].height;

    return height + kCellPaddingHeight * 2;
}

- (void)tableView:(UITableView *)view didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

    if (indexPath.section == TagSectionPreset) {
        [self.tagDelegate addTag:[self.presetTags objectAtIndex:indexPath.row]];
        [view deselectRowAtIndexPath:indexPath animated:YES];
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    
    switch (section) {
        case TagSectionCustom:
            return @"Custom Tag";
        case TagSectionPreset:
            return @"Common Tags";
        default:
            break;
    }
    return nil;
}


#pragma mark -
#pragma mark UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    switch (section) {
        case TagSectionCustom:
            return 1;
        case TagSectionPreset:
            return [self.presetTags count];
        default:
            break;
    }
    
    return 0;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return TagSectionCount;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    switch (indexPath.section) {
        case TagSectionCustom:
            return self.tagCell;
        case TagSectionPreset:
        {
            UITableViewCell *cell;

            static NSString *CellIdentifier = @"PresetTagCell";
            
            cell = [self.tableView dequeueReusableCellWithIdentifier:CellIdentifier];
            if (cell == nil) {
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
            }
            
            // Configure the cell...
            
            cell.textLabel.text = [self.presetTags objectAtIndex:indexPath.row];
            cell.accessoryType = UITableViewCellAccessoryNone;
            
            return cell;
        }
        default:
            break;
    }
    
    return nil;
}

#pragma mark -
#pragma mark UITextFieldDelegate Methods

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField {
    [textField resignFirstResponder];
}

#pragma mark -
#pragma mark Save/Cancel

- (void)save:(id)sender {
    [self.tagDelegate addTag:self.tagField.text];
    self.tagField.text = nil;
}

- (void)cancel:(id)sender {
    [self.tagDelegate cancelAddTag];
    self.tagField.text = nil;
}
    

@end
