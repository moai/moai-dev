//
//  Crittercism.h
//  Crittercism-iOS
//
//  Created by Alvin Liang on 2/14/12.
//  Copyright (c) 2012 Crittercism. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "CrittercismJSONKit.h"
#import <QuartzCore/QuartzCore.h>
#import "CrittercismReachability.h"
//#import "<CoreLocation/CoreLocation.h>"

@protocol CrittercismDelegate <NSObject>

@optional
-(void) crittercismDidClose;
-(void) crittercismDidCrashOnLastLoad;
@end

@interface Crittercism : NSObject {
	NSMutableData *responseData;
	CFMutableDictionaryRef *connectionToInfoMapping;
	id <CrittercismDelegate> delegate;
    BOOL didCrashOnLastLoad;
}

@property(retain) id <CrittercismDelegate> delegate;
@property(assign) BOOL didCrashOnLastLoad;

+ (Crittercism*)sharedInstance;
+ (void) initWithAppID:(NSString *)_app_id;
+ (void) initWithAppID:(NSString *)_app_id andMainViewController:(UIViewController *)_mainViewController andDelegate:(id) critterDelegate;

// Deprecated in v3.3.1
+ (void) initWithAppID:(NSString *)_app_id andKey:(NSString *)_keyStr andSecret:(NSString *)_secretStr;
+ (void) initWithAppID:(NSString *)_app_id andKey:(NSString *)_keyStr andSecret:(NSString *)_secretStr andMainViewController:(UIViewController *)_mainViewController;

+ (NSString *) getAppID;
+ (void) configurePushNotification:(NSData *) deviceToken;
+ (int) getCurrentOrientation;
+ (void) setCurrentOrientation: (int)_orientation;
+ (void) setOptOutStatus: (BOOL) _optOutStatus;
+ (BOOL) getOptOutStatus;

+ (BOOL) logHandledException:(NSException *)exception;
+ (void) leaveBreadcrumb:(NSString *)breadcrumb;
+ (void) setAge:(int)age;
+ (void) setGender:(NSString *)gender;
+ (void) setUsername:(NSString *)username;
+ (void) setEmail:(NSString *)email;
+ (void) setValue:(NSString *)value forKey:(NSString *)key;
@end

