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

 PHPublisherContentRequestTest.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 3/30/11.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>

#import "SBJsonParser.h"
#import "PHContent.h"
#import "PHContentView.h"
#import "PHPublisherContentRequest.h"
#import "PHStringUtil.h"

#define PUBLISHER_TOKEN @"PUBLISHER_TOKEN"
#define PUBLISHER_SECRET @"PUBLISHER_SECRET"

@interface PHPublisherContentRequest (TestMethods)
@property (nonatomic, readonly) PHPublisherContentRequestState state;
- (BOOL)setPublisherContentRequestState:(PHPublisherContentRequestState)state;
- (BOOL)isValidReward:(NSDictionary *)rewardData;
- (void)requestRewards:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;

- (BOOL)isValidPurchase:(NSDictionary *)purchaseData;
- (void)requestPurchases:(NSDictionary *)queryParameters callback:(NSString *)callback source:(PHContentView *)source;
@end

@interface PHContentTest : SenTestCase @end
@interface PHContentViewTest : SenTestCase @end
@interface PHContentViewRedirectTest : SenTestCase {
    PHContent *_content;
    PHContentView *_contentView;
    BOOL _didDismiss, _didLaunch;
}
@end

@interface PHContentViewRedirectRecyclingTest : SenTestCase {
    BOOL _shouldExpectParameter;
}
@end

@interface PHPublisherContentRequestTest : SenTestCase @end
@interface PHPublisherContentRewardsTest : SenTestCase @end
@interface PHPublisherContentPurchasesTest : SenTestCase @end
@interface PHPublisherContentRequestPreservationTest : SenTestCase @end
@interface PHPublisherContentPreloadTest : SenTestCase {
    PHPublisherContentRequest *_request;
    BOOL _didPreload;
}
@end

@interface PHPublisherContentPreloadParameterTest : SenTestCase @end
@interface PHPublisherContentStateTest : SenTestCase @end

@implementation PHContentTest

- (void)testContent
{
    NSString
        *empty   = @"{}",
        *keyword = @"{\"frame\":\"PH_FULLSCREEN\",\"url\":\"http://google.com\",\"transition\":\"PH_MODAL\",\"context\":{\"awesome\":\"awesome\"}}",
        *rect    = @"{\"frame\":{\"PH_LANDSCAPE\":{\"x\":60,\"y\":40,\"w\":200,\"h\":400},\"PH_PORTRAIT\":{\"x\":40,\"y\":60,\"w\":240,\"h\":340}},\"url\":\"http://google.com\",\"transition\":\"PH_DIALOG\",\"context\":{\"awesome\":\"awesome\"}}";

  PH_SBJSONPARSER_CLASS *parser = [[PH_SBJSONPARSER_CLASS alloc] init];
    NSDictionary
        *emptyDict   = [parser objectWithString:empty],
        *keywordDict = [parser objectWithString:keyword],
        *rectDict    = [parser objectWithString:rect];

    [parser release];

    PHContent *emptyUnit = [PHContent contentWithDictionary:emptyDict];
    STAssertNil(emptyUnit, @"Empty definition should result in nil!");

    PHContent *keywordUnit = [PHContent contentWithDictionary:keywordDict];
    STAssertNotNil(keywordUnit, @"Keyword definition should result in unit!");

    CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];
    STAssertTrue(CGRectEqualToRect([keywordUnit frameForOrientation:UIInterfaceOrientationPortrait], applicationFrame),
                 @"Frame mismatch from keyword. Got %@", NSStringFromCGRect(applicationFrame));

    NSURL *adURL = [NSURL URLWithString:@"http://google.com"];
    STAssertTrue([keywordUnit.URL isEqual:adURL],
                 @"URL mismatch. Expected %@ got %@", adURL, keywordUnit.URL);

    STAssertTrue(keywordUnit.transition == PHContentTransitionModal,
                 @"Transition type mismatched. Expected %d got %d", PHContentTransitionModal, keywordUnit.transition);

    STAssertNotNil([keywordUnit.context valueForKey:@"awesome"],
                   @"Expected payload key not found!");

    PHContent *rectUnit = [PHContent contentWithDictionary:rectDict];
    STAssertNotNil(rectUnit, @"Keyword definition should result in unit!");

    CGRect expectedLandscapeFrame = CGRectMake(60,40,200,400);
    STAssertTrue(CGRectEqualToRect([rectUnit frameForOrientation:UIInterfaceOrientationLandscapeLeft], expectedLandscapeFrame),
                 @"Frame mismatch from keyword. Got %@", NSStringFromCGRect([rectUnit frameForOrientation:UIInterfaceOrientationLandscapeLeft]));

}

