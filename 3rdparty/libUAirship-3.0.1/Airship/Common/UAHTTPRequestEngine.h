
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

#define kUARequestEngineDefaultMaxConcurrentRequests 1
#define kUARequestEngineDefaultInitialDelayIntervalSeconds 30
#define kUARequestEngineDefaultMaxDelayIntervalSeconds 300
#define kUARequestEngineDefaultBackoffFactor 2

#import <Foundation/Foundation.h>
#import "UAHTTPConnection.h"

typedef BOOL (^UAHTTPRequestEngineWhereBlock)(UAHTTPRequest *request);
typedef void (^UAHTTPRequestEngineSuccessBlock)(UAHTTPRequest *request, NSUInteger lastDelay);
typedef void (^UAHTTPRequestEngineFailureBlock)(UAHTTPRequest *request, NSUInteger lastDelay);

/**
 * A generalized abstraction for running HTTP requests in an NSOperationQueue.
 * Retry logic is implicit, and functionally configurable using blocks. This class is intended
 * as a modular component for designing REST clients.
 */
@interface UAHTTPRequestEngine : NSObject

/**
 * Default initializer.
 */
- (id)init;

/**
 * Initializer with argument for custom NSOperationQueue.
 * @param queue A custom NSOperationQueue
 */
- (id)initWithQueue:(NSOperationQueue *)queue;

/**
 * Enqueues a request for immediate or pending execution.
 * 
 * Note that the success and failure semantics for this class are higher level than those of UAHTTPConnection.
 * UAHTTPRequestEngine will automatically retry under failed (or otherwise selected) conditions, such that a success
 * callback indicates "true" success, however that is defined, rather than merely a successful HTTP response.
 *
 * @param request An instance of UAHTTPRequest to be executed.
 * @param succeedWhereBlock A UAHTTPRequestEngineWhereBlock indicating the conditions in which a request should succeed.
 * @param retryWhereBlock A UAHTTPRequestEngineWhereBlock indicating the conditions in which a request should be retried.
 * @param successBlock A UAHTTPRequestEngineSuccessBlock that will be called on success.
 * @param failureBlock A UAHTTPRequestEngineFailureBlock that will be called on failure.
 *
 */
- (void)runRequest:(UAHTTPRequest *)request
      succeedWhere:(UAHTTPRequestEngineWhereBlock)succeedWhereBlock
        retryWhere:(UAHTTPRequestEngineWhereBlock)retryWhereBlock
         onSuccess:(UAHTTPRequestEngineSuccessBlock)successBlock
         onFailure:(UAHTTPRequestEngineFailureBlock)failureBlock;

/**
 * Cancels all currently enqueued or unfinished requests.
 */
- (void)cancelAllRequests;

/**
 * The maximum number of concurrent requests that can be run. Setting this value to 1
 * results in serial execution. 
 *
 * Defaults to 1.
 */
@property(nonatomic, assign) NSUInteger maxConcurrentRequests;

/**
 * The initial delay interval in seconds, to be used in retry backoff sequences. 
 *
 * Defaults to 30 seconds.
 */
@property(nonatomic, assign) NSUInteger initialDelayIntervalInSeconds;

/**
 * The maximum delay interval in seconds, to be used in retry backoff sequences. 
 *
 * Defaults to 300 seconds.
 */
@property(nonatomic, assign) NSUInteger maxDelayIntervalInSeconds;

/**
 * The multiplier used to obtain new delay intervals during retry backoff sequences.
 *
 * Defaults to 2.
 */
@property(nonatomic, assign) NSUInteger backoffFactor;


/**
 * The operation queue used for running registrations.
 */
@property(nonatomic, readonly, strong) NSOperationQueue *queue;


@end