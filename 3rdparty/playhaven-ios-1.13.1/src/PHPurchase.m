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

 PHPurchase.m
 playhaven-sdk-ios

 Created by Thomas DiZoglio on 1/12/12.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import "PHPurchase.h"
#import "PHConstants.h"

@implementation PHPurchase
@synthesize productIdentifier = _productIdentifier;
@synthesize name     = _item;
@synthesize quantity = _quantity;
@synthesize receipt  = _receipt;
@synthesize callback = _callback;

+ (NSString *)stringForResolution:(PHPurchaseResolutionType)resolution
{
    NSString *result = @"error";
    switch (resolution) {
        case PHPurchaseResolutionBuy:
            result = @"buy";
            break;

        case PHPurchaseResolutionCancel:
            result = @"cancel";
            break;

        case PHPurchaseResolutionFailure:
            result = @"failure";
            break;

        default:
            result = @"error";
            break;
    }

    return result;
}

- (void)dealloc
{
    [_productIdentifier release], _productIdentifier = nil;
    [_item release], _item = nil;
    [_receipt release], _receipt = nil;
    [_callback release], _callback = nil;

    [super dealloc];
}

- (void)reportResolution:(PHPurchaseResolutionType)resolution
{
    NSDictionary *response =
         [NSDictionary dictionaryWithObjectsAndKeys:
                            [PHPurchase stringForResolution:resolution], @"resolution", nil];
    NSDictionary *callbackDictionary =
         [NSDictionary dictionaryWithObjectsAndKeys:
                            self.callback, @"callback",
                            response,      @"response", nil];

    [[NSNotificationCenter defaultCenter] postNotificationName:PHCONTENTVIEW_CALLBACK_NOTIFICATION
                                                        object:callbackDictionary];
}
@end
