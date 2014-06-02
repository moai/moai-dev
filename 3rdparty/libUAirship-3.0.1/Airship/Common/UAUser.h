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

extern NSString * const UAUserCreatedNotification;

/**
 * Primary interface for working with the application's associated UA user.
 */
@interface UAUser : NSObject

/**
 * Returns the singleton user instance.
 */
+ (UAUser *)defaultUser;

/**
 * Causes the user instance to stop listening for device token changes.
 */
+ (void)land;

/**
 * Indicates whether the default user has been created.
 * @return `YES` if the user has been created, `NO` otherwise.
 */
- (BOOL)defaultUserCreated;

/**
 * Loads the user from disk if available, otherwise creates the user (asynchronously) from scratch.
 */
- (void)initializeUser;

/**
 * The user name.
 */
@property (nonatomic, readonly, copy) NSString *username;
/**
 * The user password.
 */
@property (nonatomic, readonly, copy) NSString *password;
/**
 * The user url.
 */
@property (nonatomic, readonly, copy) NSString *url;

@end