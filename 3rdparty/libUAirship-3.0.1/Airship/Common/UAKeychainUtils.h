/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC ``AS IS'' AND ANY EXPRESS OR
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

#define kUAKeychainDeviceIDKey @"com.urbanairship.deviceID"

/**
 * The UAKeychainUtils object provides an interface for keychain related methods.
 */
@interface UAKeychainUtils : NSObject

/**
 * Creates a key chain.
 * @param username The username for the key chain.
 * @param password The password for the key chain.
 * @param identifier The identifier for the key chain.
 * @return YES if the key chain was created successfully. NO otherwise.
 */
+ (BOOL)createKeychainValueForUsername:(NSString *)username 
                          withPassword:(NSString *)password 
                         forIdentifier:(NSString *)identifier;

/**
 * Deletes a key chain.
 * @param identifier The identifier to specify the key chain to be deleted.
 */
+ (void)deleteKeychainValue:(NSString *)identifier;

/**
 * Updates a key chain.
 * @param username The username for the key chain.
 * @param password The password for the key chain.
 * @param identifier The identifier for the key chain.
 * @return YES if the key chain was updated successfully. NO otherwise.
 */
+ (BOOL)updateKeychainValueForUsername:(NSString *)username
                          withPassword:(NSString *)password
                         forIdentifier:(NSString *)identifier;

/**
 * Get the key chain's password.
 * @param identifier The identifier for the key chain.
 * @return The password as an NSString or an empty string if an error occurred.
 */
+ (NSString *)getPassword:(NSString *)identifier;

/**
 * Get the key chain's username.
 * @param identifier The identifier for the key chain.
 * @return The username as an NSString or an empty string if an error occurred.
 */
+ (NSString *)getUsername:(NSString *)identifier;

/**
 * Gets the device ID, creating or refreshing if necessary. Device IDs will be regenerated if a
 * device change is detected (though UAUser IDs remain the same in that case).
 *
 * @return The Urban Airship device ID or an empty string if an error occurred.
 */
+ (NSString *)getDeviceID;

@end
