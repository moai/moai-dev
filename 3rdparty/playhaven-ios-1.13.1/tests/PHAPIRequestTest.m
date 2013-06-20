/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHAPIRequestTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/30/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHAPIRequest.h"
#import "PHConstants.h"
#import "PHStringUtil.h"
#import "PHPublisherOpenRequest.h"

#define PUBLISHER_TOKEN @"PUBLISHER_TOKEN"
#define PUBLISHER_SECRET @"PUBLISHER_SECRET"

#define HASH_STRING  @"DEVICE_ID:PUBLISHER_TOKEN:PUBLISHER_SECRET:NONCE"
#define EXPECTED_HASH @"3L0xlrDOt02UrTDwMSnye05Awwk"

@interface PHAPIRequest (Private)
+ (NSMutableSet *)allRequests;
+ (void)setSession:(NSString *)session;
- (void)processRequestResponse:(NSDictionary *)response;
@end

@interface PHAPIRequestTest : SenTestCase @end
@interface PHAPIRequestResponseTest : SenTestCase <PHAPIRequestDelegate> {
    PHAPIRequest *_request;
    BOOL _didProcess;
}
@end
@interface PHAPIRequestErrorTest : SenTestCase <PHAPIRequestDelegate> {
    PHAPIRequest *_request;
    BOOL _didProcess;
}
@end
@interface PHAPIRequestByHashCodeTest : SenTestCase @end

@implementation PHAPIRequestTest

- (void)testSignatureHash
{
    NSString *signatureHash = [PHAPIRequest base64SignatureWithString:HASH_STRING];
    STAssertTrue([EXPECTED_HASH isEqualToString:signatureHash],
                 @"Hash mismatch. Expected %@ got %@",EXPECTED_HASH,signatureHash);
}

- (void)testResponseDigestVerification
{
    /*
     For this test expected digest hashes were generated using pyton's hmac library.
     */
    NSString *responseDigest, *expectedDigest;

    // Digest with nonce
    responseDigest = [PHAPIRequest expectedSignatureValueForResponse:@"response body" nonce:@"nonce" secret:PUBLISHER_SECRET];
    expectedDigest = @"rt3JHGReRAaol-xPVildr6Ev9fU=";
    STAssertTrue([responseDigest isEqualToString:expectedDigest], @"Digest mismatch. Expected %@ got %@", expectedDigest, responseDigest);

    // Digest without nonce
    responseDigest = [PHAPIRequest expectedSignatureValueForResponse:@"response body" nonce:nil secret:PUBLISHER_SECRET];
    expectedDigest = @"iNmo12xRqVAn_7quEvOSwhenEZA=";
    STAssertTrue([responseDigest isEqualToString:expectedDigest], @"Digest mismatch. Expected %@ got %@", expectedDigest, responseDigest);
}

- (void)testRequestParameters
{
    PHAPIRequest *request = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    NSDictionary *signedParameters = [request signedParameters];
    NSString     *requestURLString = [request.URL absoluteString];

    // Test for existence of parameters
    NSString
        *session   = [signedParameters valueForKey:@"session"],
        *gid       = [signedParameters valueForKey:@"gid"],
        *token     = [signedParameters valueForKey:@"token"],
        *signature = [signedParameters valueForKey:@"signature"],
        *nonce     = [signedParameters valueForKey:@"nonce"];

#if PH_USE_UNIQUE_IDENTIFIER == 1
    NSString *device = [signedParameters valueForKey:@"device"];
    STAssertNotNil(device, @"UDID param is missing!");
    STAssertFalse([requestURLString rangeOfString:@"device="].location == NSNotFound, @"UDID param is missing: %@", requestURLString);
#else
    NSString *device = [signedParameters valueForKey:@"device"];
    STAssertNil(device, @"UDID param is present!");
    STAssertTrue([requestURLString rangeOfString:@"device="].location == NSNotFound, @"UDID param exists when it shouldn't: %@", requestURLString);
#endif

#if PH_USE_MAC_ADDRESS == 1
    NSString *mac   = [signedParameters valueForKey:@"d_mac"];
    NSString *odin1 = [signedParameters valueForKey:@"d_odin1"];
    STAssertNotNil(mac, @"MAC param is missing!");
    STAssertNotNil(odin1, @"ODIN1 param is missing!");
    STAssertFalse([requestURLString rangeOfString:@"d_mac="].location == NSNotFound, @"MAC param is missing: %@", requestURLString);
    STAssertFalse([requestURLString rangeOfString:@"d_odin1="].location == NSNotFound, @"ODIN1 param is missing: %@", requestURLString);
#else
    NSString *mac   = [signedParameters valueForKey:@"d_mac"];
    NSString *odin1 = [signedParameters valueForKey:@"d_odin1"];
    STAssertNil(mac, @"MAC param is present!");
    STAssertNil(odin1, @"ODIN1 param is present!");
    STAssertTrue([requestURLString rangeOfString:@"d_mac="].location == NSNotFound, @"MAC param exists when it shouldn't: %@", requestURLString);
    STAssertTrue([requestURLString rangeOfString:@"d_odin1="].location == NSNotFound, @"ODIN1 param exists when it shouldn't: %@", requestURLString);
#endif

    STAssertNotNil(session, @"Required session param is missing!");
    STAssertNotNil(gid, @"Required gid param is missing!");
    STAssertNotNil(token, @"Required token param is missing!");
    STAssertNotNil(signature, @"Required signature param is missing!");
    STAssertNotNil(nonce, @"Required nonce param is missing!");

    NSString *parameterString = [request signedParameterString];
    STAssertNotNil(parameterString, @"Parameter string is nil?");

    NSString *tokenParam = [NSString stringWithFormat:@"token=%@",token];
    STAssertFalse([parameterString rangeOfString:tokenParam].location == NSNotFound,
                  @"Token parameter not present!");

    NSString *signatureParam = [NSString stringWithFormat:@"signature=%@",signature];
    STAssertFalse([parameterString rangeOfString:signatureParam].location == NSNotFound,
                  @"Signature parameter not present!");

    NSString
        *expectedSignatureString = [NSString stringWithFormat:@"%@:%@:%@:%@:%@", PUBLISHER_TOKEN, [PHAPIRequest session], PHGID(), nonce, PUBLISHER_SECRET],
        *expectedSignature       = [PHStringUtil b64DigestForString:expectedSignatureString];
    STAssertTrue([signature isEqualToString:expectedSignature], @"signature did not match expected value!");

    NSString *nonceParam = [NSString stringWithFormat:@"nonce=%@",nonce];
    STAssertFalse([parameterString rangeOfString:nonceParam].location == NSNotFound,
                  @"Nonce parameter not present!");
}

