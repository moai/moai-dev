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


#define kEventAppInitSize               450//397 w/ push id, no inbox id
#define kEventAppExitSize               200//136 w/ only network type
#define kEventDeviceRegistrationSize    200//153 w/ only user info
#define kEventPushReceivedSize          200//160 w/ uuid push info
#define kEventAppActiveSize             120
#define kEventAppInactiveSize           120

@interface UAEvent : NSObject

@property (nonatomic, readonly, copy) NSString *time;
@property (nonatomic, readonly, copy) NSString *event_id;
@property (nonatomic, readonly, strong) NSMutableDictionary *data;

+ (id)event;
- (id)initWithContext:(NSDictionary *)context;
+ (id)eventWithContext:(NSDictionary *)context;
- (NSString *)getType;
- (void)gatherData:(NSDictionary *)context;
- (NSUInteger)getEstimatedSize;
- (void)addDataFromSessionForKey:(NSString *)dataKey;
- (void)addDataWithValue:(id)value forKey:(NSString *)key;
@end

@interface UAEventAppInit : UAEvent
@end

@interface UAEventAppForeground : UAEventAppInit
@end

@interface UAEventAppExit : UAEvent
@end

@interface UAEventAppBackground : UAEventAppExit
@end

@interface UAEventDeviceRegistration : UAEvent
@end

@interface UAEventPushReceived : UAEvent
@end

/**
 * This event is recorded when the app becomes active: on foreground
 * or when resuming after losing focus for any of the reasons that would
 * trigger a UAEventAppInactive event.
 */
@interface UAEventAppActive : UAEvent
@end

/**
 * This event is recorded when the app resigns its active state. This will happen
 * prior to backgrounding and when there is an incoming call, the user opens the
 * notification center in iOS5+, the user launches the task-bar, etc.
 */
@interface UAEventAppInactive : UAEvent
@end
