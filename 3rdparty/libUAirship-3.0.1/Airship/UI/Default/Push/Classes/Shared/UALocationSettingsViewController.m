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

#import "UALocationSettingsViewController.h"
#import "UAMapPresentationController.h"
#import "UAGlobal.h"
#import "UALocationCommonValues.h"
#import "UAMapPresentationController.h"
#import "UAirship.h"
#import "UALocationService.h"

@implementation UALocationSettingsViewController


- (void)viewDidUnload {
    [self turnOffLocationDisplay];
    self.locationTableView = nil;
    self.locationDisplay = nil;
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.reportedLocations = [NSMutableArray arrayWithCapacity:10];
    self.locationDisplay = [NSMutableArray arrayWithCapacity:3];
    [self setupLocationDisplay];
    [self.locationTableView setScrollEnabled:NO];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.locationService = [[UAirship shared] locationService];
    self.locationService.singleLocationDesiredAccuracy = kCLLocationAccuracyHundredMeters;
    self.locationService.timeoutForSingleLocationService = 10.0;
    self.locationService.delegate = self;
}

- (void)viewWillDisappear:(BOOL)animated {
    self.locationService.delegate = nil;
    self.locationService = nil;
    [super viewWillDisappear:animated];
}


#pragma mark -
#pragma mark Rotation
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return NO;
}

#pragma mark -
#pragma mark GUI operations

- (void)turnOffLocationDisplay {
    [self.locationDisplay removeObjectsInRange:NSMakeRange(1, ([self.locationDisplay count] -1))];
    NSUInteger rows = [self.locationTableView numberOfRowsInSection:0];
    NSMutableArray *arrayOfDeletes = [NSMutableArray arrayWithCapacity:3];
    for (NSUInteger i=1; i < rows; i++) {
        NSIndexPath *path = [NSIndexPath indexPathForRow:i inSection:0];
        [arrayOfDeletes addObject:path];
    }
    [self.locationTableView deleteRowsAtIndexPaths:arrayOfDeletes withRowAnimation:UITableViewRowAnimationFade];
}

- (void)setupLocationDisplay {
    [self.locationDisplay addObject:@"Location"];
    [self.locationDisplay addObject:@"Latitude: "];
    [self.locationDisplay addObject:@"Longitude: "];
}

#pragma mark -
#pragma mark LocationSettings methods

- (void)addLocationToData:(CLLocation*)location {
    [self.reportedLocations addObject:location];
    [self.locationTableView reloadData];
}

#pragma mark -
#pragma mark IBAction Button Methods

- (IBAction)getLocationPressed:(id)sender {
    UALOG(@"Get Location pressed");
    [self checkAndAlertForLocationAuthorization];
    [self.locationService reportCurrentLocation];
}

- (void)checkAndAlertForLocationAuthorization {
    // Check the system level permissions (global location settings)
    BOOL locationServiceEnabled = [UALocationService locationServicesEnabled];
    // Check the system level per app location settings
    BOOL locationServiceAuthorized = [UALocationService locationServiceAuthorized];
    // Check if Urban Airship is allowed to use location
    BOOL airshipAllowedToUseLocation = [UALocationService airshipLocationServiceEnabled];
    if (!(locationServiceEnabled && locationServiceAuthorized && airshipAllowedToUseLocation)) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Location Error" 
                                                            message:@"The location service is either, not authorized, enabled, or Urban Airship does not have permissinon to use it" 
                                                           delegate:nil 
                                                  cancelButtonTitle:@"Dismiss" 
                                                  otherButtonTitles:nil];
        [alertView show];
    }
}

- (IBAction)mapLocationPressed:(id)sender{
    UAMapPresentationController *mapController = [[UAMapPresentationController alloc] initWithNibName:@"UAMapPresentationViewController"
                                                                                               bundle:[NSBundle mainBundle]];
    mapController.locations = self.reportedLocations;
    [self.navigationController pushViewController:mapController animated:YES];
}

#pragma mark -
#pragma mark UITableViewDataSource

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = nil;
    if (0 == [indexPath indexAtPosition:1]) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
        cell.textLabel.text = [self.locationDisplay objectAtIndex:0];
        cell.textLabel.textAlignment = UITextAlignmentCenter;
    }
    if(1 == [indexPath indexAtPosition:1]) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"latitude"];
        cell.textLabel.text = [self.locationDisplay objectAtIndex:[indexPath indexAtPosition:1]];
        CLLocation  *location = [self.reportedLocations lastObject];
        if (!location) {
            UALOG(@"Empty lat value");
            cell.detailTextLabel.text = @"";
            return cell;
        }
        cell.detailTextLabel.text = [NSString stringWithFormat:@"%.7f", location.coordinate.latitude];
    }
    if (2 == [indexPath indexAtPosition:1]) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"longitude"];
        cell.textLabel.text = [self.locationDisplay objectAtIndex:[indexPath indexAtPosition:1]];
        CLLocation *location = [self.reportedLocations lastObject];
        if (!location) {
            UALOG(@"Empty long value");
            cell.detailTextLabel.text = @"";
            return cell;
        }
        cell.detailTextLabel.text = [NSString stringWithFormat:@"%.7f", location.coordinate.longitude];                    
    }
    return cell ;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self.locationDisplay count];
}

#pragma mark -
#pragma mark UALocationServiceDelegate

- (void)locationService:(UALocationService*)service didFailWithError:(NSError*)error {
    UALOG(@"LOCATION_ERROR, %@", error.description);
    CLLocation *bestAvailableLocation = [[error userInfo] valueForKey:UALocationServiceBestAvailableSingleLocationKey];
    // Desired accuracy couldn't be met, the service timed out to save battery, this is the best location obtained
    if (bestAvailableLocation) {
        [self addLocationToData:bestAvailableLocation];
    }
}
- (void)locationService:(UALocationService*)service didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
    UALOG(@"LOCATION_AUTHORIZATION_STATUS %u", status);
}
- (void)locationService:(UALocationService*)service didUpdateToLocation:(CLLocation*)newLocation fromLocation:(CLLocation*)oldLocation {
    UALOG(@"LOCATION_UPDATE LAT:%f LONG:%f", newLocation.coordinate.latitude, newLocation.coordinate.longitude);
    [self addLocationToData:newLocation];
}


@end
