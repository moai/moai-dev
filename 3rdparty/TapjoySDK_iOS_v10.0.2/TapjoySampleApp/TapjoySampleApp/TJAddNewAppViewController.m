// Copyright (C) 2014 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJAddNewAppViewController.h"
#import "TJAppModel.h"
#import "TJEventDataStore.h"
#import "TJUtils.h"

@interface TJAddNewAppViewController ()

@property (nonatomic, strong) TJAppModel* tempApp;
@property (weak, nonatomic) IBOutlet UIView *deviceInfoView;
@property (weak, nonatomic) IBOutlet UIView *appIDView;
@property (weak, nonatomic) IBOutlet UIView *currencyIDView;
@property (weak, nonatomic) IBOutlet UITextField *appIDTextField;
@property (weak, nonatomic) IBOutlet UITextField *secretKeyTextField;
@property (weak, nonatomic) IBOutlet UISwitch *managedCurrencySwitch;
@property (weak, nonatomic) IBOutlet UITextField *currencyIDTextField;
@property (weak, nonatomic) IBOutlet UIButton *saveButton;

@end

@implementation TJAddNewAppViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];

  _tempApp = [[TJAppModel alloc] init];
    
  [_appIDTextField becomeFirstResponder];
	
	// Round the corners.
	[TJUtils applyRoundedCorners:_deviceInfoView];
	[TJUtils applyTopRoundedCorners:_appIDView];
	[TJUtils applyBottomRoundedCorners:_currencyIDView];
	[TJUtils applyRoundedCorners:_saveButton];
	
	
	[_currencyTable setUserInteractionEnabled:NO];
	[_currencyTable setAlpha:0.5];
	[_addCurrencyButton setUserInteractionEnabled:NO];
	[_addCurrencyButton setAlpha:0.5];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    _tempApp = nil;
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (void)viewDidUnload {
    [self setAppIDTextField:nil];
    [self setSecretKeyTextField:nil];
    [self setManagedCurrencySwitch:nil];
    [self setCurrencyTable:nil];
    [self setTempApp:nil];
    [self setAddCurrencyButton:nil];
    [self setCurrencyIDTextField:nil];
	[self setDeviceInfoView:nil];
	[self setAppIDView:nil];
	[self setCurrencyIDView:nil];
	[self setSaveButton:nil];
    [super viewDidUnload];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    if ((toInterfaceOrientation == UIInterfaceOrientationPortrait) ||
        (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft))
        return YES;
    
    return NO;
}

#pragma mark - Table View Datasource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if ([_tempApp currencyIDs])
        return [[_tempApp currencyIDs] count];
    
    return 0;
}

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    if ([_tempApp currencyIDs] && [[_tempApp currencyIDs] count] > [indexPath row])
        cell.textLabel.text = [[_tempApp currencyIDs] objectAtIndex:[indexPath row]];
    
    return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return @"Currency IDs";
}

#pragma mark - Table View Delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

# pragma mark - Textfield Delegate
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if (textField == _appIDTextField)
    {
        [_secretKeyTextField becomeFirstResponder];
    }
    else if (textField == _secretKeyTextField)
    {
        [_currencyIDTextField becomeFirstResponder];
    }
    else if (textField == _currencyIDTextField)
    {
        [textField resignFirstResponder];
    }
    return NO;
}

# pragma mark - Save

- (void)show
{

}

- (IBAction)saveNewApp:(id)sender
{	
	if (![TJUtils isValidAppID:_appIDTextField.text] || ![TJUtils isValidSecretKey:_secretKeyTextField.text])
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
														message:@"You must enter a valid App ID and Secret Key"
													   delegate:self
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert show];
		return;
	}
	
    if (_appIDTextField.text)
        [_tempApp setAppID:_appIDTextField.text];
    
    if (_secretKeyTextField.text)
        [_tempApp setSecretKey:_secretKeyTextField.text];

    [_tempApp setManagedCurrency:[_managedCurrencySwitch isOn]];
	
	[[TJEventDataStore defaultEventDataStore] saveApp:_tempApp];
    
    [self.navigationController popViewControllerAnimated:YES];
}

# pragma mark - New App Editing


- (IBAction)currencySwitched:(id)sender
{
	CGFloat alpha = [sender isOn] ? 0.5 : 1;
	// Is managed currency on?
	[_currencyTable setUserInteractionEnabled:![sender isOn]];
	[_currencyTable setAlpha:alpha];
	[_addCurrencyButton setUserInteractionEnabled:![sender isOn]];
	[_addCurrencyButton setAlpha:alpha];
}

- (IBAction)addCurrency:(id)sender
{
	if (![TJUtils isValidCurrencyID:_currencyIDTextField.text])
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
														message:@"You must enter a valid Currency ID"
													   delegate:self
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert show];
		return;
	}
	
    // only add new currency if add button is explicitely pressed
    
    // add if text was entered
    if ([_currencyIDTextField.text length] > 0)
    {
        NSMutableArray *tempArray = [[NSMutableArray alloc] initWithArray:[_tempApp currencyIDs]];
        [tempArray addObject:_currencyIDTextField.text];
        
        [_tempApp setCurrencyIDs:tempArray];
        
        [_currencyTable reloadData];
    }
    
    [_currencyIDTextField setText:nil];
}
@end
