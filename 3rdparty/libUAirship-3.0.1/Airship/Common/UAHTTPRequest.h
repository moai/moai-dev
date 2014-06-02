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

@class UAHTTPRequest;
typedef void (^UAHTTPConnectionSuccessBlock)(UAHTTPRequest *request);
typedef void (^UAHTTPConnectionFailureBlock)(UAHTTPRequest *request);

/**
 * Wraps NSURLRequest, to be used in conjunction with UAHTTPConnection.
 */
@interface UAHTTPRequest : NSObject

/**
 * The URL for the request.
 */
@property (nonatomic, readonly, strong) NSURL *url;

/**
 * The dictionary containing the request's header fields.
 */
@property (nonatomic, strong) NSDictionary *headers;

/**
 * The HTTP request method.
 */
@property (nonatomic, copy) NSString *HTTPMethod;

/**
 * The user name for basic authorization.
 */
@property (nonatomic, copy) NSString *username;

/**
 * The user password for basic authorization.
 */
@property (nonatomic, copy) NSString *password;

@property (nonatomic, strong) NSURL *mainDocumentURL;
/**
 * The body of the request.
 */
@property (nonatomic, strong) NSMutableData *body;

/**
 * Boolean to compress the request's body.
 * @return YES will enable GZIP and compress the body.
 * @return NO will not compress the body.
 */
@property (nonatomic, assign) BOOL compressBody;

/**
 * Contextual data containing optional user info for access later.
 */
@property (nonatomic, strong) id userInfo;

/**
 * The response.
 */
@property (nonatomic, readonly, strong) NSHTTPURLResponse *response;

/**
 * The response string.
 */
@property (nonatomic, readonly, copy) NSString *responseString;

/**
 * The response data.
 */
@property (nonatomic, readonly, strong) NSData *responseData;

/**
 * The error related to the request.
 */
@property (nonatomic, readonly, strong) NSError *error;

/**
 * Create a request with the URL string.
 * @param urlString The URL string.
 */
+ (UAHTTPRequest *)requestWithURLString:(NSString *)urlString;

/**
 * Create a request with the URL.
 * @param url The URL.
 */
+ (UAHTTPRequest *)requestWithURL:(NSURL *)url;

/**
 * Set the default user agent.
 * @param userAgent The user agent string.
 */
+ (void)setDefaultUserAgentString:(NSString *)userAgent;

/**
 * UAHTTPRequest initializer taking a URL string.
 * @param urlString The URL string.
 */
- (id)initWithURLString:(NSString *)urlString;

/**
 * UAHTTPRequest initializer taking a URL.
 * @param url The URL.
 */
- (id)initWithURL:(NSURL *)url;

/**
 * Add a request header.
 * @param header The header string to be added.
 * @param value The value string to be added.
 */
- (void)addRequestHeader:(NSString *)header value:(NSString *)value;

/**
 * Append data to the body.
 * @param data The data to be added to the body.
 */
- (void)appendBodyData:(NSData *)data;

@end

