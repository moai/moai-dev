// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>
#import <Tapjoy/TJEvent.h>
#import "TJDebugTextView.h"

@interface TJEventsViewController : UIViewController<UIActionSheetDelegate, TJEventDelegate>

- (id)initWithEvents:(NSArray *)events;

@end
