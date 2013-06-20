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

 PHPublisherIAPTrackingRequestTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 2/24/2012.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PlayHavenSDK.h"

@interface PHPublisherIAPTrackingRequestTest : SenTestCase
@end

@implementation PHPublisherIAPTrackingRequestTest

- (void)testConstructors
{
    PHPublisherIAPTrackingRequest *request;

    request = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET"];
    STAssertNotNil(request, @"Expected request to exist!");


    NSString *product  = @"com.playhaven.item";
    NSInteger quantity = 1;
    request = [PHPublisherIAPTrackingRequest requestForApp:@"APP"
                                                    secret:@"SECRET"
                                                   product:product
                                                  quantity:quantity
                                                resolution:PHPurchaseResolutionBuy
                                               receiptData:nil];
    STAssertNotNil(request, @"Expected request to exist!");

    request = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET" product:product quantity:quantity error:PHCreateError(PHIAPTrackingSimulatorErrorType) receiptData:nil];
    STAssertNotNil(request, @"Expected request to exist!");
}

- (void)testCookie
{
    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET" product:@"PRODUCT" quantity:1 resolution:PHPurchaseResolutionBuy receiptData:nil];
    [request send];
    STAssertTrue([[request signedParameterString] rangeOfString:@"cookie"].location == NSNotFound, @"expected no cookie string parameterString: %@", [request signedParameterString]);
    [request cancel];

    [PHPublisherIAPTrackingRequest setConversionCookie:@"COOKIE" forProduct:@"PRODUCT"];

    PHPublisherIAPTrackingRequest *request2a = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET" product:@"PRODUCT_OTHER" quantity:1 resolution:PHPurchaseResolutionBuy receiptData:nil];
    [request2a send];
    STAssertTrue([[request2a signedParameterString] rangeOfString:@"cookie"].location == NSNotFound, @"expected no cookie string parameterString: %@", [request2a signedParameterString]);
    [request2a cancel];

    PHPublisherIAPTrackingRequest *request2 = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET" product:@"PRODUCT" quantity:1 resolution:PHPurchaseResolutionBuy receiptData:nil];
    [request2 send];
    STAssertTrue([[request2 signedParameterString] rangeOfString:@"cookie"].location != NSNotFound, @"expected cookie string parameterString: %@", [request2 signedParameterString]);
    [request2 cancel];

    PHPublisherIAPTrackingRequest *request3 = [PHPublisherIAPTrackingRequest requestForApp:@"APP" secret:@"SECRET" product:@"PRODUCT" quantity:1 resolution:PHPurchaseResolutionBuy receiptData:nil];
    [request3 send];
    STAssertTrue([[request3 signedParameterString] rangeOfString:@"cookie"].location == NSNotFound, @"cookie should only exist once! parameterString: %@", [request3 signedParameterString]);
    [request3 cancel];
}

- (void)testRequestParameters
{
    NSString *token  = @"PUBLISHER_TOKEN",
             *secret = @"PUBLISHER_SECRET";

    [PHAPIRequest setCustomUDID:nil];

    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:token secret:secret];

    NSDictionary *signedParameters  = [request signedParameters];
    NSString     *requestURLString  = [request.URL absoluteString];

#if PH_USE_UNIQUE_IDENTIFIER == 1
    NSString *device = [signedParameters valueForKey:@"device"];
    STAssertNotNil(device, @"UDID param is missing!");
    STAssertFalse([requestURLString rangeOfString:@"device="].location == NSNotFound, @"UDID param is missing!");
#else
    NSString *device = [signedParameters valueForKey:@"device"];
    STAssertNil(device, @"UDID param is present!");
    STAssertTrue([requestURLString rangeOfString:@"device="].location == NSNotFound, @"UDID param exists when it shouldn't.");
#endif

#if PH_USE_MAC_ADDRESS == 1
    NSString *mac   = [signedParameters valueForKey:@"d_mac"];
    NSString *odin1 = [signedParameters valueForKey:@"d_odin1"];
    STAssertNotNil(mac, @"MAC param is missing!");
    STAssertNotNil(odin1, @"ODIN1 param is missing!");
    STAssertFalse([requestURLString rangeOfString:@"d_mac="].location == NSNotFound, @"MAC param is missing!");
    STAssertFalse([requestURLString rangeOfString:@"d_odin1="].location == NSNotFound, @"ODIN1 param is missing!");
#else
    NSString *mac   = [signedParameters valueForKey:@"d_mac"];
    NSString *odin1 = [signedParameters valueForKey:@"d_odin1"];
    STAssertNil(mac, @"MAC param is present!");
    STAssertNil(odin1, @"ODIN1 param is present!");
    STAssertTrue([requestURLString rangeOfString:@"d_mac="].location == NSNotFound, @"MAC param exists when it shouldn't.");
    STAssertTrue([requestURLString rangeOfString:@"d_odin1="].location == NSNotFound, @"ODIN1 param exists when it shouldn't.");
#endif
}
@end
