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

 PublisherIAPTrackingViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 2/23/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PublisherIAPTrackingViewController.h"
#import "NSData+Base64.h"

@implementation PublisherIAPTrackingViewController
@synthesize productField;
@synthesize quantityField;
@synthesize receiptField;
@synthesize resolutionSegment;

- (void)startRequest
{
    [super startRequest];

    /*
     * This is an alternate implementation which allows you you get response
     * data from API requests. This isn't necessary for most developers.
     */
    PHPublisherIAPTrackingRequest *request = [PHPublisherIAPTrackingRequest requestForApp:self.token
                                                                                   secret:self.secret];
    request.delegate = self;

    request.product  = ([self.productField.text isEqualToString:@""]) ?
                                @"com.playhaven.example.candy" :
                                self.productField.text;
    request.quantity = ([self.quantityField.text isEqualToString:@""]) ?
                                1 : [self.quantityField.text integerValue];

    request.receiptData = ([self.receiptField.text isEqualToString:@""]) ?
                                nil : [NSData decodeBase64WithString:self.receiptField.text];

    request.resolution = (PHPurchaseResolutionType)[self.resolutionSegment selectedSegmentIndex];
    request.error      = PHCreateError(PHIAPTrackingSimulatorErrorType);
    [request send];

    [self.productField resignFirstResponder];
    [self.quantityField resignFirstResponder];
}

- (void)dealloc
{
    [PHAPIRequest cancelAllRequestsWithDelegate:self];
    [productField release];
    [quantityField release];
    [resolutionSegment release];
    [receiptField release];
    [super dealloc];
}

#pragma mark - PHAPIRequestDelegate
- (void)request:(PHAPIRequest *)request didSucceedWithResponse:(NSDictionary *)responseData
{
    NSString *urlMessage = [NSString stringWithFormat:@"URL: %@", request.URL];
    [self addMessage:urlMessage];

    NSString *message    = [NSString stringWithFormat:@"[OK] Success with response: %@", responseData];
    [self addMessage:message];

    [self finishRequest];
}

- (void)request:(PHAPIRequest *)request didFailWithError:(NSError *)error
{
    NSString *urlMessage = [NSString stringWithFormat:@"URL: %@", request.URL];
    [self addMessage:urlMessage];

    NSString *message    = [NSString stringWithFormat:@"[ERROR] Failed with error: %@", error];
    [self addMessage:message];

    [self finishRequest];
}

- (void)viewDidUnload
{
    [self setProductField:nil];
    [self setQuantityField:nil];
    [self setResolutionSegment:nil];
    [super viewDidUnload];
}
@end
