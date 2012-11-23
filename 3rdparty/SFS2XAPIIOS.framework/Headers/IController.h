//
//  IController.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@protocol IMessage;

@protocol IController <NSObject>

@required

-(NSInteger)id;

-(void)setId:(NSInteger)value;

-(void)handleMessage:(id <IMessage>)message;

@end