- (void)testCustomRequestParameters
{
    NSDictionary *signedParameters;
    PHAPIRequest *request = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];

    // Test what happens when they are not set
    NSString
        *customUDID       = [PHAPIRequest customUDID],
        *pluginIdentifier = [PHAPIRequest pluginIdentifier],
        *requestURLString = [request.URL absoluteString];

    STAssertTrue([requestURLString rangeOfString:@"d_custom="].location == NSNotFound,
                  @"Custom parameter exists when none should be set.");
    STAssertNil(customUDID, @"Custom UDID param is not nil!");
    STAssertNotNil(pluginIdentifier, @"Plugin identifier param is missing!");
    STAssertTrue([pluginIdentifier isEqualToString:@"ios"], @"Plugin identifier param is incorrect!");

    // Test what happens when they are set to nil
    [PHAPIRequest setCustomUDID:nil];
    [PHAPIRequest setPluginIdentifier:nil];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([requestURLString rangeOfString:@"d_custom="].location == NSNotFound,
                  @"Custom parameter exists when none should be set.");
    STAssertNil(customUDID, @"Custom UDID param is not nil!");
    STAssertNotNil(pluginIdentifier, @"Plugin identifier param is missing!");
    STAssertTrue([pluginIdentifier isEqualToString:@"ios"], @"Plugin identifier param is incorrect!");

    // Test what happens when they are set to empty strings
    [PHAPIRequest setCustomUDID:@""];
    [PHAPIRequest setPluginIdentifier:@""];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([requestURLString rangeOfString:@"d_custom="].location == NSNotFound,
                  @"Custom parameter exists when none should be set.");
    STAssertNil(customUDID, @"Custom UDID param is not nil!");
    STAssertNotNil(pluginIdentifier, @"Plugin identifier param is missing!");
    STAssertTrue([pluginIdentifier isEqualToString:@"ios"], @"Plugin identifier param is incorrect!");

    // Test what happens when they are set to [NSNull null]
    [PHAPIRequest setCustomUDID:(id)[NSNull null]];
    [PHAPIRequest setPluginIdentifier:(id)[NSNull null]];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([requestURLString rangeOfString:@"d_custom="].location == NSNotFound,
                  @"Custom parameter exists when none should be set.");
    STAssertNil(customUDID, @"Custom UDID param is not nil!");
    STAssertNotNil(pluginIdentifier, @"Plugin identifier param is missing!");
    STAssertTrue([pluginIdentifier isEqualToString:@"ios"], @"Plugin identifier param is incorrect!");

    // Test what happens when they are longer than the allowed amount for plugin identifier (42)
    [PHAPIRequest setCustomUDID:@"12345678911234567892123456789312345678941234567895"];
    [PHAPIRequest setPluginIdentifier:@"12345678911234567892123456789312345678941234567895"];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([customUDID isEqualToString:@"12345678911234567892123456789312345678941234567895"],
                 @"Custom UDID param is not 42 characters!"); // Stays the same...
    STAssertTrue([pluginIdentifier isEqualToString:@"123456789112345678921234567893123456789412"],
                 @"Plugin identifier param is not 42 characters!"); // Trimmed...
    STAssertTrue([pluginIdentifier length], @"Plugin identifier param is not 42 characters!");


    // Test what happens when they have mixed reserved characters
    [PHAPIRequest setCustomUDID:@"abcdefg:?#[]@/!$&'()*+,;=\"abcdefg"];
    [PHAPIRequest setPluginIdentifier:@"abcdefg:?#[]@/!$&'()*+,;=\"abcdefg"];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([customUDID isEqualToString:@"abcdefgabcdefg"],
                 @"Custom UDID param is not stripped of reserved characters properly!"); // Stripped...
    STAssertTrue([pluginIdentifier isEqualToString:@"abcdefgabcdefg"],
                 @"Plugin identifier param is not stripped of reserved characters properly!"); // Stripped...

    // Test what happens when they have mixed reserved characters and at length 42 after
    [PHAPIRequest setCustomUDID:@"1234567891123456789212345678931234567894:?#[]@/!$&'()*+,;=\"12"];
    [PHAPIRequest setPluginIdentifier:@"1234567891123456789212345678931234567894:?#[]@/!$&'()*+,;=\"12"];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([customUDID isEqualToString:@"123456789112345678921234567893123456789412"],
                 @"Custom UDID param is not stripped of reserved characters properly!");
    STAssertTrue([pluginIdentifier isEqualToString:@"123456789112345678921234567893123456789412"],
                 @"Plugin identifier param is not stripped of reserved characters properly!");

    // Test what happens when they have mixed reserved characters and over length 42 after
    [PHAPIRequest setCustomUDID:@"1234567891123456789212345678931234567894:?#[]@/!$&'()*+,;=\"1234567895"];
    [PHAPIRequest setPluginIdentifier:@"1234567891123456789212345678931234567894:?#[]@/!$&'()*+,;=\"1234567895"];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([customUDID isEqualToString:@"12345678911234567892123456789312345678941234567895"],
                 @"Custom UDID param is not stripped of reserved characters properly!"); // Stripped
    STAssertTrue([pluginIdentifier isEqualToString:@"123456789112345678921234567893123456789412"],
                 @"Plugin identifier param is not stripped of reserved characters properly!"); // Stripped and trimmed

    // Test what happens when it's only reserved characters
    [PHAPIRequest setCustomUDID:@":?#[]@/!$&'()*+,;=\""];
    [PHAPIRequest setPluginIdentifier:@":?#[]@/!$&'()*+,;=\""];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];

    customUDID       = [PHAPIRequest customUDID];
    pluginIdentifier = [PHAPIRequest pluginIdentifier];

    STAssertTrue([requestURLString rangeOfString:@"d_custom="].location == NSNotFound,
                  @"Custom parameter exists when none should be set.");
    STAssertNil(customUDID, @"Custom UDID param is not nil!");
    STAssertNotNil(pluginIdentifier, @"Plugin identifier param is missing!");
    STAssertTrue([pluginIdentifier isEqualToString:@"ios"], @"Plugin identifier param is incorrect!");

    // Test PHPublisherOpenRequest.customUDID property and PHAPIRequest property and class methods
    PHPublisherOpenRequest *openRequest = [PHPublisherOpenRequest requestForApp:PUBLISHER_TOKEN
                                                                         secret:PUBLISHER_SECRET];

    [openRequest setCustomUDID:@"one"];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];
    signedParameters = [request signedParameters];
    customUDID       = [signedParameters valueForKey:@"d_custom"];

    STAssertFalse([requestURLString rangeOfString:@"d_custom="].location == NSNotFound, @"Custom parameter missing when one is set.");
    STAssertTrue([customUDID isEqualToString:@"one"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be one", customUDID);

    customUDID       = [PHAPIRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"one"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be one", customUDID);

    customUDID       = [request customUDID];
    STAssertTrue([customUDID isEqualToString:@"one"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be one", customUDID);

    customUDID       = [openRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"one"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be one", customUDID);

    [PHAPIRequest setCustomUDID:@"two"];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];
    signedParameters = [request signedParameters];
    customUDID       = [signedParameters valueForKey:@"d_custom"];

    STAssertFalse([requestURLString rangeOfString:@"d_custom="].location == NSNotFound, @"Custom parameter missing when one is set.");
    STAssertTrue([customUDID isEqualToString:@"two"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be two", customUDID);

    customUDID       = [PHAPIRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"two"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be two", customUDID);

    customUDID       = [request customUDID];
    STAssertTrue([customUDID isEqualToString:@"two"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be two", customUDID);

    customUDID       = [openRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"two"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be two", customUDID);

    [request setCustomUDID:@"three"];

    request          = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    requestURLString = [request.URL absoluteString];
    signedParameters = [request signedParameters];
    customUDID       = [signedParameters valueForKey:@"d_custom"];

    STAssertFalse([requestURLString rangeOfString:@"d_custom="].location == NSNotFound, @"Custom parameter missing when one is set.");
    STAssertTrue([customUDID isEqualToString:@"three"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be three", customUDID);

    customUDID       = [PHAPIRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"three"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be three", customUDID);

    customUDID       = [request customUDID];
    STAssertTrue([customUDID isEqualToString:@"three"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be three", customUDID);

    customUDID       = [openRequest customUDID];
    STAssertTrue([customUDID isEqualToString:@"three"],
                  @"Custom UDID isn't synced between base PHAPIRequest and PHPublisherOpenRequest: is %@ and should be three", customUDID);
}

- (void)testURLProperty
{
    PHAPIRequest *request = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    NSString     *desiredURLString = @"http://thisisatesturlstring.com";

    request.urlPath = desiredURLString;
    STAssertFalse([[request.URL absoluteString] rangeOfString:desiredURLString].location == NSNotFound,
                  @"urlPath not present in signed URL!");

}

- (void)testSession
{
    STAssertNoThrow([PHAPIRequest setSession:@"test_session"], @"setting a session shouldn't throw an error");
    STAssertNoThrow([PHAPIRequest setSession:nil], @"clearing a session shouldn't throw");
}
@end

@implementation PHAPIRequestResponseTest

- (void)setUp
{
    _request = [[PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET] retain];
    _request.delegate = self;
    _didProcess = NO;
}

- (void)testResponse
{
    NSDictionary *testDictionary     = [NSDictionary dictionaryWithObjectsAndKeys:
                                                            @"awesomesause", @"awesome", nil];
    NSDictionary *responseDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                                            testDictionary,@"response",
                                                            [NSNull null],@"error",
                                                            [NSNull null],@"errobj", nil];
    [_request processRequestResponse:responseDictionary];
}

- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData
{
    STAssertNotNil(responseData, @"Expected responseData, got nil!");
    STAssertTrue([[responseData allKeys] count] == 1, @"Unexpected number of keys in response data!");
    STAssertTrue([@"awesomesause" isEqualToString:[responseData valueForKey:@"awesome"]],
                 @"Expected 'awesomesause' got %@",
                 [responseData valueForKey:@"awesome"]);
    _didProcess = YES;
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    STFail(@"Request failed with error, but it wasn't supposed to!");
}

- (void)tearDown
{
    STAssertTrue(_didProcess, @"Did not actually process request!");
}

- (void)dealloc
{
    [_request release], _request = nil;
    [super dealloc];
}
@end

@implementation PHAPIRequestErrorTest

- (void)setUp
{
    _request = [[PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET] retain];
    _request.delegate = self;
    _didProcess = NO;
}

- (void)testResponse
{
    NSDictionary *responseDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                            @"this is awesome!",@"error", nil];
    [_request processRequestResponse:responseDictionary];
}

- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData
{
    STFail(@"Request failed succeeded, but it wasn't supposed to!");
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    STAssertNotNil(error, @"Expected error but got nil!");
    _didProcess = YES;
}

- (void)tearDown
{
    STAssertTrue(_didProcess, @"Did not actually process request!");
}
@end

@implementation PHAPIRequestByHashCodeTest

- (void)testRequestByHashCode
{
    int hashCode = 100;

    PHAPIRequest *request = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    request.hashCode = hashCode;

    PHAPIRequest *retrievedRequest = [PHAPIRequest requestWithHashCode:hashCode];
    STAssertTrue(request == retrievedRequest, @"Request was not able to be retrieved by hashCode.");
    STAssertNil([PHAPIRequest requestWithHashCode:hashCode+1], @"Non-existent hashCode returned a request.");

    [request cancel];
    STAssertNil([PHAPIRequest requestWithHashCode:hashCode], @"Canceled request was retrieved by hashCode");
}

- (void)testRequestCancelByHashCode
{
    int hashCode = 200;

    PHAPIRequest *request = [PHAPIRequest requestForApp:PUBLISHER_TOKEN secret:PUBLISHER_SECRET];
    request.hashCode = hashCode;

    STAssertTrue([PHAPIRequest cancelRequestWithHashCode:hashCode] == 1, @"Request was not canceled!");
    STAssertTrue([PHAPIRequest cancelRequestWithHashCode:hashCode] == 0, @"Canceled request was canceled again.");
    STAssertTrue([PHAPIRequest cancelRequestWithHashCode:hashCode+1] == 0, @"Nonexistent request was canceled.");
    STAssertFalse([[PHAPIRequest allRequests] containsObject:request], @"Request was not removed from request array!");
}
@end
