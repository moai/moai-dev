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

#import "UAGlobal.h"
#import "UAPushSettingsSoundsViewController.h"

#import <AudioToolbox/AudioServices.h>

#if __IPHONE_OS_VERSION_MAX_ALLOWED < 60000
// This is available in iOS 6.0 and later, define it for older versions
#define NSLineBreakByWordWrapping 0
#endif

enum {
    SectionDesc     = 0,
    SectionSounds   = 1,
    SectionCount    = 2
};

enum {
    DescSectionText     = 0,
    DescSectionRowCount = 1
};

@implementation UAPushSettingsSoundsViewController

#pragma mark -
#pragma mark View lifecycle


- (void)viewDidLoad {
    [super viewDidLoad];

    self.title = @"Bundled Sounds";
    
    if (self.soundList == nil) {

        NSArray *aiffFiles = [[NSBundle mainBundle] pathsForResourcesOfType:@"aiff" inDirectory:nil];
        NSArray *cafFiles = [[NSBundle mainBundle] pathsForResourcesOfType:@"caf" inDirectory:nil];
        NSArray *wavFiles = [[NSBundle mainBundle] pathsForResourcesOfType:@"wav" inDirectory:nil];

                              
        self.soundList = [[NSMutableArray alloc] init];
        [self.soundList addObjectsFromArray:aiffFiles];
        [self.soundList addObjectsFromArray:cafFiles];
        [self.soundList addObjectsFromArray:wavFiles];
        
    }
    
    self.textLabel.text = @"Notifications can optionally trigger a custom sound upon receipt. "
    @"Send a sound with your push by including the sound's filename in the Urban Airship push form. "
    @"This sample application includes the sound files below.";
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return SectionCount;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    switch (section) {
        case SectionSounds:
            return [self.soundList count];
        case SectionDesc:
            return DescSectionRowCount;
        default:
            break;
    }
    return 0;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    UITableViewCell *cell = nil;
    
    switch (indexPath.section) {
        case SectionDesc:
        {
            cell = self.textCell;
            break;
        }
        case SectionSounds:
        {
            static NSString *CellIdentifier = @"SoundCell";
            
            cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
            if (cell == nil) {
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
            }
            
            // Configure the cell...
            cell.textLabel.text = [[[self.soundList objectAtIndex:indexPath.row] pathComponents] lastObject];
            break;
        }
        default:
            break;
    }
    
    return cell;
}

#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {

    if (indexPath.section == SectionSounds) {
        SystemSoundID soundID;
        AudioServicesCreateSystemSoundID((__bridge CFURLRef)[NSURL fileURLWithPath:[self.soundList objectAtIndex:indexPath.row]], &soundID);
        AudioServicesPlayAlertSound(soundID);
        
        [tableView deselectRowAtIndexPath:indexPath animated:YES];
    }
}

#define kCellPaddingHeight 10

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.section == SectionDesc) {
        CGFloat height = [self.textLabel.text sizeWithFont:self.textLabel.font
                          constrainedToSize:CGSizeMake(240, 1500)
                              lineBreakMode:NSLineBreakByWordWrapping].height;
        return height + kCellPaddingHeight * 2;
    } else {
        return 44;
    }
}


#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
    
    self.textCell = nil;
    self.textLabel = nil;
}




@end

