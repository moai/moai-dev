/*
 * Copyright 2012 Facebook
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <FacebookSDK/FacebookSDK.h>
#import "PPViewController.h"
#import "PPAppDelegate.h"

enum SampleLocation {
    SampleLocationSeattle,
    SampleLocationSanFrancisco,
    SampleLocationGPS,
};

// FBSample logic
// We need to handle some of the UX events related to friend selection, and so we declare
// that we implement the FBPlacePickerDelegate here; the delegate lets us filter the view
// as well as handle selection events
@interface PPViewController () <CLLocationManagerDelegate, FBPlacePickerDelegate>

@property (strong, nonatomic) CLLocationManager *locationManager;
@property (nonatomic) NSInteger viewStateSearchScopeIndex;
@property (nonatomic, copy) NSString *viewStateSearchText;
@property (nonatomic) BOOL viewStateSearchWasActive;

- (void)refresh;

@end

@implementation PPViewController

@synthesize locationManager = _locationManager;
@synthesize viewStateSearchScopeIndex = _viewStateSearchScopeIndex;
@synthesize viewStateSearchText = _viewStateSearchText;
@synthesize viewStateSearchWasActive = _viewStateSearchWasActive;

// FBSample logic
// This method is responsible for keeping UX and session state in sync
- (void)refresh {
    // if the session is open, then load the data for our view controller
    if (FBSession.activeSession.isOpen) {
        // Default to Seattle, this method calls loadData
        [self searchDisplayController:nil shouldReloadTableForSearchScope:SampleLocationSeattle];
    } else { 
        // if the session isn't open, we open it here, which may cause UX to log in the user
        [FBSession openActiveSessionWithPermissions:nil
                                       allowLoginUI:YES 
                                  completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
                                      if (!error) {
                                          [self refresh];
                                      } else {
                                          [[[UIAlertView alloc] initWithTitle:@"Error"
                                                                      message:error.localizedDescription
                                                                     delegate:nil
                                                            cancelButtonTitle:@"OK" 
                                                            otherButtonTitles:nil]
                                           show];
                                      }
                                  }];
    }
}

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchScope:(NSInteger)searchOption {
    switch (searchOption) {
        case SampleLocationSeattle:
            // FBSample logic
            // After setting the coordinates for the data we wish to fetch, we call loadData to initiate 
            // the actual network round-trip with Facebook; likewise for the other two locations
            self.locationCoordinate = CLLocationCoordinate2DMake(47.6097, -122.3331);
            [self loadData];
            break;
        case SampleLocationSanFrancisco:
            self.locationCoordinate = CLLocationCoordinate2DMake(37.7750, -122.4183);
            [self loadData];
            break;
        case SampleLocationGPS:
            self.locationManager = [[CLLocationManager alloc] init];
            self.locationManager.delegate = self;
            [self.locationManager startUpdatingLocation];
            break;
    }
    
    // When startUpdatingLocation/loadData finish, we will reload then.
    return NO;
}

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString {
    // FBSample logic
    // When search text changes, we update the property on our base class, and then refetch data; the 
    // Scrumptious sample shows a more complex implementation of this, where frequent updates are aggregated,
    // and fetching happens on a timed basis to avoid becomming to chatty with the server; this sample takes
    // a more simplistic approach
    self.searchText = searchString;
    [self loadData];

    // Setting self.searchText will reload the table when results arrive
    return NO;
}

- (void)searchDisplayControllerDidEndSearch:(UISearchDisplayController *)controller
{
    [self.tableView reloadData];
}

- (void)placePickerViewControllerSelectionDidChange:(FBPlacePickerViewController *)placePicker
{
    // FBSample logic
    // Here we see a use of the FBGraphPlace protocol, where our code can use dot-notation to 
    // select name and location data from the selected place
    id<FBGraphPlace> place = placePicker.selection;

    // we'll use logging to show the simple typed property access to place and location info
    NSLog(@"place=%@, city=%@, state=%@, lat long=%@ %@", 
          place.name,
          place.location.city,
          place.location.state,
          place.location.latitude,
          place.location.longitude);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // FBSample logic
    // We are inheriting FBPlacePickerViewController, and so in order to handle events such 
    // as selection change, we set our base class' delegate property to self
    self.delegate = self;

    self.searchDisplayController.searchResultsDataSource = self.tableView.dataSource;
    self.searchDisplayController.searchResultsDelegate = self.tableView.delegate;

    if (self.viewStateSearchText) {
        [self.searchDisplayController.searchBar
         setSelectedScopeButtonIndex:self.viewStateSearchScopeIndex];
        [self.searchDisplayController.searchBar setText:self.viewStateSearchText];
        [self.searchDisplayController setActive:self.viewStateSearchWasActive];
        
        self.viewStateSearchText = nil;
    }

    [self refresh];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];

    self.viewStateSearchScopeIndex = [self.searchDisplayController.searchBar selectedScopeButtonIndex];
    self.viewStateSearchText = [self.searchDisplayController.searchBar text];
    self.viewStateSearchWasActive = [self.searchDisplayController isActive];
}

- (void)placePickerViewControllerDataDidChange:(FBPlacePickerViewController *)placePicker {
    [self.searchDisplayController.searchResultsTableView reloadData];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Location Manager delegate

- (void)locationManager:(CLLocationManager *)manager
	didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation {
    if (newLocation.horizontalAccuracy < 100) {
        // We wait for a precision of 100m and turn the GPS off
        [self.locationManager stopUpdatingLocation];
        self.locationManager = nil;
        
        self.locationCoordinate = newLocation.coordinate;
        [self loadData];
    }
}

@end
