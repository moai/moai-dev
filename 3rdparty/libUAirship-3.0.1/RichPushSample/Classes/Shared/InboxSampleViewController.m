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
#import "InboxSampleViewController.h"
#import "InboxSampleAppDelegate.h"
#import "UAirship.h"
#import "UAInbox.h"
#import "UAInboxMessageListController.h"
#import "UAInboxMessageViewController.h"

#import "UAInboxNavUI.h"
#import "UAInboxUI.h"

#import "UAUtils.h"

@implementation InboxSampleViewController

- (IBAction)mail:(id)sender {
    [UAInbox displayInboxInViewController:self.navigationController animated:YES];
}

- (IBAction)selectInboxStyle:(id)sender {
    
    NSString *popoverOrNav;
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        popoverOrNav = @"Popover";
    }
    
    else {
        popoverOrNav = @"Navigation Controller";
    }
    
    
    UIActionSheet *sheet = [[UIActionSheet alloc] initWithTitle:@"Select Inbox Style" delegate:self 
                        cancelButtonTitle:@"Cancel" 
                   destructiveButtonTitle:nil 
                        otherButtonTitles:@"Modal", popoverOrNav, nil];
    
    [sheet showInView:self.view];
    
}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    switch (buttonIndex) {
        case 0:
            [UAInbox useCustomUI:[UAInboxUI class]];
            [UAInbox shared].pushHandler.delegate = [UAInboxUI shared];
            break;
        case 1:
            [UAInbox useCustomUI:[UAInboxNavUI class]];
            [UAInbox shared].pushHandler.delegate = [UAInboxNavUI shared];
            break;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.version.text = [NSString stringWithFormat:@"UAInbox Version: %@", [UAirshipVersion get]];
    
    self.navigationItem.rightBarButtonItem 
        = [[UIBarButtonItem alloc] initWithTitle:@"Inbox" style:UIBarButtonItemStylePlain target:self action:@selector(mail:)];
    
    // For UINavigationController UI
    [UAInboxNavUI shared].popoverButton = self.navigationItem.rightBarButtonItem;
    
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


@end
