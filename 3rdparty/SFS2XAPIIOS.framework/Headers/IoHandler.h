//
//  IoHandler.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@protocol IMessage;
@protocol IProtocolCodec;

@protocol IoHandler <NSObject> 
@required

-(void)onDataRead:(NSData *)buffer;

-(void)onDataWrite:(id <IMessage>)message;

-(id <IProtocolCodec>)codec;

@end