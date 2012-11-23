//
//  UDPManager.h
//  SFS2X
//
//  Created by Wayne Helman on 12-01-10.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Common.h"

#import "IBitSwarmEvents.h"
#import "UDPController.h"

EXTERN NSInteger const SFSIOHandler_SHORT_BYTE_SIZE;
EXTERN NSInteger const SFSIOHandler_INT_BYTE_SIZE;

EXTERN NSInteger const MAX_RETRY;
EXTERN NSInteger const RESPONSE_TIMEOUT;

@class BitSwarmClient;
@class SmartFox2XClient;
@class Logger;

/** Creates and manages a UDP socket connection to the server.
 
 This class is allocated on client startup, however it requires initialization.
 
 See <[SmartFox2XClient initUDP:::]>
 */ 
@interface UDPManager : NSObject <UDPControllerDelegate>
{
    @private
    
	SmartFox2XClient        *_sfs;
	BitSwarmClient          *_bitSwarm;
    Logger                  *_log;
    
    UDPController           *_udpc;
    NSTimer                 *_initThread;
    NSNumber                *_packetId;
    
    NSString                *_hostName;
    NSUInteger              _port;
    
    BOOL                    _locked;
    BOOL                    _initSuccess;
    NSInteger               _currentAttempt;
    
}

@property (retain) BitSwarmClient *bitSwarm;
@property (retain) SmartFox2XClient *sfs;

@property (nonatomic, retain, readwrite) UDPController *udpc;
@property (nonatomic, retain, readwrite) NSTimer *initThread;

@property (nonatomic, copy, readwrite) NSString *hostName;
@property (nonatomic, assign, readwrite) NSUInteger port;
@property (nonatomic) BOOL inited;



-(id)initWithBitSwarm:(BitSwarmClient *)bitSwarm host:(NSString *)host port:(NSInteger)port;

/** Internal method to set defaults
 */
-(id)initDefaults;

/** Internal method to initialize the UDPController 
 */
-(void)initializeWithHost:(NSString *)udpHost port:(NSInteger)updPort;

/** Internal method to reset the UDPManager when connection is closed
 */
-(void)reset;

/** Internal method to send UDP handshake to the server
 */
-(void)sendInitializationRequest;

/** Internal method to send UDP message to the server
 
 @param message the message to send to the server
 */
-(void)send:(NSData *) message;

/** Returns the next UDP packet Id
 
 @return NSNumber an incremented packet id
 */
-(NSNumber *)nextUdpPacketId;



@end


