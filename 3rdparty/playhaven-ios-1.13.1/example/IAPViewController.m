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

 IAPViewController.m
 playhaven-sdk-ios

 Created by Jesus Fernandez on 7/24/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "IAPViewController.h"
#import "IAPHelper.h"
#import "PHPurchase.h"

@interface IAPViewController ()

@end

@implementation IAPViewController
@synthesize productField  = _productField;
@synthesize quantityField = _quantityField;

- (void)dealloc
{
    [_productField release], _productField = nil;
    [_quantityField release], _quantityField = nil;
    [super dealloc];
}

#pragma mark -
- (void)startRequest
{
    [super startRequest];

    PHPurchase *purchase = [PHPurchase new];

    purchase.productIdentifier = ([self.productField.text isEqualToString:@""]) ?
                                            @"com.playhaven.example.candy" :
                                            self.productField.text;
    purchase.quantity          = ([self.quantityField.text isEqualToString:@""]) ?
                                            1 : [self.quantityField.text integerValue];

    [[IAPHelper sharedIAPHelper] startPurchase:purchase];

    [purchase release];

    [super finishRequest];
}
@end
