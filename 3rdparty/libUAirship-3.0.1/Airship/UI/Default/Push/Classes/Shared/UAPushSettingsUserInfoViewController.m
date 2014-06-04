/*
 Copyright 2009-2013 Urban Airship Inc. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binaryform must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided withthe distribution.

 THIS SOFTWARE IS PROVIDED BY THE URBAN AIRSHIP INC``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 EVENT SHALL URBAN AIRSHIP INC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "UAPushSettingsUserInfoViewController.h"
#import "UAUser.h"

#if __IPHONE_OS_VERSION_MAX_ALLOWED < 60000
// This is available in iOS 6.0 and later, define it for older versions
#define NSLineBreakByWordWrapping 0
#endif

@implementation UAPushSettingsUserInfoViewController


- (void)viewDidLoad {
    [super viewDidLoad];

    self.title = @"Username";

    self.text = @"Your current username. Test a rich push notification at "
    @"https://go.urbanairship.com";
}

- (void)viewWillAppear:(BOOL)animated {
    self.usernameLabel.text = [UAUser defaultUser].username ?: @"Unavailable";
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)viewDidUnload {
    [super viewDidUnload];
    self.emailButton = nil;
    self.usernameLabel = nil;
}

#pragma mark -
#pragma mark UITableViewDelegate

#define kCellPaddingHeight 10

- (CGFloat)tableView: (UITableView *) tableView heightForRowAtIndexPath:(NSIndexPath *) indexPath {
    UIFont *font = [UIFont systemFontOfSize:17];
    CGFloat height = [self.text sizeWithFont:font
                           constrainedToSize:CGSizeMake(280.0, 1500.0)
                               lineBreakMode:NSLineBreakByWordWrapping].height;
    return height + kCellPaddingHeight;
}

#pragma mark -
#pragma mark UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UIImage *bgImage = [UIImage imageNamed:@"middle-detail.png"];
    UIImage *stretchableBgImage = [bgImage stretchableImageWithLeftCapWidth:20 topCapHeight:0];
    UIImageView *bgImageView = [[UIImageView alloc] initWithImage: stretchableBgImage];

    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"description-cell"];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:@"description-cell"];
    }

    UIFont *font = [UIFont systemFontOfSize: 17];

    UILabel* description = [[UILabel alloc] init];
    description.text = self.text;
    description.lineBreakMode = UILineBreakModeWordWrap;
    description.numberOfLines = 0;
    description.backgroundColor = [UIColor clearColor];
    [description setFont: font];
    CGFloat height = [self.text sizeWithFont:font
                           constrainedToSize:CGSizeMake(280.0, 800.0)
                               lineBreakMode:NSLineBreakByWordWrapping].height;
    [description setFrame: CGRectMake(0.0f, 10.0f, 320.0f, height)];
    [description setBounds: CGRectMake(0.20f, 0.0f, 290.0f, height)];
    [description setAutoresizingMask:UIViewAutoresizingFlexibleWidth];

    [cell addSubview: description];
    [cell setSelectionStyle: UITableViewCellSelectionStyleNone];
    [cell setBackgroundView: bgImageView];


    return cell;
}

#pragma mark -
#pragma mark UI Button Actions
- (IBAction)copyUsername {
    NSString *username = [UAUser defaultUser].username;
    if (username) {
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        pasteboard.string = username;
    }
}

- (IBAction)emailUsername {

    if ([MFMailComposeViewController canSendMail]) {
		MFMailComposeViewController *mfViewController = [[MFMailComposeViewController alloc] init];
		mfViewController.mailComposeDelegate = self;



        NSString *messageBody = [NSString stringWithFormat:@"Your username is %@\n\nSend a test rich push at http://go.urbanairship.com", [UAUser defaultUser].username];

        [mfViewController setSubject:@"Username"];
        [mfViewController setMessageBody:messageBody isHTML:NO];

		[self presentModalViewController:mfViewController animated:YES];
	}else {
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"Your device is not currently configured to send mail." delegate:nil cancelButtonTitle:@"ok" otherButtonTitles:nil];

		[alert show];
	}
}

#pragma mark -
#pragma mark MFMailComposeViewControllerDelegate Methods

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error {
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Message Status" message:@"" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];

	switch (result) {
		case MFMailComposeResultCancelled:
			//alert.message = @"Canceled";
			break;
		case MFMailComposeResultSaved:
			//alert.message = @"Saved";
			break;
		case MFMailComposeResultSent:
			alert.message = @"Sent";
            [alert show];
			break;
		case MFMailComposeResultFailed:
			//alert.message = @"Message Failed";
			break;
		default:
			//alert.message = @"Message Not Sent";
            break;
    }

	[self dismissModalViewControllerAnimated:YES];


}

@end
