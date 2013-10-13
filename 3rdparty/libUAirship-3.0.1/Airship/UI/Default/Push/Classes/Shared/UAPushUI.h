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

#import <Foundation/Foundation.h>
#import "UAPush.h"

#define UA_PU_TR(key) [[UAPushUI shared].localizationBundle localizedStringForKey:key value:@"" table:nil]

/**
 * The default implementation provided in the library's sample UI distribution.
 */

@interface UAPushUI : NSObject<UAPushUIProtocol>

@property (nonatomic, strong) UIViewController *apnsSettingsViewController;
@property (nonatomic, strong) UIViewController *tokenSettingsViewController;
@property (nonatomic, strong) NSBundle *localizationBundle;

SINGLETON_INTERFACE(UAPushUI)

/**
 * Open the push token demo screen. The default implementation provides a UI for vieweing and
 * managing device token metadata.
 *
 * @param viewController The parent view controller.
 * @param animated `YES` to animate the display, otherwise `NO`
 */
+ (void)openTokenSettings:(UIViewController *)viewController
                 animated:(BOOL)animated;

/**
 * Close the push token demo screen.
 *
 * @param animated `YES` to animate the view transition, otherwise `NO`
 */
 + (void)closeTokenSettingsAnimated:(BOOL)animated;

@end
