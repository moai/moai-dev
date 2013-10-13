
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
#import "UAHTTPConnection.h"

/**
 * Represents a UAHTTPConnection as an abstract unit of work, which can be
 * run in serial or parallel in an NSOperationQueue.  All connection success and failure
 * semantics are identical to UAHTTPConnection.
 */
@interface UAHTTPConnectionOperation : NSOperation

/**
 * UAHTTPConnectionOperation initializer.
 * @param request An instance of UAHTTPRequest to be executed in the connection.
 * @param successBlock A UAHTTPConnectionSuccessBlock which will be called if the resulting connection succeeds.
 * @param failureBlock A UAHTTPConnectionFailureBlock which will be called if the resulting connection fails.
 */
- (id)initWithRequest:(UAHTTPRequest *)request
            onSuccess:(UAHTTPConnectionSuccessBlock)successBlock
            onFailure:(UAHTTPConnectionFailureBlock)failureBlock;

/**
 * Autoreleased UAHTTPConnectionOperation class factory method.
 * @param request An instance of UAHTTPRequest to be executed in the connection.
 * @param successBlock A UAHTTPConnectionSuccessBlock which will be called if the resulting connection succeeds.
 * @param failureBlock A UAHTTPConnectionFailureBlock which will be called if the resulting connection fails.
 */
+ (id)operationWithRequest:(UAHTTPRequest *)request
                 onSuccess:(UAHTTPConnectionSuccessBlock)successBlock
                 onFailure:(UAHTTPConnectionFailureBlock)failureBlock;

@end
