// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJVirtualCurrencyViewController.h"
#import "TJUtils.h"
#import <Tapjoy/Tapjoy.h>

typedef enum TJCellTag
{
	TJCellTagGetTapPoints = 0,
	TJCellTagSpendTapPoints,
	TJCellTagAwardTapPoints,
} TJCellTag;

@interface TJVirtualCurrencyViewController ()

@property (nonatomic, strong) NSArray* vcMethods;
@property (weak, nonatomic) IBOutlet TJDebugTextView *debugLog;

@end

@implementation TJVirtualCurrencyViewController

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder])
	{
        _vcMethods = @[@"GetTapPointsCell", @"SpendTapPointsCell", @"AwardTapPointsCell"];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    [_debugLog setEditable:NO];
    [_debugLog setText:@""];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;

	[TJUtils applyRoundedCorners:_debugLog];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    if ((toInterfaceOrientation == UIInterfaceOrientationPortrait) ||
        (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft))
        return YES;
    
    return NO;
}

-(void)logTapPointsError
{
    // Print out the updated points value.
    [_debugLog log:@"Error occurred, be sure to validate currency errors and not allow purchases if there is an error"];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [_vcMethods count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSString *sectionName;
    switch (section)
    {
        case 0:
            sectionName = NSLocalizedString(@"API Calls", @"The API Calls available to the developer");
            break;
        default:
            sectionName = @"API Calls";
            break;
    }
    return sectionName;
}

- (NSString *)tableView:(UITableView *)tableView titleForFooterInSection:(NSInteger)section
{
    return @"Debug Log";
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:_vcMethods[indexPath.row]];
    if (cell == nil)
	{
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:_vcMethods[indexPath.row]];
    }
    
    // Configure the cell...
    cell.textLabel.text = [_vcMethods objectAtIndex:[indexPath row]];
    
    return cell;
}



#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	switch (indexPath.row)
	{
		case TJCellTagGetTapPoints:
		{
			[Tapjoy getTapPointsWithCompletion:^(NSDictionary *parameters, NSError *error) {
				if (error)
					[self logTapPointsError];
				else
				{
					[_debugLog log:[NSString stringWithFormat:@"getTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
				}
			}];
		}
			break;
			
		case TJCellTagSpendTapPoints:
		{
			[Tapjoy spendTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
				if (error)
					[self logTapPointsError];
				else
					[_debugLog log:[NSString stringWithFormat:@"spendTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
			}];
		}
			break;
			
		case TJCellTagAwardTapPoints:
		{
			[Tapjoy awardTapPoints:10 completion:^(NSDictionary *parameters, NSError *error) {
				if (error)
					[self logTapPointsError];
				else
				{
					[_debugLog log:[NSString stringWithFormat:@"awardTapPoints returned %@: %d", parameters[@"currencyName"], [parameters[@"amount"] intValue]]];
					
					// Pops up a UIAlert notifying the user that they have successfully earned some currency.
					// This is the default alert, so you may place a custom alert here if you choose to do so.
					[Tapjoy showDefaultEarnedCurrencyAlert];
				}
			}];
		}
			break;
			
		default:
			break;
	}
	
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)viewDidUnload {
    [self setDebugLog:nil];
    [super viewDidUnload];
}
@end
