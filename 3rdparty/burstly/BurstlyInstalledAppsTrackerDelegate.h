//
//  InstalledAppsTrackerDelegate.h
//  Burstly
//
//  Created by Fedor Kudrys on 12/29/11.
//  Copyright (c) 2011 Burslty. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol BurstlyInstalledAppsTrackerDelegate <NSObject>

@optional
- (void) installedAppsTrackerDidLoadAppList:(NSArray*) appList;
- (void) installedAppsTrackerDidFailToLoadAppList:(NSError*) error; 
@end
