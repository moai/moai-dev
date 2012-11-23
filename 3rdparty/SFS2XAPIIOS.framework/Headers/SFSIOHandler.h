//
//  SFSIOHandler.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"
#import "IoHandler.h"

EXTERN NSInteger const SFSIOHandler_SHORT_BYTE_SIZE;
EXTERN NSInteger const SFSIOHandler_INT_BYTE_SIZE;

@class PendingPacket;
@class BitSwarmClient;
@class Logger;

@interface SFSIOHandler : NSObject <IoHandler>{
@private
	BitSwarmClient *_bitSwarm;
	Logger *_log;
	NSInteger _readState;
	PendingPacket *_pendingPacket;
	id <IProtocolCodec> _protocolCodec;
	NSData *_emptyBuffer;
}

-(id)initWithBitSwarmClient:(BitSwarmClient *)bitSwarm;

@property (nonatomic, retain) BitSwarmClient *bitSwarm;
@property (nonatomic, retain) Logger *log;
@property (nonatomic, readwrite) NSInteger readState;
@property (nonatomic, retain) id <IProtocolCodec> codec;
@property (nonatomic, retain) PendingPacket *pendingPacket;

@end
