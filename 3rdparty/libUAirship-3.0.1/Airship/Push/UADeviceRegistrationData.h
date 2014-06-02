
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
#import "UADeviceRegistrationPayload.h"

/**
 * Model object encapsulating the data relevant to a registration or unregistration processed by UADeviceAPIClient.
 */
@interface UADeviceRegistrationData : NSObject

/**
 * UADeviceRegistrationData initializer.
 *
 * @param token A device token string.
 * @param payload An NSDictionary representing the payload to be sent in the request body.
 * @param enabled A BOOL indicating whether push is currently enabled.
 */
- (id)initWithDeviceToken:(NSString *)token withPayload:(UADeviceRegistrationPayload *)payload pushEnabled:(BOOL)enabled;

/**
 * Autoreleased UADeviceRegistrationData class factory method.
 *
 * @param token A device token string.
 * @param payload An NSDictionary representing the payload to be sent in the request body.
 * @param enabled A BOOL indicating whether push is currently enabled.
 */
+ (id)dataWithDeviceToken:(NSString *)token withPayload:(UADeviceRegistrationPayload *)payload pushEnabled:(BOOL)enabled;

/**
 * The device token.
 */
@property(nonatomic, copy, readonly) NSString *deviceToken;
/**
 * The request payload as an NSDictionary.
 */
@property(nonatomic, strong, readonly) UADeviceRegistrationPayload *payload;
/**
 * Indicates whether push was enabled at the time the object was constructed.
 */
@property(nonatomic, assign, readonly) BOOL pushEnabled;

@end
