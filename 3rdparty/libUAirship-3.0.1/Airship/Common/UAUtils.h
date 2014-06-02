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

@class UAHTTPRequest;

/**
 * The UAUtils object provides an interface for utility methods.
 */
@interface UAUtils : NSObject

///---------------------------------------------------------------------------------------
/// @name Digest/Hash Utils
///---------------------------------------------------------------------------------------

+ (NSString *)md5:(NSString *)sourceString;

///---------------------------------------------------------------------------------------
/// @name Device ID Utils
///---------------------------------------------------------------------------------------

/**
 * Generate a UUID.
 * Uses CFUUID to generate and return a UUID.
 *
 * @return A UUID.
 */
+ (NSString *)UUID;

/**
 * Get the device model name. e.g., iPhone3,1
 * @return The device model name.
 */
+ (NSString *)deviceModelName;

/**
 * Gets the Urban Airship Device ID.
 *
 * @return The device ID, or an empty string if the ID cannot be retrieved or created.
 */
+ (NSString *)deviceID;

///---------------------------------------------------------------------------------------
/// @name URL Encoding
///---------------------------------------------------------------------------------------

+ (NSString *)urlEncodedStringWithString:(NSString *)string encoding:(NSStringEncoding)encoding;

///---------------------------------------------------------------------------------------
/// @name UAHTTP Authenticated Request Helpers
///---------------------------------------------------------------------------------------

+ (UAHTTPRequest *)UAHTTPUserRequestWithURL:(NSURL *)url method:(NSString *)method;

+ (UAHTTPRequest *)UAHTTPRequestWithURL:(NSURL *)url method:(NSString *)method;

+ (void)logFailedRequest:(UAHTTPRequest *)request withMessage:(NSString *)message;

/**
 * Returns a basic auth header string.
 *
 * The return value takes the form of: `Basic [Base64 Encoded "username:password"]`
 *
 * @return An HTTP Basic Auth header string value for the user's credentials.
 */
+ (NSString *)userAuthHeaderString;

///---------------------------------------------------------------------------------------
/// @name UI Formatting Helpers
///---------------------------------------------------------------------------------------

+ (NSString *)pluralize:(int)count 
           singularForm:(NSString*)singular
             pluralForm:(NSString*)plural;

+ (NSString *)getReadableFileSizeFromBytes:(double)bytes;

///---------------------------------------------------------------------------------------
/// @name Date Formatting
///---------------------------------------------------------------------------------------

/**
 * Creates an ISO dateFormatter (UTC) with the following attributes:
 * locale set to 'en_US_POSIX', timestyle set to 'NSDATEFormatterFullStyle',
 * date format set to 'yyyy-MM-dd HH:mm:ss'.
 *
 * @return A DateFormatter with the default attributes.
 */
+ (NSDateFormatter *)ISODateFormatterUTC;

///---------------------------------------------------------------------------------------
/// @name File management
///---------------------------------------------------------------------------------------

/**
 * Sets a file or directory at a url to not backup in
 * iCloud or iTunes
 * @param url The items url
 * @return YES if successful, NO otherwise
 */
+ (BOOL)addSkipBackupAttributeToItemAtURL:(NSURL *)url;


@end
