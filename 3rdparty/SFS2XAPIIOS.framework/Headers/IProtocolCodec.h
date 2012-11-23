//
//  IProtocolCodec.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@protocol IMessage;
@protocol IoHandler;

@protocol IProtocolCodec <NSObject>

-(void)onPacketRead:(NSData *)packet;
-(void)onPacketWrite:(id <IMessage>)message;

-(id <IoHandler>)ioHandler;
-(void)setIoHandler:(id <IoHandler>)handler;

@end