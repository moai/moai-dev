/*
 * Copyright 2012 Facebook
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "FBSessionTests.h"
#import "FBTestSession.h"
#import "FBRequest.h"
#import "FBGraphUser.h"
#import "FBTestBlocker.h"
#import "FBTests.h"
#import "FBUtility.h"

#if defined(FACEBOOKSDK_SKIP_SESSION_TESTS)

#pragma message ("warning: Skipping FBSessionTests")

#else

@implementation FBSessionTests

// All code under test must be linked into the Unit Test bundle
- (void)testSessionBasic
{
    FBConditionalLog(NO, @"Testing conditional %@", @"log");
    FBConditionalLog(NO, @"Testing conditional log");
    FBConditionalLog(YES, nil, @"Testing conditional log");
    
    // create valid
    FBTestBlocker *blocker = [[[FBTestBlocker alloc] init] autorelease];
    
    FBTestSession *session = [FBTestSession sessionWithSharedUserWithPermissions:nil];
    [session openWithCompletionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
        [blocker signal];
    }];
    
    [blocker wait];
    
    STAssertTrue(session.isOpen, @"Session should be valid, and is not");
    
    FBRequest *request = [[[FBRequest alloc] initWithSession:session
                                                   graphPath:@"me"]
                          autorelease];
    [request startWithCompletionHandler:
     ^(FBRequestConnection *connection, id<FBGraphUser> me, NSError *error) {
         STAssertTrue(me.id.length > 0, @"user id should be non-empty");
         [blocker signal];
     }];
    
    [blocker wait];
    
    [session close];
}

// All code under test must be linked into the Unit Test bundle
- (void)testSessionInvalidate
{
    // create valid
    FBTestBlocker *blocker = [[[FBTestBlocker alloc] init] autorelease];
    
    __block BOOL wasNotifiedOfInvalid = NO;
    FBTestSession *session = [FBTestSession sessionWithPrivateUserWithPermissions:nil];
    [session openWithCompletionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
        if (status == FBSessionStateClosed) {
            wasNotifiedOfInvalid = YES;
        }
        [blocker signal];
    }];
    [blocker wait];
    
    STAssertTrue(session.isOpen, @"Session should be open, and is not");

    __block NSString *userID = nil;
    FBRequest *request1 = [[[FBRequest alloc] initWithSession:session
                                                   graphPath:@"me"]
                          autorelease];
    [request1 startWithCompletionHandler:
     ^(FBRequestConnection *connection, id<FBGraphUser> me, NSError *error) {
         userID = [me.id retain];
         STAssertTrue(userID.length > 0, @"user id should be non-empty");
         [blocker signal];
     }];
    
    [blocker wait];

    // use FBRequest to create an NSURLRequest
    FBRequest *temp = [[FBRequest alloc] initWithSession:session
                                               graphPath:userID
                                              parameters:[NSDictionary dictionaryWithObjectsAndKeys:
                                                          @"delete", @"method",
                                                          nil]
                                              HTTPMethod:nil];
    FBRequestConnection *connection = [[FBRequestConnection alloc] init];
    [connection addRequest:temp completionHandler:nil];
    NSURLRequest *urlRequest = connection.urlRequest;
    [userID release];
    [connection release];
    [temp release];
    
    // synchronously delete the user
    NSURLResponse *response;
    NSError *error = nil;
    NSData *data;
    data = [NSURLConnection sendSynchronousRequest:urlRequest 
                                 returningResponse:&response
                                             error:&error];
    // if !data or if data == false, log
    NSString *body = !data ? nil : [[[NSString alloc] initWithData:data
                                                          encoding:NSUTF8StringEncoding]
                                    autorelease];    
    STAssertTrue([body isEqualToString:@"true"], @"body should return 'true'");
    
    FBRequest *request2 = [[[FBRequest alloc] initWithSession:session
                                                   graphPath:@"me"]
                          autorelease];
    [request2 startWithCompletionHandler:
     ^(FBRequestConnection *connection, id<FBGraphUser> me, NSError *error) {
        STAssertTrue(error != nil, @"response should be an error due to deleted user");
        [blocker signal];
    }];
    
    STAssertFalse(wasNotifiedOfInvalid, @"should not have invalidated the token yet");
    [blocker wait];
    STAssertTrue(wasNotifiedOfInvalid, @"should have invalidated the token by now");
    
    [session close];
}

@end

#endif

