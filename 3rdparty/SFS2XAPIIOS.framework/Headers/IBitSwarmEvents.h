//
//  IBitSwarmEvents.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@class BitSwarmEvent;

@protocol IBitSwarmEvents <NSObject>

@required

-(void)onSocketConnect:(BitSwarmEvent *)evt;
-(void)onSocketReconnectionTry:(BitSwarmEvent *)evt;
-(void)onSocketIOError:(BitSwarmEvent *)evt;
-(void)onSocketClose:(BitSwarmEvent *)evt;

@end