- (void)testCloseButtonDelayParameter
{
  PHContent *content = [[PHContent alloc] init];
  STAssertTrue(content.closeButtonDelay == 10.0f, @"Default closeButton delay value incorrect!");
  [content release];

  NSString *rect = @"{\"frame\":{\"x\":60,\"y\":40,\"w\":200,\"h\":400},\"url\":\"http://google.com\",\"transition\":\"PH_DIALOG\",\"context\":{\"awesome\":\"awesome\"},\"close_delay\":23}";

  PH_SBJSONPARSER_CLASS *parser = [[PH_SBJSONPARSER_CLASS alloc] init];
  NSDictionary *rectDict = [parser objectWithString:rect];
  [parser release];

  PHContent *rectUnit = [PHContent contentWithDictionary:rectDict];
  STAssertTrue(rectUnit.closeButtonDelay == 23.0f, @"Expected 23 got %f", content.closeButtonDelay);
}

- (void)testCloseButtonUrlParameter
{
  PHContent *content = [[PHContent alloc] init];
  STAssertTrue(content.closeButtonURLPath == nil, @"CloseButtonURLPath property not available");
  [content release];

  NSString *rect = @"{\"frame\":{\"x\":60,\"y\":40,\"w\":200,\"h\":400},\"url\":\"http://google.com\",\"transition\":\"PH_DIALOG\",\"context\":{\"awesome\":\"awesome\"},\"close_ping\":\"http://playhaven.com\"}";

  PH_SBJSONPARSER_CLASS *parser = [[PH_SBJSONPARSER_CLASS alloc] init];
  NSDictionary *rectDict = [parser objectWithString:rect];
  [parser release];

  PHContent *rectUnit = [PHContent contentWithDictionary:rectDict];
  STAssertTrue([rectUnit.closeButtonURLPath isEqualToString:@"http://playhaven.com"], @"Expected 'http://playhaven.com got %@", content.closeButtonURLPath);
}
@end

@implementation PHContentViewTest

- (void)testcontentView
{
    PHContent *content = [[PHContent alloc] init];

    PHContentView *contentView = [[PHContentView alloc] initWithContent:content];
    STAssertTrue([contentView respondsToSelector:@selector(show:)], @"Should respond to show selector");
    STAssertTrue([contentView respondsToSelector:@selector(dismiss:)], @"Should respond to dismiss selector");
    [contentView release];
    [content release];
}
@end

@implementation PHContentViewRedirectTest

- (void)setUp
{
    _content = [[PHContent alloc] init];

    _contentView = [[PHContentView alloc] initWithContent:_content];
    [_contentView redirectRequest:@"ph://dismiss" toTarget:self action:@selector(dismissRequestCallback:)];
    [_contentView redirectRequest:@"ph://launch" toTarget:self action:@selector(launchRequestCallback:)];
}

- (void)testRegularRequest
{
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://google.com"]];
    BOOL result = [_contentView webView:nil shouldStartLoadWithRequest:request navigationType:UIWebViewNavigationTypeLinkClicked];
    STAssertTrue(result, @"_contentView should open http://google.com in webview!");
}

