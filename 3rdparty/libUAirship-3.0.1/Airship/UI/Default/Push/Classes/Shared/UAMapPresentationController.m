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
#import "UAMapPresentationController.h"
#import "UAGlobal.h"
#import "UALocationDemoAnnotation.h"
#import "UALocationService.h"
#import "UAGlobal.h"

@implementation UAMapPresentationController

#pragma mark -
#pragma mark Memory



#pragma mark -
#pragma mark View Cycle
- (void)viewDidLoad
{
    [super viewDidLoad];
    if (!self.locations) {
        self.locations = [NSMutableArray array];
    }
    UA_LDEBUG(@"LOCATIONS ARRAY %@", self.locations);
    self.annotations = [NSMutableArray array];
    [self convertLocationsToAnnotations];
    self.navigationItem.rightBarButtonItem = self.rightButton;
}

- (void)viewDidUnload
{
    self.mapView = nil;
    self.rightButton = nil;
    [super viewDidUnload];
}

- (void)viewWillDisappear:(BOOL)animated {
    self.mapView.delegate = nil; // delegate is set in xib
    [super viewWillDisappear:animated];
}

#pragma mark -
#pragma mark Autorotation

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

#pragma mark -
#pragma mark Class Methods

- (void)moveSpanToCoordinate:(CLLocationCoordinate2D)location {
    MKCoordinateSpan span = MKCoordinateSpanMake(0.05, 0.05);
    MKCoordinateRegion region = MKCoordinateRegionMake(location, span);
    [self.mapView setRegion:region animated:NO  ];
}

- (void)convertLocationsToAnnotations {
    for (CLLocation* location in self.locations) {
        UALocationDemoAnnotation *annotation = [UALocationDemoAnnotation locationAnnotationFromLocation:location];
        [self.annotations addObject:annotation];
    }
    UA_LDEBUG(@"ANNOTATIONS %@", self.annotations);
}

- (void)annotateMap {
    UA_LDEBUG(@"annotateMap");
    [self.mapView addAnnotations:self.annotations];
    self.rightButton.title = @"-Pin";
}

- (IBAction)rightBarButtonPressed:(id)sender {
    UA_LDEBUG(@"Right bar button pressed");
    // The Map                   
    if (self.mapView.annotations.count > 1) {
        UA_LDEBUG(@"Removing annotations");
        [self.mapView removeAnnotations:self.annotations];
        self.rightButton.title = @"+Pin";
    }
    else {
        UA_LDEBUG(@"Adding annotations");
        [self annotateMap];
    }
}


#pragma mark -
#pragma mark MKMapViewDelegate 

- (void)mapView:(MKMapView *)mapView didChangeUserTrackingMode:(MKUserTrackingMode)mode animated:(BOOL)animated {
    UA_LDEBUG(@"didChangeUserTrackingMode");
}

- (MKAnnotationView *)mapView:(MKMapView *)mapView viewForAnnotation:(id < MKAnnotation >)annotation {
    // Return nil for the MKUserLocation object
    if ([annotation isKindOfClass:[MKUserLocation class]]) {
        self.lastUserAnnotation = annotation;
        UA_LDEBUG(@"Returning nil for MKUserLocation Lat:%f Long:%f", annotation.coordinate.latitude, annotation.coordinate.longitude);
        return nil;
    }
    UA_LDEBUG(@"Creating view for annotation %@", annotation);
    
    if (!annotation) {
        UA_LDEBUG(@"ANNOTATION IS NIL!!!!");
    }
    MKPinAnnotationView *pinView = [[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:nil];
    pinView.pinColor = MKPinAnnotationColorPurple;
    pinView.animatesDrop = YES;
    return pinView;
}

- (void)mapView:(MKMapView *)mapView didAddAnnotationViews:(NSArray *)views {
    UA_LDEBUG(@"Annotations added to map %@", views);
    if ([views count] > 0) {
       MKAnnotationView *view = [views objectAtIndex:0];
        CLLocationCoordinate2D coord = view.annotation.coordinate;
        [self moveSpanToCoordinate:coord];
    }
}

@end
