//
//  UDPController.h
//  SFS2X
//
//  Created by Wayne Helman on 12-01-20.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_EMBEDDED || TARGET_IPHONE_SIMULATOR
    #import <CFNetwork/CFNetwork.h>
#else
    #import <CoreServices/CoreServices.h>
#endif

#import "SFSEvent.h"

@protocol UDPControllerDelegate;

@interface UDPController : NSObject
{    
    id<UDPControllerDelegate>   _delegate;

    NSString        *_hostName;
    NSData          *_hostAddress;
    NSUInteger       _port;
    
    CFHostRef       _cfHost;
    CFSocketRef     _cfSocket;
}


@property (nonatomic, assign, readwrite) id<UDPControllerDelegate> delegate;
@property (nonatomic, copy,   readonly ) NSString *hostName;
@property (nonatomic, copy,   readonly ) NSData *hostAddress;
@property (nonatomic, assign, readonly ) NSUInteger port;


- (id)init;
- (void)startConnectionToHostName:(NSString *)hostName port:(NSUInteger)port;
- (void)sendData:(NSData *)data;
- (void)stop;


@end


@protocol UDPControllerDelegate <NSObject>

@optional

- (void)udpc:(UDPController *)udpc didReceiveData:(NSData *)data fromAddress:(NSData *)addr;
- (void)udpc:(UDPController *)udpc didReceiveError:(NSError *)error;
- (void)udpc:(UDPController *)udpc didSendData:(NSData *)data toAddress:(NSData *)addr;
- (void)udpc:(UDPController *)udpc didFailToSendData:(NSData *)data toAddress:(NSData *)addr error:(NSError *)error;
- (void)udpc:(UDPController *)udpc didStartWithAddress:(NSData *)address;
- (void)udpc:(UDPController *)udpc didStopWithError:(NSError *)error;

@end
