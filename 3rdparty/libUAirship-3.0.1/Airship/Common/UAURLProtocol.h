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


/**
 * A NSURLProtocol that caches successful responses to requests
 * who's URL or mainDocumentURL has been added as a cachableURL.
 * A failed response will always fall back to a cached response 
 * when available.
 */
@interface UAURLProtocol : NSURLProtocol

#define kUASkipProtocolHeader @"UA-Client-Protocol"
#define kUACacheMemorySizeInMB  5 * 1024 * 1024 // 5 MB

/**
 * Adds a URL to be handled and cached by the Protocol.
 *
 *@param url The URL or mainDocumentURL of a request to cache.
 */
+ (void)addCachableURL:(NSURL *)url;

/**
 * Removes a URL from being cached.
 *
 *@param url The URL or mainDocumentURL of a request.
 */
+ (void)removeCachableURL:(NSURL *)url;

/**
 * Clears the URL cache
 */
+ (void)clearCache;
@end
