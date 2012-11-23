//
//  SFSProtocolCodec.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IProtocolCodec.h"
#import "IoHandler.h"

@class Logger;
@class BitSwarmClient;

@interface SFSProtocolCodec : NSObject <IProtocolCodec>{
@private
	id <IoHandler> _ioHandler;
	Logger *_log;
	BitSwarmClient *_bitSwarm;
}

@property (nonatomic, retain) id <IoHandler> ioHandler;
@property (nonatomic, retain) Logger *log;
@property (nonatomic, retain) BitSwarmClient *bitSwarm;

-(id)initWithIoHandler:(id <IoHandler>)ioHandler bitSwarm:(BitSwarmClient *)bitSwarm;

@end