- (void)testDismissRequest
{
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"ph://dismiss"]];
    BOOL result = [_contentView webView:nil shouldStartLoadWithRequest:request navigationType:UIWebViewNavigationTypeLinkClicked];
    STAssertFalse(result, @"_contentView should not open ph://dismiss in webview!");
}

- (void)dismissRequestCallback:(NSDictionary *)parameters
{
    STAssertNil(parameters, @"request with no parameters returned parameters!");
}

- (void)testLaunchRequest
{
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"ph://launch?context=%7B%22url%22%3A%22http%3A%2F%2Fadidas.com%22%7D"]];
    BOOL result = [_contentView webView:nil shouldStartLoadWithRequest:request navigationType:UIWebViewNavigationTypeLinkClicked];
    STAssertFalse(result, @"_contentView should not open ph://dismiss in webview!");
}

- (void)launchRequestCallback:(NSDictionary *)parameters
{
    STAssertNotNil(parameters, @"request with parameters returned no parameters!");
    STAssertTrue([@"http://adidas.com" isEqualToString:[parameters valueForKey:@"url"]],
                 @"Expected 'http://adidas.com' got %@ as %@",
                 [parameters valueForKey:@"url"], [[parameters valueForKey:@"url"] class]);

}

- (void)dealloc
{
    [_content release], _content = nil;
    [_contentView release], _contentView = nil;
    [super dealloc];
}
@end

@implementation PHContentViewRedirectRecyclingTest
- (void)testRedirectRecycling
{
    PHContent     *content     = [[PHContent alloc] init];
    PHContentView *contentView = [[PHContentView alloc] initWithContent:content];
    [content release];

    [contentView redirectRequest:@"ph://test" toTarget:self action:@selector(handleTest:)];

    NSURLRequest *request  =
            [NSURLRequest requestWithURL:[NSURL URLWithString:@"ph://test?context=%7B%22url%22%3A%22http%3A%2F%2Fadidas.com%22%7D"]];
    _shouldExpectParameter = YES;
    STAssertFalse([contentView webView:nil
            shouldStartLoadWithRequest:request
                        navigationType:UIWebViewNavigationTypeLinkClicked], @"Didn't redirect to dispatch handler");

    // NOTE: This rest ensures that invocation objects are being properly recycled.
    NSURLRequest *nextRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:@"ph://test"]];
    _shouldExpectParameter = NO;
    STAssertFalse([contentView webView:nil
            shouldStartLoadWithRequest:nextRequest
                        navigationType:UIWebViewNavigationTypeLinkClicked], @"Didn't redirect next request to dispatch handler");
}

- (void)handleTest:(NSDictionary *)parameters
{
    NSString *url = [parameters valueForKey:@"url"];
    if (_shouldExpectParameter) {
        STAssertNotNil(url, @"Expected parameter was not present");
    } else  {
        STAssertNil(url, @"Expected nil returned a value");
    }
}
@end

@implementation PHPublisherContentRequestTest

- (void)testAnimatedParameter
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:PUBLISHER_TOKEN
                                                                           secret:PUBLISHER_SECRET];
    STAssertTrue(request.animated, @"Default state of animated property should be TRUE");

    request.animated = NO;
    STAssertFalse(request.animated, @"Animated property not set!");
}

- (void)testRequestParameters
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:PUBLISHER_TOKEN
                                                                           secret:PUBLISHER_SECRET];
    request.placement = @"placement_id";

    NSDictionary *dictionary = [request signedParameters];
    STAssertNotNil([dictionary valueForKey:@"placement_id"], @"Expected 'placement_id' parameter.");

    NSString *parameterString = [request signedParameterString];
    NSString *placementParam  = @"placement_id=placement_id";
    STAssertFalse([parameterString rangeOfString:placementParam].location == NSNotFound,
                  @"Placment_id parameter not present!");

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

