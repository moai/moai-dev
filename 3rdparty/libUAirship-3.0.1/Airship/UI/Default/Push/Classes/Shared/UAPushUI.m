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

#import "UAPushUI.h"
#import "UAPushSettingsViewController.h"
#import "UAPushMoreSettingsViewController.h"

@implementation UAPushUI

SINGLETON_IMPLEMENTATION(UAPushUI)

- (id)init {
    self = [super init];
    if (self) {
        NSString *path = [[[NSBundle mainBundle] resourcePath]
                          stringByAppendingPathComponent:@"UAPushLocalization.bundle"];
        
        self.localizationBundle = [NSBundle bundleWithPath:path];
    
    }
	return self;
}

- (UIViewController *)apnsSettingsViewController {
    if (!_apnsSettingsViewController) {
        UIViewController *root = [[UAPushSettingsViewController alloc]
                                   initWithNibName:@"UAPushSettingsView"
                                            bundle:nil];
        _apnsSettingsViewController = [[UINavigationController alloc] initWithRootViewController:root];
    }
    return _apnsSettingsViewController;
}

- (UIViewController *)tokenSettingsViewController {
    if (!_tokenSettingsViewController) {
        UIViewController *root = [[UAPushMoreSettingsViewController alloc]
                                   initWithNibName:@"UAPushMoreSettingsView"
                                            bundle:nil];
        _tokenSettingsViewController = [[UINavigationController alloc] initWithRootViewController:root];
    }
    return _tokenSettingsViewController;
}

+ (void)openApnsSettings:(UIViewController *)viewController
                animated:(BOOL)animated {
    [viewController presentModalViewController:[UAPushUI shared].apnsSettingsViewController
                                      animated:animated];
}

+ (void)openTokenSettings:(UIViewController *)viewController
                 animated:(BOOL)animated {
    [viewController presentModalViewController:[UAPushUI shared].tokenSettingsViewController
                                      animated:animated];
}

+ (void)closeApnsSettingsAnimated:(BOOL)animated {
    [[UAPushUI shared].apnsSettingsViewController dismissModalViewControllerAnimated:animated];
}

+ (void)closeTokenSettingsAnimated:(BOOL)animated {
    [[UAPushUI shared].tokenSettingsViewController dismissModalViewControllerAnimated:animated];
}


@end
