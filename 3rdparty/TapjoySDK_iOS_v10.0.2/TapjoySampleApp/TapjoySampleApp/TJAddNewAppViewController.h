// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>

@interface TJAddNewAppViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

@property (weak, nonatomic) IBOutlet UITableView *currencyTable;
@property (weak, nonatomic) IBOutlet UIButton *addCurrencyButton;

- (IBAction)currencySwitched:(id)sender;
- (IBAction)addCurrency:(id)sender;
- (IBAction)saveNewApp:(id)sender;

@end