@implementation PHPublisherContentRewardsTest

- (void)testValidation
{
    NSString *reward    = @"SLAPPY_COINS";
    NSNumber *quantity  = [NSNumber numberWithInt:1234];
    NSNumber *receipt   = [NSNumber numberWithInt:102930193];
    NSString *signature = [PHStringUtil hexDigestForString:[NSString stringWithFormat:@"%@:%@:%@:%@:%@",
                                                            reward, quantity, PHGID(), receipt, PUBLISHER_SECRET]];

    NSDictionary *rewardDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    reward, @"reward",
                                                    quantity, @"quantity",
                                                    receipt, @"receipt",
                                                    signature, @"signature", nil];
    NSDictionary *badRewardDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    reward, @"reward",
                                                    quantity, @"quantity",
                                                    receipt, @"receipt",
                                                    @"BAD_SIGNATURE_RARARA", @"signature", nil];

    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:PUBLISHER_TOKEN
                                                                           secret:PUBLISHER_SECRET];

    STAssertTrue([request isValidReward:rewardDict], @"PHPublisherContentRequest could not validate valid reward.");
    STAssertFalse([request isValidReward:badRewardDict], @"PHPublisherContentRequest validated invalid reward.");
}
@end

@implementation PHPublisherContentPurchasesTest

- (void)testValidation
{
    NSString *product   = @"com.playhaven.example.candy";
    NSString *name      = @"Delicious Candy";
    NSNumber *quantity  = [NSNumber numberWithInt:1234];
    NSNumber *receipt   = [NSNumber numberWithInt:102930193];
    NSString *signature = [PHStringUtil hexDigestForString:
                                                [NSString stringWithFormat:@"%@:%@:%@:%@:%@:%@",
                                                                product, name, quantity, PHGID(),
                                                                receipt, PUBLISHER_SECRET]];
    NSNumber *cookie    = [NSNumber numberWithInt:3423413];

    NSDictionary *purchaseDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    product, @"product",
                                                    name, @"name",
                                                    quantity, @"quantity",
                                                    receipt, @"receipt",
                                                    signature, @"signature",
                                                    cookie, @"cookie", nil];

    NSDictionary *purchasesDict = [NSDictionary dictionaryWithObject:[NSArray arrayWithObject:purchaseDict]
                                                              forKey:@"purchases"];

    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:PUBLISHER_TOKEN
                                                                           secret:PUBLISHER_SECRET];

    STAssertTrue([request isValidPurchase:purchaseDict], @"PHPublisherContentRequest could not validate valid purchase");
    STAssertNoThrow([request requestPurchases:purchasesDict callback:nil source:nil], @"Problem processing valid purchases array");
}

- (void)testAlternateValidation
{
    NSString *product   = @"com.playhaven.example.candy";
    NSString *name      = @"Delicious Candy";
    NSNumber *quantity  = [NSNumber numberWithInt:1234];
    NSString *receipt   = @"102930193";
    NSString *signature = [PHStringUtil hexDigestForString:
                                                [NSString stringWithFormat:@"%@:%@:%@:%@:%@:%@",
                                                                product, name, quantity, PHGID(),
                                                                receipt, PUBLISHER_SECRET]];
    NSString *cookie    = @"3423413";

    NSDictionary *purchaseDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                                    product, @"product",
                                                    name, @"name",
                                                    quantity, @"quantity",
                                                    receipt, @"receipt",
                                                    signature, @"signature",
                                                    cookie, @"cookie", nil];

    NSDictionary *purchasesDict = [NSDictionary dictionaryWithObject:[NSArray arrayWithObject:purchaseDict]
                                                              forKey:@"purchases"];

    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:PUBLISHER_TOKEN
                                                                           secret:PUBLISHER_SECRET];

    STAssertTrue([request isValidPurchase:purchaseDict], @"PHPublisherContentRequest could not validate valid purchase");
    STAssertNoThrow([request requestPurchases:purchasesDict callback:nil source:nil], @"Problem processing valid purchases array");
}
@end

