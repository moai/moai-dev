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

#import "UAPushSettingsTagsViewController.h"
#import "UAPushSettingsAddTagViewController.h"
#import "UAPush.h"

#if __IPHONE_OS_VERSION_MAX_ALLOWED < 60000
// This is available in iOS 6.0 and later, define it for older versions
#define NSLineBreakByWordWrapping 0
#endif

enum {
    SectionDesc     = 0,
    SectionTags     = 1,
    SectionCount    = 2
};

enum {
    DescSectionText     = 0,
    DescSectionRowCount = 1
};

@implementation UAPushSettingsTagsViewController

#pragma mark -
#pragma mark View lifecycle


- (void)viewDidLoad {
    [super viewDidLoad];

    self.title = @"Tags";
    
    //Create an add button in the nav bar
    if (self.addButton == nil) {
        self.addButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addItem:)];
    }
    self.navigationItem.rightBarButtonItem = self.addButton;
    
    self.textLabel.text = @"Assign tags to a device to simplify "
    @"the process of sending notifications. Define custom tags, or use UATagUtils to "
    @"generate commonly used tags.";
}

- (void)viewWillAppear:(BOOL)animated {
    
    //default to editing, since the view is for adding/removing tags
    [self setEditing:YES];
    [self.tableView reloadData];
    [super viewWillAppear:animated];
    
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
        case SectionTags:
            return [[UAPush shared].tags count];
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
        case SectionTags:
        {
            static NSString *CellIdentifier = @"TagCell";
            
            cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
            if (cell == nil) {
                cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
            }
            
            // Configure the cell...
            
            cell.textLabel.text = [[UAPush shared].tags objectAtIndex:indexPath.row];
            cell.accessoryType = UITableViewCellAccessoryNone;
            break;
        }
        default:
            break;
    }

    return cell;
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath {
    return UITableViewCellEditingStyleDelete;
}


// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {

    if (indexPath.section == SectionTags) {
        return YES;
    } else {
        return NO;
    }
    
}




// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        
        NSString *tagToDelete = [[UAPush shared].tags objectAtIndex:indexPath.row];
        
        // Commit to server
        [[UAPush shared] removeTagFromCurrentDevice:tagToDelete];
        [[UAPush shared] updateRegistration];
        
        // Delete the row from the view
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
        
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    
    switch (section) {
        case SectionTags:
            return @"Current Tags";
        default:
            break;
    }
    return nil;
}

#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    // do nothing
}

#define kCellPaddingHeight 10

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSString *text;
    
    if (indexPath.section == SectionDesc) {
        text = self.textLabel.text;
    } else {
        text = [[UAPush shared].tags objectAtIndex:indexPath.row];
    }
    
    CGFloat height = [text sizeWithFont:self.textLabel.font
                                     constrainedToSize:CGSizeMake(240, 1500)
                                         lineBreakMode:NSLineBreakByWordWrapping].height;
    
    return height + kCellPaddingHeight * 2;

}

#pragma mark -
#pragma mark Add Item

- (void)addItem:(id)sender {
    
    if (!self.addTagController) {
        self.addTagController = [[UAPushSettingsAddTagViewController alloc] init];
        self.addTagController.tagDelegate = self;
    }
    
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:self.addTagController];
    [[self navigationController] presentModalViewController:navigationController animated:YES];
}


 - (void)addTag:(NSString *)tag {
     
     [[self navigationController] dismissModalViewControllerAnimated:YES];
     
     if ([[UAPush shared].tags containsObject:tag]) {
         UALOG(@"Tag %@ already exists.", tag);
         return;
     }
     
     if ([[tag stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] == 0) {
         UALOG(@"Tag is an empty string.");
         return;
     }

     // Add a tag to the end of the existing set of tags
     NSMutableArray* tagUpdate = [NSMutableArray arrayWithArray:[[UAPush shared] tags]];
     [tagUpdate addObject:tag];
     [[UAPush shared] setTags:tagUpdate];

     // Update the tableview
     NSInteger index = [[UAPush shared].tags count] -1;
     NSArray *indexArray = [NSArray arrayWithObject:[NSIndexPath indexPathForRow:index inSection:SectionTags]];
     [self.tableView insertRowsAtIndexPaths:indexArray withRowAnimation:UITableViewRowAnimationTop];

     // Update the registration
     [[UAPush shared] updateRegistration];
 }
 
 - (void)cancelAddTag {
     [[self navigationController] dismissModalViewControllerAnimated:YES];
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

    self.addTagController.tagDelegate = nil;
    self.addTagController = nil;
    
    self.textCell = nil;
    self.textLabel = nil;
    self.addButton = nil;

    [super viewDidUnload];
}


- (void)dealloc {    
    self.addTagController.tagDelegate = nil;
}


@end