@implementation PHPublisherContentRequestPreservationTest

- (void)testPreservation
{
    PHPublisherContentRequest *request                   =
          [PHPublisherContentRequest requestForApp:@"token1" secret:@"secret1" placement:@"placement1" delegate:nil];
    PHPublisherContentRequest *requestIdentical          =
          [PHPublisherContentRequest requestForApp:@"token1" secret:@"secret1" placement:@"placement1" delegate:nil];
    PHPublisherContentRequest *requestDifferentToken     =
          [PHPublisherContentRequest requestForApp:@"token2" secret:@"secret2" placement:@"placement1" delegate:nil];
    PHPublisherContentRequest *requestDifferentPlacement =
          [PHPublisherContentRequest requestForApp:@"token1" secret:@"secret1" placement:@"placement2" delegate:nil];


    STAssertTrue(request == requestIdentical, @"These requests should be the same instance!");
    STAssertTrue(request != requestDifferentPlacement, @"These requests should be different!");
    STAssertTrue(request != requestDifferentToken, @"These requests should be different!");

    NSString *newDelegate = @"DELEGATE";
    PHPublisherContentRequest *requestNewDelegate = [PHPublisherContentRequest requestForApp:@"token1" secret:@"secret1" placement:@"placement1" delegate:newDelegate];

    STAssertTrue((id)requestNewDelegate.delegate == (id)newDelegate, @"This request should have had its delegate reassigned!");
}
@end

@implementation PHPublisherContentPreloadTest

- (void)setUp
{
    _request = [[PHPublisherContentRequest requestForApp:@"zombie1"
                                                  secret:@"haven1"
                                               placement:@"more_games"
                                                delegate:self] retain];
    _didPreload = NO;
}

- (void)requestDidGetContent:(PHPublisherContentRequest *)request
{
    _didPreload = YES;
}

- (void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content
{
    STAssertTrue(FALSE, @"This isn't supposed to happen!");
}

- (void)tearDown
{
    STAssertTrue(_didPreload, @"Preloading didn't happen!");
    STAssertTrue([_request state] == PHPublisherContentRequestPreloaded,@"Request wasn't preloaded!");

    [_request release], _request = nil;
}
@end

@implementation PHPublisherContentPreloadParameterTest

- (void)testPreloadParameterWhenPreloading
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:@"zombie1"
                                                                           secret:@"haven1"
                                                                        placement:@"more_games"
                                                                         delegate:nil];
    [request preload];

    NSString *parameters = [request.URL absoluteString];
    STAssertFalse([parameters rangeOfString:@"preload=1"].location == NSNotFound, @"Expected 'preload=1' in parameter string, did not find it!");
    [request cancel];
}

- (void)testPreloadParameterWhenSending
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:@"zombie1"
                                                                           secret:@"haven1"
                                                                        placement:@"more_games"
                                                                         delegate:nil];
    [request send];

    NSString *parameters = [request.URL absoluteString];
    STAssertFalse([parameters rangeOfString:@"preload=0"].location == NSNotFound, @"Expected 'preload=0' in parameter string, did not find it!");
    [request cancel];
}
@end

@implementation PHPublisherContentStateTest

- (void)testStateChanges
{
    PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:@"zombie1"
                                                                           secret:@"haven1"
                                                                        placement:@"more_games"
                                                                         delegate:nil];

    STAssertTrue(request.state == PHPublisherContentRequestInitialized, @"Expected initialized state, got %d", request.state);
    STAssertTrue([request setPublisherContentRequestState:PHPublisherContentRequestPreloaded], @"Expected to be able to advance state!");
    STAssertFalse([request setPublisherContentRequestState:PHPublisherContentRequestPreloading], @"Expected not to be able to regress state!");

    [request cancel];
}
@end
